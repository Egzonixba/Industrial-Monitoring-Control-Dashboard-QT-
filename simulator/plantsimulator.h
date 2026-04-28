#ifndef PLANTSIMULATOR_H
#define PLANTSIMULATOR_H

#include <QObject>
#include <QTimer>
#include <QRandomGenerator>
#include <QtMath>
#include "simplemqtt.h"

class PlantSimulator : public QObject
{
    Q_OBJECT

public:
    explicit PlantSimulator(const QString& host = "localhost",
        int port = 1883,
        QObject* parent = nullptr);

private slots:
    void onConnected();
    void onMessageReceived(const QString& topic, const QString& payload);
    void publishSensorData();

private:
    SimpleMqtt* m_mqtt;
    QTimer m_publishTimer;

    double m_vesselTempSP = 35.0;
    double m_pumpPressureSP = 750.0;
    double m_pumpRpmSP = 1500.0;
    double m_chillerTempSP = 5.0;
    double m_co2FlowSP = 4500.0;

    double m_vesselTemp = 20.0;
    double m_pumpPressure = 0.0;
    double m_pumpRpm = 0.0;
    double m_chillerTemp = 20.0;
    double m_co2Flow = 0.0;

    bool m_pumpRunning = false;
    bool m_runActive = false;
};

#endif