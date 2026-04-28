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

        Q_PROPERTY(QString currentUser READ currentUser NOTIFY currentUserChanged)
        Q_PROPERTY(bool useMqtt READ useMqtt WRITE setUseMqtt NOTIFY useMqttChanged)

        Q_PROPERTY(double vesselTempSetpoint READ vesselTempSetpoint WRITE setVesselTempSetpoint NOTIFY vesselTempSetpointChanged)
        Q_PROPERTY(double pumpPressureSetpoint READ pumpPressureSetpoint WRITE setPumpPressureSetpoint NOTIFY pumpPressureSetpointChanged)
        Q_PROPERTY(double pumpRpmSetpoint READ pumpRpmSetpoint WRITE setPumpRpmSetpoint NOTIFY pumpRpmSetpointChanged)
        Q_PROPERTY(double chillerTempSetpoint READ chillerTempSetpoint WRITE setChillerTempSetpoint NOTIFY chillerTempSetpointChanged)
        Q_PROPERTY(double co2FlowSetpoint READ co2FlowSetpoint WRITE setCo2FlowSetpoint NOTIFY co2FlowSetpointChanged)

        Q_PROPERTY(double vesselTemp READ vesselTemp NOTIFY sensorDataChanged)
        Q_PROPERTY(double pumpPressure READ pumpPressure NOTIFY sensorDataChanged)
        Q_PROPERTY(double pumpRpm READ pumpRpm NOTIFY sensorDataChanged)
        Q_PROPERTY(double chillerTemp READ chillerTemp NOTIFY sensorDataChanged)
        Q_PROPERTY(double co2Flow READ co2Flow NOTIFY sensorDataChanged)

        Q_PROPERTY(int vesselTempAlarm READ vesselTempAlarm NOTIFY alarmStateChanged)
        Q_PROPERTY(int pumpPressureAlarm READ pumpPressureAlarm NOTIFY alarmStateChanged)
        Q_PROPERTY(int pumpRpmAlarm READ pumpRpmAlarm NOTIFY alarmStateChanged)
        Q_PROPERTY(int chillerTempAlarm READ chillerTempAlarm NOTIFY alarmStateChanged)
        Q_PROPERTY(int co2FlowAlarm READ co2FlowAlarm NOTIFY alarmStateChanged)

        Q_PROPERTY(bool running READ running NOTIFY runningChanged)
        Q_PROPERTY(bool emergencyStopped READ emergencyStopped NOTIFY emergencyStoppedChanged)
        Q_PROPERTY(QString runTimeString READ runTimeString NOTIFY runTimeChanged)
        Q_PROPERTY(int runElapsedSeconds READ runElapsedSeconds NOTIFY runTimeChanged)

        Q_PROPERTY(QString batchId READ batchId WRITE setBatchId NOTIFY batchIdChanged)
        Q_PROPERTY(QString recipe READ recipe WRITE setRecipe NOTIFY recipeChanged)
        Q_PROPERTY(QStringList recipeList READ recipeList CONSTANT)

        Q_PROPERTY(bool pumpRunning READ pumpRunning NOTIFY pumpStateChanged)
        Q_PROPERTY(bool flushing READ flushing NOTIFY pumpStateChanged)
        Q_PROPERTY(int flushCountdown READ flushCountdown NOTIFY flushCountdownChanged)

        Q_PROPERTY(QVariantList vesselTempHistory READ vesselTempHistory NOTIFY sensorDataChanged)
        Q_PROPERTY(QVariantList pumpPressureHistory READ pumpPressureHistory NOTIFY sensorDataChanged)
        Q_PROPERTY(QVariantList pumpRpmHistory READ pumpRpmHistory NOTIFY sensorDataChanged)
        Q_PROPERTY(QVariantList chillerTempHistory READ chillerTempHistory NOTIFY sensorDataChanged)
        Q_PROPERTY(QVariantList co2FlowHistory READ co2FlowHistory NOTIFY sensorDataChanged)
        Q_PROPERTY(QVariantList timeHistory READ timeHistory NOTIFY sensorDataChanged)

