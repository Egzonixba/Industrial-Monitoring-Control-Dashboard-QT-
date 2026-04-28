#include "backend.h"
#include <QStandardPaths>
#include <QDir>
#include <QDebug>

DashboardBackend::DashboardBackend(QObject *parent)
    : QObject(parent)
{
    m_sensorTimer.setInterval(1000);
    connect(&m_sensorTimer, &QTimer::timeout, this, &DashboardBackend::simulateSensorData);

    m_runClockTimer.setInterval(1000);
    connect(&m_runClockTimer, &QTimer::timeout, this, &DashboardBackend::updateRunTime);

    // Flush countdown timer — fires every 1 second
    m_flushTimer.setInterval(1000);
    connect(&m_flushTimer, &QTimer::timeout, this, &DashboardBackend::flushTick);

    // Post-flush delay before navigating (1 second after pump goes green)
    m_postFlushTimer.setSingleShot(true);
    m_postFlushTimer.setInterval(1000);
    connect(&m_postFlushTimer, &QTimer::timeout, this, &DashboardBackend::onPumpSequenceReady);
}

// ─── Login ───────────────────────────────────────────────

bool DashboardBackend::login(const QString &username, const QString &password)
{
    if (username.trimmed().isEmpty()) {
        emit loginFailed("Username cannot be empty");
        return false;
    }
    if (password.isEmpty()) {
        emit loginFailed("Password cannot be empty");
        return false;
    }
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
    // Reset alarm breach timer when setpoint changes
    m_vesselTempBreachTime = 0;
    m_vesselTempAlarm = 0;
    emit alarmStateChanged();
}

void DashboardBackend::setPumpPressureSetpoint(double v) {
    if (qFuzzyCompare(m_pumpPressureSetpoint, v)) return;
    m_pumpPressureSetpoint = v;
    emit pumpPressureSetpointChanged();
    m_pumpPressureBreachTime = 0;
    m_pumpPressureAlarm = 0;
    emit alarmStateChanged();
}

void DashboardBackend::setPumpRpmSetpoint(double v) {
    if (qFuzzyCompare(m_pumpRpmSetpoint, v)) return;
    m_pumpRpmSetpoint = v;
    emit pumpRpmSetpointChanged();
    m_pumpRpmBreachTime = 0;
    m_pumpRpmAlarm = 0;
    emit alarmStateChanged();
}

void DashboardBackend::setChillerTempSetpoint(double v) {
    if (qFuzzyCompare(m_chillerTempSetpoint, v)) return;
    m_chillerTempSetpoint = v;
    emit chillerTempSetpointChanged();
    m_chillerTempBreachTime = 0;
    m_chillerTempAlarm = 0;
    emit alarmStateChanged();
}

void DashboardBackend::setCo2FlowSetpoint(double v) {
    if (qFuzzyCompare(m_co2FlowSetpoint, v)) return;
    m_co2FlowSetpoint = v;
    emit co2FlowSetpointChanged();
    m_co2FlowBreachTime = 0;
    m_co2FlowAlarm = 0;
    emit alarmStateChanged();
}

// ─── Recipe ──────────────────────────────────────────────

QStringList DashboardBackend::recipeList() const
{
    return {
        "Standard Drying",
        "High Pressure Cycle",
        "Low Temp Preservation",
        "Rapid Cool-Down",
        "Sterilization Cycle",
        "Custom"
    };
}

