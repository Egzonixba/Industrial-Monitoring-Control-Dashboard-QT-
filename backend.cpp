#include "backend.h"
#include <QStandardPaths>
#include <QDir>
#include <QDebug>

DashboardBackend::DashboardBackend(QObject* parent)
    : QObject(parent)
{
    // Sensor simulation timer — fires every 1 second
    m_sensorTimer.setInterval(1000);
    connect(&m_sensorTimer, &QTimer::timeout, this, &DashboardBackend::simulateSensorData);

    // Run clock timer — fires every 1 second to update displayed time
    m_runClockTimer.setInterval(1000);
    connect(&m_runClockTimer, &QTimer::timeout, this, &DashboardBackend::updateRunTime);

    // Flush timer — single shot 3 seconds
    m_flushTimer.setSingleShot(true);
    m_flushTimer.setInterval(3000);
    connect(&m_flushTimer, &QTimer::timeout, this, &DashboardBackend::finishFlushing);
}

// ─── Login ───────────────────────────────────────────────

bool DashboardBackend::login(const QString& username, const QString& password)
{
    // Simple hardcoded validation — replace with real auth later
    // Accept any non-empty username with password "1234" for demo
    if (username.trimmed().isEmpty()) {
        emit loginFailed("Username cannot be empty");
        return false;
    }
    if (password.isEmpty()) {
        emit loginFailed("Password cannot be empty");
        return false;
    }

    // Demo credentials: any username, password = "1234"
    if (password != "1234") {
        emit loginFailed("Invalid password (hint: use 1234)");
        return false;
    }

    m_currentUser = username.trimmed();
    emit currentUserChanged();
    return true;
}

void DashboardBackend::logout()
{
    if (m_running)
        finishRun();

    m_currentUser.clear();
    emit currentUserChanged();
}

// ─── Setpoints ───────────────────────────────────────────

void DashboardBackend::setVesselTempSetpoint(double v) {
    if (qFuzzyCompare(m_vesselTempSetpoint, v)) return;
    m_vesselTempSetpoint = v;
    emit vesselTempSetpointChanged();
}

void DashboardBackend::setPumpPressureSetpoint(double v) {
    if (qFuzzyCompare(m_pumpPressureSetpoint, v)) return;
    m_pumpPressureSetpoint = v;
    emit pumpPressureSetpointChanged();
}

void DashboardBackend::setPumpRpmSetpoint(double v) {
    if (qFuzzyCompare(m_pumpRpmSetpoint, v)) return;
    m_pumpRpmSetpoint = v;
    emit pumpRpmSetpointChanged();
}

void DashboardBackend::setChillerTempSetpoint(double v) {
    if (qFuzzyCompare(m_chillerTempSetpoint, v)) return;
    m_chillerTempSetpoint = v;
    emit chillerTempSetpointChanged();
}

void DashboardBackend::setDryAirFlowSetpoint(double v) {
    if (qFuzzyCompare(m_dryAirFlowSetpoint, v)) return;
    m_dryAirFlowSetpoint = v;
    emit dryAirFlowSetpointChanged();
}

// ─── Run Control ─────────────────────────────────────────

void DashboardBackend::startRun()
{
    if (m_running) return;

    m_running = true;
    m_emergencyStopped = false;
    m_sampleCount = 0;
    m_runElapsedMs = 0;

    // Reset live values to 0 initially — they'll ramp up
    m_vesselTemp = 0;
    m_pumpPressure = 0;
    m_pumpRpm = 0;
    m_chillerTemp = 0;
    m_dryAirFlow = 0;

    clearHistory();
    m_runLog = QJsonArray();

    m_runTimer.start();
    m_sensorTimer.start();
    m_runClockTimer.start();

    emit runningChanged();
    emit emergencyStoppedChanged();
    emit sensorDataChanged();
    emit runTimeChanged();
}

void DashboardBackend::finishRun()
{
    if (!m_running) return;

    m_running = false;
    m_sensorTimer.stop();
    m_runClockTimer.stop();
    m_runElapsedMs = m_runTimer.elapsed();

    emit runningChanged();
    emit runTimeChanged();
}

void DashboardBackend::emergencyStop()
{
    m_emergencyStopped = true;
    m_running = false;
    m_sensorTimer.stop();
    m_runClockTimer.stop();
    m_runElapsedMs = m_runTimer.elapsed();

    // All readings go to 0
    m_vesselTemp = 0;
    m_pumpPressure = 0;
    m_pumpRpm = 0;
    m_chillerTemp = 0;
    m_dryAirFlow = 0;

    // Pump off
    m_pumpRunning = false;

    emit runningChanged();
    emit emergencyStoppedChanged();
    emit sensorDataChanged();
    emit runTimeChanged();
    emit pumpStateChanged();
}