public:
    explicit DashboardBackend(QObject* parent = nullptr);

    enum AlarmState { LowAlarm = -1, Normal = 0, HighAlarm = 1 };

    // Login
    QString currentUser() const { return m_currentUser; }
    Q_INVOKABLE bool login(const QString& username, const QString& password);
    Q_INVOKABLE void logout();

    // MQTT mode
    bool useMqtt() const { return m_useMqtt; }
    void setUseMqtt(bool v);

    // Setpoints
    double vesselTempSetpoint() const { return m_vesselTempSetpoint; }
    double pumpPressureSetpoint() const { return m_pumpPressureSetpoint; }
    double pumpRpmSetpoint() const { return m_pumpRpmSetpoint; }
    double chillerTempSetpoint() const { return m_chillerTempSetpoint; }
    double co2FlowSetpoint() const { return m_co2FlowSetpoint; }

    void setVesselTempSetpoint(double v);
    void setPumpPressureSetpoint(double v);
    void setPumpRpmSetpoint(double v);
    void setChillerTempSetpoint(double v);
    void setCo2FlowSetpoint(double v);

    // Live values
    double vesselTemp() const { return m_vesselTemp; }
    double pumpPressure() const { return m_pumpPressure; }
    double pumpRpm() const { return m_pumpRpm; }
    double chillerTemp() const { return m_chillerTemp; }
    double co2Flow() const { return m_co2Flow; }

    // MQTT live setters
    void setLiveVesselTemp(double v);
    void setLivePumpPressure(double v);
    void setLivePumpRpm(double v);
    void setLiveChillerTemp(double v);
    void setLiveCo2Flow(double v);

    // Alarms
    int vesselTempAlarm() const { return m_vesselTempAlarm; }
    int pumpPressureAlarm() const { return m_pumpPressureAlarm; }
    int pumpRpmAlarm() const { return m_pumpRpmAlarm; }
    int chillerTempAlarm() const { return m_chillerTempAlarm; }
    int co2FlowAlarm() const { return m_co2FlowAlarm; }

    // Run
    bool running() const { return m_running; }
    bool emergencyStopped() const { return m_emergencyStopped; }
    QString runTimeString() const;
    int runElapsedSeconds() const;

    Q_INVOKABLE void startRun();
    Q_INVOKABLE void finishRun();
    Q_INVOKABLE void emergencyStop();
    Q_INVOKABLE bool saveRun();

    // Batch / Recipe
    QString batchId() const { return m_batchId; }
    void setBatchId(const QString& id);
    QString recipe() const { return m_recipe; }
    void setRecipe(const QString& r);
    QStringList recipeList() const;
    Q_INVOKABLE void applyRecipe(const QString& recipeName);

    // Pump
    bool pumpRunning() const { return m_pumpRunning; }
    bool flushing() const { return m_flushing; }
    int flushCountdown() const { return m_flushCountdown; }
    Q_INVOKABLE void startPumpSequence();

    // Chart data
    QVariantList vesselTempHistory() const { return m_vesselTempHistory; }
    QVariantList pumpPressureHistory() const { return m_pumpPressureHistory; }
    QVariantList pumpRpmHistory() const { return m_pumpRpmHistory; }
    QVariantList chillerTempHistory() const { return m_chillerTempHistory; }
    QVariantList co2FlowHistory() const { return m_co2FlowHistory; }
    QVariantList timeHistory() const { return m_timeHistory; }

signals:
    void requestPumpCommand(const QString &command);
    void requestRunCommand(const QString &command);
    void currentUserChanged();
    void useMqttChanged();
    void vesselTempSetpointChanged();
    void pumpPressureSetpointChanged();
    void pumpRpmSetpointChanged();
    void chillerTempSetpointChanged();
    void co2FlowSetpointChanged();
    void sensorDataChanged();
    void alarmStateChanged();
    void runningChanged();
    void emergencyStoppedChanged();
    void runTimeChanged();
    void batchIdChanged();
    void recipeChanged();
    void pumpStateChanged();
    void flushCountdownChanged();
    void loginFailed(const QString& reason);
    void runSaved(const QString& filepath);
    void pumpSequenceComplete();

private slots:
    void simulateSensorData();
    void updateRunTime();
    void flushTick();
    void onPumpSequenceReady();

private:
    void clearHistory();
    void checkAlarms();
    void appendToHistory();

    QString m_currentUser;
    bool m_useMqtt = false;

    double m_vesselTempSetpoint = 25.0;
    double m_pumpPressureSetpoint = 750.0;
    double m_pumpRpmSetpoint = 1500.0;
    double m_chillerTempSetpoint = 5.0;
    double m_co2FlowSetpoint = 4500.0;

    double m_vesselTemp = 0.0;
    double m_pumpPressure = 0.0;
    double m_pumpRpm = 0.0;
    double m_chillerTemp = 0.0;
    double m_co2Flow = 0.0;

    int m_vesselTempAlarm = 0;
    int m_pumpPressureAlarm = 0;
    int m_pumpRpmAlarm = 0;
    int m_chillerTempAlarm = 0;
    int m_co2FlowAlarm = 0;

    int m_vesselTempBreachTime = 0;
    int m_pumpPressureBreachTime = 0;
    int m_pumpRpmBreachTime = 0;
    int m_chillerTempBreachTime = 0;
    int m_co2FlowBreachTime = 0;

    static constexpr int ALARM_DELAY_SECONDS = 15;

    bool m_running = false;
    bool m_emergencyStopped = false;
    QElapsedTimer m_runTimer;
    qint64 m_runElapsedMs = 0;

    QTimer m_sensorTimer;
    QTimer m_runClockTimer;
    QTimer m_flushTimer;
    QTimer m_postFlushTimer;

    QString m_batchId;
    QString m_recipe;

    bool m_pumpRunning = false;
    bool m_flushing = false;
    int m_flushCountdown = 0;

    QVariantList m_vesselTempHistory;
    QVariantList m_pumpPressureHistory;
    QVariantList m_pumpRpmHistory;
    QVariantList m_chillerTempHistory;
    QVariantList m_co2FlowHistory;
    QVariantList m_timeHistory;

    int m_sampleCount = 0;
    int m_lastHistorySecond = -1;
    QJsonArray m_runLog;
};

#endif