void DashboardBackend::applyRecipe(const QString &recipeName)
{
    qDebug() << "Applying recipe:" << recipeName;

    if (recipeName == "Standard Drying") {
        setVesselTempSetpoint(35.0);
        setPumpPressureSetpoint(750.0);
        setPumpRpmSetpoint(1500.0);
        setChillerTempSetpoint(5.0);
        setCo2FlowSetpoint(4500.0);
    }
    else if (recipeName == "High Pressure Cycle") {
        setVesselTempSetpoint(45.0);
        setPumpPressureSetpoint(2500.0);
        setPumpRpmSetpoint(3000.0);
        setChillerTempSetpoint(8.0);
        setCo2FlowSetpoint(8000.0);
    }
    else if (recipeName == "Low Temp Preservation") {
        setVesselTempSetpoint(10.0);
        setPumpPressureSetpoint(400.0);
        setPumpRpmSetpoint(800.0);
        setChillerTempSetpoint(-5.0);
        setCo2FlowSetpoint(2000.0);
    }
    else if (recipeName == "Rapid Cool-Down") {
        setVesselTempSetpoint(5.0);
        setPumpPressureSetpoint(600.0);
        setPumpRpmSetpoint(2000.0);
        setChillerTempSetpoint(-15.0);
        setCo2FlowSetpoint(6000.0);
    }
    else if (recipeName == "Sterilization Cycle") {
        setVesselTempSetpoint(121.0);
        setPumpPressureSetpoint(3500.0);
        setPumpRpmSetpoint(4000.0);
        setChillerTempSetpoint(20.0);
        setCo2FlowSetpoint(10000.0);
    }
    // "Custom" — don't change anything, user sets manually
}

void DashboardBackend::setBatchId(const QString &id) {
    if (m_batchId == id) return;
    m_batchId = id;
    emit batchIdChanged();
}

void DashboardBackend::setRecipe(const QString &r) {
    if (m_recipe == r) return;
    m_recipe = r;
    emit recipeChanged();
}

// ─── Pump Sequence ───────────────────────────────────────

void DashboardBackend::startPumpSequence()
{
    if (m_flushing || m_pumpRunning) return;

    qDebug() << "Starting pump sequence: flushing for 5 seconds";
    m_flushing = true;
    m_flushCountdown = 5;
    emit pumpStateChanged();
    emit flushCountdownChanged();

    qDebug() << "Emitting requestPumpCommand FLUSH";
    emit requestPumpCommand("FLUSH");

    m_flushTimer.start();
}

void DashboardBackend::flushTick()
{
    m_flushCountdown--;
    emit flushCountdownChanged();
    qDebug() << "Flush countdown:" << m_flushCountdown;

    if (m_flushCountdown <= 0) {
        m_flushTimer.stop();
        m_flushing = false;
        m_pumpRunning = true;
        emit pumpStateChanged();

        qDebug() << "Pump is ON (green), waiting 1s before starting run";
        // Wait 1 second then signal ready
        m_postFlushTimer.start();
    }
}

void DashboardBackend::onPumpSequenceReady()
{
    qDebug() << "Pump sequence complete, starting run";

    qDebug() << "Emitting requestPumpCommand ON";
    emit requestPumpCommand("ON");

    qDebug() << "Emitting requestRunCommand START";
    emit requestRunCommand("START");

    startRun();
    emit pumpSequenceComplete();
}

// ─── Run Control ─────────────────────────────────────────

void DashboardBackend::startRun()
{
    if (m_running) return;

    m_running = true;
    m_emergencyStopped = false;
    m_sampleCount = 0;
    m_runElapsedMs = 0;
    m_lastHistorySecond = -1;

    m_vesselTemp = 0;
    m_pumpPressure = 0;
    m_pumpRpm = 0;
    m_chillerTemp = 0;
    m_co2Flow = 0;

    // Reset alarms
    m_vesselTempAlarm = 0;
    m_pumpPressureAlarm = 0;
    m_pumpRpmAlarm = 0;
    m_chillerTempAlarm = 0;
    m_co2FlowAlarm = 0;
    m_vesselTempBreachTime = 0;
    m_pumpPressureBreachTime = 0;
    m_pumpRpmBreachTime = 0;
    m_chillerTempBreachTime = 0;
    m_co2FlowBreachTime = 0;

    clearHistory();
    m_runLog = QJsonArray();

    m_runTimer.start();
    m_sensorTimer.start();
    m_runClockTimer.start();

    qDebug() << "Run started. Data source:" <<(m_useMqtt ? "MQTT" : "Simulation");

    emit runningChanged();
    emit emergencyStoppedChanged();
    emit sensorDataChanged();
    emit alarmStateChanged();
    emit runTimeChanged();
}

