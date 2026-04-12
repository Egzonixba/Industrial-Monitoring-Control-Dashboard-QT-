#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QTimer>
#include <QElapsedTimer>
#include <QVariantList>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QtMath>
#include <QRandomGenerator>

class DashboardBackend : public QObject
{
    Q_OBJECT

        // --- Login ---
        Q_PROPERTY(QString currentUser READ currentUser NOTIFY currentUserChanged)

        // --- Process Parameters (setpoints from MainOperationView) ---
        Q_PROPERTY(double vesselTempSetpoint READ vesselTempSetpoint WRITE setVesselTempSetpoint NOTIFY vesselTempSetpointChanged)
        Q_PROPERTY(double pumpPressureSetpoint READ pumpPressureSetpoint WRITE setPumpPressureSetpoint NOTIFY pumpPressureSetpointChanged)
        Q_PROPERTY(double pumpRpmSetpoint READ pumpRpmSetpoint WRITE setPumpRpmSetpoint NOTIFY pumpRpmSetpointChanged)
        Q_PROPERTY(double chillerTempSetpoint READ chillerTempSetpoint WRITE setChillerTempSetpoint NOTIFY chillerTempSetpointChanged)
        Q_PROPERTY(double dryAirFlowSetpoint READ dryAirFlowSetpoint WRITE setDryAirFlowSetpoint NOTIFY dryAirFlowSetpointChanged)

        // --- Live sensor values (monitoring) ---
        Q_PROPERTY(double vesselTemp READ vesselTemp NOTIFY sensorDataChanged)
        Q_PROPERTY(double pumpPressure READ pumpPressure NOTIFY sensorDataChanged)
        Q_PROPERTY(double pumpRpm READ pumpRpm NOTIFY sensorDataChanged)
        Q_PROPERTY(double chillerTemp READ chillerTemp NOTIFY sensorDataChanged)
        Q_PROPERTY(double dryAirFlow READ dryAirFlow NOTIFY sensorDataChanged)

        // --- Run state ---
        Q_PROPERTY(bool running READ running NOTIFY runningChanged)
        Q_PROPERTY(bool emergencyStopped READ emergencyStopped NOTIFY emergencyStoppedChanged)
        Q_PROPERTY(QString runTimeString READ runTimeString NOTIFY runTimeChanged)
        Q_PROPERTY(int runElapsedSeconds READ runElapsedSeconds NOTIFY runTimeChanged)

        // --- Batch / Recipe ---
        Q_PROPERTY(QString batchId READ batchId WRITE setBatchId NOTIFY batchIdChanged)
        Q_PROPERTY(QString recipe READ recipe WRITE setRecipe NOTIFY recipeChanged)
        Q_PROPERTY(QStringList recipeList READ recipeList CONSTANT)

        // --- Pump ---
        Q_PROPERTY(bool pumpRunning READ pumpRunning NOTIFY pumpStateChanged)
        Q_PROPERTY(bool flushing READ flushing NOTIFY pumpStateChanged)

        // --- Chart data ---
        Q_PROPERTY(QVariantList vesselTempHistory READ vesselTempHistory NOTIFY sensorDataChanged)
        Q_PROPERTY(QVariantList pumpPressureHistory READ pumpPressureHistory NOTIFY sensorDataChanged)
        Q_PROPERTY(QVariantList pumpRpmHistory READ pumpRpmHistory NOTIFY sensorDataChanged)
        Q_PROPERTY(QVariantList chillerTempHistory READ chillerTempHistory NOTIFY sensorDataChanged)
        Q_PROPERTY(QVariantList dryAirFlowHistory READ dryAirFlowHistory NOTIFY sensorDataChanged)
        Q_PROPERTY(QVariantList timeHistory READ timeHistory NOTIFY sensorDataChanged)

public:
    explicit DashboardBackend(QObject* parent = nullptr);

    // Login
    QString currentUser() const { return m_currentUser; }
    Q_INVOKABLE bool login(const QString& username, const QString& password);
    Q_INVOKABLE void logout();