bool DashboardBackend::saveRun()
{
    QJsonObject runData;
    runData["batchId"] = m_batchId;
    runData["recipe"] = m_recipe;
    runData["operator"] = m_currentUser;
    runData["startTime"] = QDateTime::currentDateTime().addMSecs(-m_runElapsedMs).toString(Qt::ISODate);
    runData["endTime"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    runData["durationSeconds"] = (int)(m_runElapsedMs / 1000);
    runData["emergencyStopped"] = m_emergencyStopped;

    // Setpoints
    QJsonObject setpoints;
    setpoints["vesselTemp_C"] = m_vesselTempSetpoint;
    setpoints["pumpPressure_Pa"] = m_pumpPressureSetpoint;
    setpoints["pumpRpm"] = m_pumpRpmSetpoint;
    setpoints["chillerTemp_C"] = m_chillerTempSetpoint;
    setpoints["dryAirFlow_kgm3"] = m_dryAirFlowSetpoint;
    runData["setpoints"] = setpoints;

    // Data log
    runData["dataLog"] = m_runLog;

    QJsonDocument doc(runData);

    // Save to Documents folder
    QString dirPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
        + "/MQTTDashboard/Runs";
    QDir().mkpath(dirPath);

    QString filename = QString("%1/Run_%2_%3.json")
        .arg(dirPath)
        .arg(m_batchId.isEmpty() ? "NoBatch" : m_batchId)
        .arg(QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss"));

    QFile file(filename);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
        qDebug() << "Run saved to:" << filename;
        emit runSaved(filename);
        return true;
    }

    qWarning() << "Failed to save run to:" << filename;
    return false;
}

// ─── Run Time ────────────────────────────────────────────

QString DashboardBackend::runTimeString() const
{
    qint64 ms = m_running ? m_runTimer.elapsed() : m_runElapsedMs;
    int totalSecs = (int)(ms / 1000);
    int h = totalSecs / 3600;
    int m = (totalSecs % 3600) / 60;
    int s = totalSecs % 60;
    return QString("%1:%2:%3")
        .arg(h, 2, 10, QChar('0'))
        .arg(m, 2, 10, QChar('0'))
        .arg(s, 2, 10, QChar('0'));
}

int DashboardBackend::runElapsedSeconds() const
{
    qint64 ms = m_running ? m_runTimer.elapsed() : m_runElapsedMs;
    return (int)(ms / 1000);
}

void DashboardBackend::updateRunTime()
{
    emit runTimeChanged();
}

// ─── Batch / Recipe ──────────────────────────────────────

void DashboardBackend::setBatchId(const QString& id) {
    if (m_batchId == id) return;
    m_batchId = id;
    emit batchIdChanged();
}

void DashboardBackend::setRecipe(const QString& r) {
    if (m_recipe == r) return;
    m_recipe = r;
    emit recipeChanged();
}

QStringList DashboardBackend::recipeList() const
{
    return {
        "Standard Drying - 60°C",
        "High Pressure Cycle",
        "Low Temp Preservation",
        "Rapid Cool-Down",
        "Custom Recipe A",
        "Custom Recipe B"
    };
}

// ─── Pump ────────────────────────────────────────────────

void DashboardBackend::pumpOn()
{
    if (m_pumpRunning) return;
    m_pumpRunning = true;
    emit pumpStateChanged();
}

void DashboardBackend::pumpOff()
{
    if (!m_pumpRunning) return;
    m_pumpRunning = false;
    emit pumpStateChanged();
}

void DashboardBackend::flushPump()
{
    if (m_flushing) return;
    m_flushing = true;
    emit pumpStateChanged();
    m_flushTimer.start();
}

void DashboardBackend::finishFlushing()
{
    m_flushing = false;
    emit pumpStateChanged();
}

// ─── Sensor Simulation ──────────────────────────────────

void DashboardBackend::simulateSensorData()
{
    if (!m_running || m_emergencyStopped) return;

    m_sampleCount++;
    double t = m_sampleCount; // seconds

    auto rng = QRandomGenerator::global();

    // Simulate values ramping toward setpoints with noise
    // Simple exponential approach: value = setpoint * (1 - exp(-t/tau)) + noise
    double tau = 15.0; // time constant in seconds
    double factor = 1.0 - qExp(-t / tau);

    m_vesselTemp = m_vesselTempSetpoint * factor + rng->bounded(2.0) - 1.0;
    m_pumpPressure = m_pumpPressureSetpoint * factor + rng->bounded(500.0) - 250.0;
    m_pumpRpm = m_pumpRpmSetpoint * factor + rng->bounded(20.0) - 10.0;
    m_chillerTemp = m_chillerTempSetpoint * factor + rng->bounded(1.0) - 0.5;
    m_dryAirFlow = m_dryAirFlowSetpoint * factor + rng->bounded(0.1) - 0.05;

    // Clamp negatives
    m_vesselTemp = qMax(0.0, m_vesselTemp);
    m_pumpPressure = qMax(0.0, m_pumpPressure);
    m_pumpRpm = qMax(0.0, m_pumpRpm);
    m_chillerTemp = qMax(0.0, m_chillerTemp);
    m_dryAirFlow = qMax(0.0, m_dryAirFlow);

    // Store history
    m_timeHistory.append(t);
    m_vesselTempHistory.append(m_vesselTemp);
    m_pumpPressureHistory.append(m_pumpPressure);
    m_pumpRpmHistory.append(m_pumpRpm);
    m_chillerTempHistory.append(m_chillerTemp);
    m_dryAirFlowHistory.append(m_dryAirFlow);

    // Keep last 300 samples (5 minutes at 1Hz)
    const int maxSamples = 300;
    if (m_timeHistory.size() > maxSamples) {
        m_timeHistory.removeFirst();
        m_vesselTempHistory.removeFirst();
        m_pumpPressureHistory.removeFirst();
        m_pumpRpmHistory.removeFirst();
        m_chillerTempHistory.removeFirst();
        m_dryAirFlowHistory.removeFirst();
    }

    // Log entry for saving
    QJsonObject entry;
    entry["t"] = t;
    entry["vesselTemp"] = m_vesselTemp;
    entry["pumpPressure"] = m_pumpPressure;
    entry["pumpRpm"] = m_pumpRpm;
    entry["chillerTemp"] = m_chillerTemp;
    entry["dryAirFlow"] = m_dryAirFlow;
    m_runLog.append(entry);

    emit sensorDataChanged();
}

void DashboardBackend::clearHistory()
{
    m_vesselTempHistory.clear();
    m_pumpPressureHistory.clear();
    m_pumpRpmHistory.clear();
    m_chillerTempHistory.clear();
    m_dryAirFlowHistory.clear();
    m_timeHistory.clear();
}