void DashboardBackend::finishRun()
{
    if (!m_running) return;

    qDebug() << "Emitting requestRunCommand STOP";
    emit requestRunCommand("STOP");

    m_running = false;
    m_sensorTimer.stop();
    m_runClockTimer.stop();
    m_runElapsedMs = m_runTimer.elapsed();

    emit runningChanged();
    emit runTimeChanged();
}

void DashboardBackend::emergencyStop()
{
    qDebug() << "Emitting requestRunCommand EMERGENCY";
    emit requestRunCommand("EMERGENCY");

    qDebug() << "Emitting requestPumpCommand OFF";
    emit requestPumpCommand("OFF");

    m_emergencyStopped = true;
    m_running = false;
    m_sensorTimer.stop();
    m_runClockTimer.stop();
    m_flushTimer.stop();
    m_postFlushTimer.stop();
    m_runElapsedMs = m_runTimer.elapsed();

    m_vesselTemp = 0;
    m_pumpPressure = 0;
    m_pumpRpm = 0;
    m_chillerTemp = 0;
    m_co2Flow = 0;

    m_pumpRunning = false;
    m_flushing = false;
    m_flushCountdown = 0;

    m_vesselTempAlarm = 0;
    m_pumpPressureAlarm = 0;
    m_pumpRpmAlarm = 0;
    m_chillerTempAlarm = 0;
    m_co2FlowAlarm = 0;

    emit runningChanged();
    emit emergencyStoppedChanged();
    emit sensorDataChanged();
    emit alarmStateChanged();
    emit runTimeChanged();
    emit pumpStateChanged();
    emit flushCountdownChanged();
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

    QJsonObject setpoints;
    setpoints["vesselTemp_C"] = m_vesselTempSetpoint;
    setpoints["pumpPressure_Pa"] = m_pumpPressureSetpoint;
    setpoints["pumpRpm"] = m_pumpRpmSetpoint;
    setpoints["chillerTemp_C"] = m_chillerTempSetpoint;
    setpoints["co2Flow_Pa"] = m_co2FlowSetpoint;
    runData["setpoints"] = setpoints;
    runData["dataLog"] = m_runLog;

    QJsonDocument doc(runData);
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

        // Reset pump state for next run
        m_pumpRunning = false;
        emit pumpStateChanged();
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

// ─── Sensor Simulation ──────────────────────────────────

void DashboardBackend::simulateSensorData()
{
    if (m_useMqtt) return;

    if (!m_running || m_emergencyStopped) return;

    m_sampleCount++;
    double t = m_sampleCount;

    auto rng = QRandomGenerator::global();

    double tau = 15.0;
    double factor = 1.0 - qExp(-t / tau);

    m_vesselTemp = m_vesselTempSetpoint * factor + rng->bounded(2.0) - 1.0;
    m_pumpPressure = m_pumpPressureSetpoint * factor + rng->bounded(80.0) - 40.0;
    m_pumpRpm = m_pumpRpmSetpoint * factor + rng->bounded(200.0) - 100.0;
    m_chillerTemp = m_chillerTempSetpoint * factor + rng->bounded(1.5) - 0.75;
    m_co2Flow = m_co2FlowSetpoint * factor + rng->bounded(150.0) - 75.0;

    // Allow negative for chiller
    m_vesselTemp = qMax(0.0, m_vesselTemp);
    m_pumpPressure = qMax(0.0, m_pumpPressure);
    m_pumpRpm = qMax(0.0, m_pumpRpm);
    m_co2Flow = qMax(0.0, m_co2Flow);

    // Store history
    m_timeHistory.append(t);
    m_vesselTempHistory.append(m_vesselTemp);
    m_pumpPressureHistory.append(m_pumpPressure);
    m_pumpRpmHistory.append(m_pumpRpm);
    m_chillerTempHistory.append(m_chillerTemp);
    m_co2FlowHistory.append(m_co2Flow);

    const int maxSamples = 300;
    if (m_timeHistory.size() > maxSamples) {
        m_timeHistory.removeFirst();
        m_vesselTempHistory.removeFirst();
        m_pumpPressureHistory.removeFirst();
        m_pumpRpmHistory.removeFirst();
        m_chillerTempHistory.removeFirst();
        m_co2FlowHistory.removeFirst();
    }

    QJsonObject entry;
    entry["t"] = t;
    entry["vesselTemp"] = m_vesselTemp;
    entry["pumpPressure"] = m_pumpPressure;
    entry["pumpRpm"] = m_pumpRpm;
    entry["chillerTemp"] = m_chillerTemp;
    entry["co2Flow"] = m_co2Flow;
    m_runLog.append(entry);

    emit sensorDataChanged();

    // Check alarms after emitting sensor data
    checkAlarms();
}

void DashboardBackend::checkAlarms()
{
    int oldVessel = m_vesselTempAlarm;
    int oldPressure = m_pumpPressureAlarm;
    int oldRpm = m_pumpRpmAlarm;
    int oldChiller = m_chillerTempAlarm;
    int oldCo2 = m_co2FlowAlarm;

    // --- Vessel Temp: high +2, low -5 ---
    double vesselDiff = m_vesselTemp - m_vesselTempSetpoint;
    if (vesselDiff > 2.0) {
        m_vesselTempBreachTime++;
        if (m_vesselTempBreachTime >= ALARM_DELAY_SECONDS)
            m_vesselTempAlarm = HighAlarm;
    } else if (vesselDiff < -5.0) {
        m_vesselTempBreachTime++;
        if (m_vesselTempBreachTime >= ALARM_DELAY_SECONDS)
            m_vesselTempAlarm = LowAlarm;
    } else {
        m_vesselTempBreachTime = 0;
        m_vesselTempAlarm = Normal;
    }

    // --- Pump Pressure: high +50, low -100 ---
    double pressureDiff = m_pumpPressure - m_pumpPressureSetpoint;
    if (pressureDiff > 50.0) {
        m_pumpPressureBreachTime++;
        if (m_pumpPressureBreachTime >= ALARM_DELAY_SECONDS)
            m_pumpPressureAlarm = HighAlarm;
    } else if (pressureDiff < -100.0) {
        m_pumpPressureBreachTime++;
        if (m_pumpPressureBreachTime >= ALARM_DELAY_SECONDS)
            m_pumpPressureAlarm = LowAlarm;
    } else {
        m_pumpPressureBreachTime = 0;
        m_pumpPressureAlarm = Normal;
    }

    // --- Pump RPM: high +300, low -300 ---
    double rpmDiff = m_pumpRpm - m_pumpRpmSetpoint;
    if (rpmDiff > 300.0) {
        m_pumpRpmBreachTime++;
        if (m_pumpRpmBreachTime >= ALARM_DELAY_SECONDS)
            m_pumpRpmAlarm = HighAlarm;
    } else if (rpmDiff < -300.0) {
        m_pumpRpmBreachTime++;
        if (m_pumpRpmBreachTime >= ALARM_DELAY_SECONDS)
            m_pumpRpmAlarm = LowAlarm;
    } else {
        m_pumpRpmBreachTime = 0;
        m_pumpRpmAlarm = Normal;
    }

    // --- Chiller Temp: high +1, low -2 ---
    double chillerDiff = m_chillerTemp - m_chillerTempSetpoint;
    if (chillerDiff > 1.0) {
        m_chillerTempBreachTime++;
        if (m_chillerTempBreachTime >= ALARM_DELAY_SECONDS)
            m_chillerTempAlarm = HighAlarm;
    } else if (chillerDiff < -2.0) {
        m_chillerTempBreachTime++;
        if (m_chillerTempBreachTime >= ALARM_DELAY_SECONDS)
            m_chillerTempAlarm = LowAlarm;
    } else {
        m_chillerTempBreachTime = 0;
        m_chillerTempAlarm = Normal;
    }

    // --- CO2 Flow: high +100, low -200 ---
    double co2Diff = m_co2Flow - m_co2FlowSetpoint;
    if (co2Diff > 100.0) {
        m_co2FlowBreachTime++;
        if (m_co2FlowBreachTime >= ALARM_DELAY_SECONDS)
            m_co2FlowAlarm = HighAlarm;
    } else if (co2Diff < -200.0) {
        m_co2FlowBreachTime++;
        if (m_co2FlowBreachTime >= ALARM_DELAY_SECONDS)
            m_co2FlowAlarm = LowAlarm;
    } else {
        m_co2FlowBreachTime = 0;
        m_co2FlowAlarm = Normal;
    }

    if (oldVessel != m_vesselTempAlarm || oldPressure != m_pumpPressureAlarm ||
        oldRpm != m_pumpRpmAlarm || oldChiller != m_chillerTempAlarm ||
        oldCo2 != m_co2FlowAlarm) {
        emit alarmStateChanged();
    }
}

void DashboardBackend::clearHistory()
{
    m_vesselTempHistory.clear();
    m_pumpPressureHistory.clear();
    m_pumpRpmHistory.clear();
    m_chillerTempHistory.clear();
    m_co2FlowHistory.clear();
    m_timeHistory.clear();
}
// ─── MQTT Mode ───────────────────────────────────────────

void DashboardBackend::setUseMqtt(bool v)
{
    if (m_useMqtt == v) return;
    m_useMqtt = v;
    emit useMqttChanged();
    qDebug() << "Data source:" << (m_useMqtt ? "MQTT" : "Simulation");
}

// ─── MQTT Live Setters ──────────────────────────────────

void DashboardBackend::setLiveVesselTemp(double v)
{
    if (!m_useMqtt || !m_running) return;
    m_vesselTemp = v;
    appendToHistory();
}

void DashboardBackend::setLivePumpPressure(double v)
{
    if (!m_useMqtt || !m_running) return;
    m_pumpPressure = v;
    appendToHistory();
}

void DashboardBackend::setLivePumpRpm(double v)
{
    if (!m_useMqtt || !m_running) return;
    m_pumpRpm = v;
    appendToHistory();
}

void DashboardBackend::setLiveChillerTemp(double v)
{
    if (!m_useMqtt || !m_running) return;
    m_chillerTemp = v;
    appendToHistory();
}

void DashboardBackend::setLiveCo2Flow(double v)
{
    if (!m_useMqtt || !m_running) return;
    m_co2Flow = v;
    appendToHistory();
}

void DashboardBackend::appendToHistory()
{
    if (!m_running) return;

    int currentSecond = (int)(m_runTimer.elapsed() / 1000);

    if (currentSecond == m_lastHistorySecond) {
        emit sensorDataChanged();
        checkAlarms();
        return;
    }
    m_lastHistorySecond = currentSecond;

    double t = currentSecond;

    m_timeHistory.append(t);
    m_vesselTempHistory.append(m_vesselTemp);
    m_pumpPressureHistory.append(m_pumpPressure);
    m_pumpRpmHistory.append(m_pumpRpm);
    m_chillerTempHistory.append(m_chillerTemp);
    m_co2FlowHistory.append(m_co2Flow);

    const int maxSamples = 300;
    if (m_timeHistory.size() > maxSamples) {
        m_timeHistory.removeFirst();
        m_vesselTempHistory.removeFirst();
        m_pumpPressureHistory.removeFirst();
        m_pumpRpmHistory.removeFirst();
        m_chillerTempHistory.removeFirst();
        m_co2FlowHistory.removeFirst();
    }

    QJsonObject entry;
    entry["t"] = t;
    entry["vesselTemp"] = m_vesselTemp;
    entry["pumpPressure"] = m_pumpPressure;
    entry["pumpRpm"] = m_pumpRpm;
    entry["chillerTemp"] = m_chillerTemp;
    entry["co2Flow"] = m_co2Flow;
    m_runLog.append(entry);

    emit sensorDataChanged();
    checkAlarms();
}