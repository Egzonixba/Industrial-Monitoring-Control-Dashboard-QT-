#include "plantsimulator.h"
#include <QDebug>

PlantSimulator::PlantSimulator(const QString& host, int port, QObject* parent)
    : QObject(parent)
{
    QString clientId = "Simulator_" + QString::number(QRandomGenerator::global()->bounded(10000));
    m_mqtt = new SimpleMqtt(host, port, clientId, this);

    connect(m_mqtt, &SimpleMqtt::connected, this, &PlantSimulator::onConnected);
    connect(m_mqtt, &SimpleMqtt::messageReceived, this, &PlantSimulator::onMessageReceived);

    m_publishTimer.setInterval(1000);
    connect(&m_publishTimer, &QTimer::timeout, this, &PlantSimulator::publishSensorData);
}

void PlantSimulator::onConnected()
{
    qDebug() << "Simulator: Connected! Subscribing to commands...";
    m_mqtt->subscribe("plant/setpoints/#");
    m_mqtt->subscribe("plant/pump/command");
    m_mqtt->subscribe("plant/run/command");
    m_publishTimer.start();
}

void PlantSimulator::onMessageReceived(const QString& topic, const QString& payload)
{
    qDebug() << "Simulator received:" << topic << "=" << payload;

    if (topic == "plant/setpoints/vesselTemp") m_vesselTempSP = payload.toDouble();
    else if (topic == "plant/setpoints/pumpPressure") m_pumpPressureSP = payload.toDouble();
    else if (topic == "plant/setpoints/pumpRpm") m_pumpRpmSP = payload.toDouble();
    else if (topic == "plant/setpoints/chillerTemp") m_chillerTempSP = payload.toDouble();
    else if (topic == "plant/setpoints/co2Flow") m_co2FlowSP = payload.toDouble();
    else if (topic == "plant/pump/command") {
        if (payload == "ON" || payload == "FLUSH") {
            m_pumpRunning = true;
            m_mqtt->publish("plant/pump/status", "RUNNING");
        }
        else if (payload == "OFF") {
            m_pumpRunning = false;
            m_mqtt->publish("plant/pump/status", "OFF");
        }
    }
    else if (topic == "plant/run/command") {
        if (payload == "START") {
            m_runActive = true;
            m_mqtt->publish("plant/run/status", "RUNNING");
            qDebug() << "Simulator: RUN STARTED";
        }
        else if (payload == "STOP" || payload == "EMERGENCY") {
            m_runActive = false;
            m_pumpRunning = false;
            m_vesselTemp = 20.0; m_pumpPressure = 0; m_pumpRpm = 0;
            m_chillerTemp = 20.0; m_co2Flow = 0;
            m_mqtt->publish("plant/run/status", "STOPPED");
            m_mqtt->publish("plant/pump/status", "OFF");
            qDebug() << "Simulator: RUN STOPPED";
        }
    }
}

void PlantSimulator::publishSensorData()
{
    if (!m_runActive || !m_pumpRunning) return;

    auto rng = QRandomGenerator::global();
    double alpha = 0.05;

    m_vesselTemp += alpha * (m_vesselTempSP - m_vesselTemp) + (rng->bounded(1.0) - 0.5);
    m_pumpPressure += alpha * (m_pumpPressureSP - m_pumpPressure) + (rng->bounded(30.0) - 15.0);
    m_pumpRpm += alpha * (m_pumpRpmSP - m_pumpRpm) + (rng->bounded(50.0) - 25.0);
    m_chillerTemp += alpha * (m_chillerTempSP - m_chillerTemp) + (rng->bounded(0.5) - 0.25);
    m_co2Flow += alpha * (m_co2FlowSP - m_co2Flow) + (rng->bounded(60.0) - 30.0);

    m_vesselTemp = qMax(0.0, m_vesselTemp);
    m_pumpPressure = qMax(0.0, m_pumpPressure);
    m_pumpRpm = qMax(0.0, m_pumpRpm);
    m_co2Flow = qMax(0.0, m_co2Flow);

    m_mqtt->publish("plant/sensors/vesselTemp", QString::number(m_vesselTemp, 'f', 2));
    m_mqtt->publish("plant/sensors/pumpPressure", QString::number(m_pumpPressure, 'f', 2));
    m_mqtt->publish("plant/sensors/pumpRpm", QString::number(m_pumpRpm, 'f', 2));
    m_mqtt->publish("plant/sensors/chillerTemp", QString::number(m_chillerTemp, 'f', 2));
    m_mqtt->publish("plant/sensors/co2Flow", QString::number(m_co2Flow, 'f', 2));
}