    // Setpoints
    double vesselTempSetpoint() const { return m_vesselTempSetpoint; }
    double pumpPressureSetpoint() const { return m_pumpPressureSetpoint; }
    double pumpRpmSetpoint() const { return m_pumpRpmSetpoint; }
    double chillerTempSetpoint() const { return m_chillerTempSetpoint; }
    double dryAirFlowSetpoint() const { return m_dryAirFlowSetpoint; }

    void setVesselTempSetpoint(double v);
    void setPumpPressureSetpoint(double v);
    void setPumpRpmSetpoint(double v);
    void setChillerTempSetpoint(double v);
    void setDryAirFlowSetpoint(double v);

    // Live values
    double vesselTemp() const { return m_vesselTemp; }
    double pumpPressure() const { return m_pumpPressure; }
    double pumpRpm() const { return m_pumpRpm; }
    double chillerTemp() const { return m_chillerTemp; }
    double dryAirFlow() const { return m_dryAirFlow; }

    // Run
    bool running() const { return m_running; }
    bool emergencyStopped() const { return m_emergencyStopped; }
    QString runTimeString() const;
    int runElapsedSeconds() const;

    Q_INVOKABLE void startRun();
    Q_INVOKABLE void finishRun();
    Q_INVOKABLE void emergencyStop();
    Q_INVOKABLE bool saveRun();

    // Batch
    QString batchId() const { return m_batchId; }
    void setBatchId(const QString& id);
    QString recipe() const { return m_recipe; }
    void setRecipe(const QString& r);
    QStringList recipeList() const;

    // Pump
    bool pumpRunning() const { return m_pumpRunning; }
    bool flushing() const { return m_flushing; }
    Q_INVOKABLE void pumpOn();
    Q_INVOKABLE void pumpOff();
    Q_INVOKABLE void flushPump();

    // Chart data
    QVariantList vesselTempHistory() const { return m_vesselTempHistory; }
    QVariantList pumpPressureHistory() const { return m_pumpPressureHistory; }
    QVariantList pumpRpmHistory() const { return m_pumpRpmHistory; }
    QVariantList chillerTempHistory() const { return m_chillerTempHistory; }
    QVariantList dryAirFlowHistory() const { return m_dryAirFlowHistory; }
    QVariantList timeHistory() const { return m_timeHistory; }

signals:
    void currentUserChanged();
    void vesselTempSetpointChanged();
    void pumpPressureSetpointChanged();
    void pumpRpmSetpointChanged();
    void chillerTempSetpointChanged();
    void dryAirFlowSetpointChanged();
    void sensorDataChanged();
    void runningChanged();
    void emergencyStoppedChanged();
    void runTimeChanged();
    void batchIdChanged();
    void recipeChanged();
    void pumpStateChanged();
    void loginFailed(const QString& reason);
    void runSaved(const QString& filepath);

private slots:
    void simulateSensorData();
    void updateRunTime();
    void finishFlushing();

private:
    void clearHistory();

    // Login
    QString m_currentUser;

    // Setpoints
    double m_vesselTempSetpoint = 25.0;
    double m_pumpPressureSetpoint = 101325.0;
    double m_pumpRpmSetpoint = 1500.0;
    double m_chillerTempSetpoint = 5.0;
    double m_dryAirFlowSetpoint = 1.2;

    // Live
    double m_vesselTemp = 0.0;
    double m_pumpPressure = 0.0;
    double m_pumpRpm = 0.0;
    double m_chillerTemp = 0.0;
    double m_dryAirFlow = 0.0;

    // Run
    bool m_running = false;
    bool m_emergencyStopped = false;
    QElapsedTimer m_runTimer;
    qint64 m_runElapsedMs = 0;

    // Timers
    QTimer m_sensorTimer;
    QTimer m_runClockTimer;
    QTimer m_flushTimer;

    // Batch
    QString m_batchId;
    QString m_recipe;

    // Pump
    bool m_pumpRunning = false;
    bool m_flushing = false;

    // History for charts
    QVariantList m_vesselTempHistory;
    QVariantList m_pumpPressureHistory;
    QVariantList m_pumpRpmHistory;
    QVariantList m_chillerTempHistory;
    QVariantList m_dryAirFlowHistory;
    QVariantList m_timeHistory;

    int m_sampleCount = 0;

    // For saving
    QJsonArray m_runLog;
};

#endif // BACKEND_H
