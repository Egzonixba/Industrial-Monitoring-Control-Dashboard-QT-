#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QDebug>
#include "backend.h"
#include "mqttmanager.h"

using namespace Qt::StringLiterals;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQuickStyle::setStyle("Material");

    QQmlApplicationEngine engine;

    DashboardBackend backend;
    MqttManager mqtt;

    // MQTT sensor signals → backend live setters
    QObject::connect(&mqtt, &MqttManager::vesselTempReceived, [&](double v) {
        backend.setLiveVesselTemp(v);
    });
    QObject::connect(&mqtt, &MqttManager::pumpPressureReceived, [&](double v) {
        backend.setLivePumpPressure(v);
    });
    QObject::connect(&mqtt, &MqttManager::pumpRpmReceived, [&](double v) {
        backend.setLivePumpRpm(v);
    });
    QObject::connect(&mqtt, &MqttManager::chillerTempReceived, [&](double v) {
        backend.setLiveChillerTemp(v);
    });
    QObject::connect(&mqtt, &MqttManager::co2FlowReceived, [&](double v) {
        backend.setLiveCo2Flow(v);
    });

    // Setpoint changes → publish via MQTT
    QObject::connect(&backend, &DashboardBackend::vesselTempSetpointChanged, [&]() {
        mqtt.publishSetpoint("vesselTemp", backend.vesselTempSetpoint());
    });
    QObject::connect(&backend, &DashboardBackend::pumpPressureSetpointChanged, [&]() {
        mqtt.publishSetpoint("pumpPressure", backend.pumpPressureSetpoint());
    });
    QObject::connect(&backend, &DashboardBackend::pumpRpmSetpointChanged, [&]() {
        mqtt.publishSetpoint("pumpRpm", backend.pumpRpmSetpoint());
    });
    QObject::connect(&backend, &DashboardBackend::chillerTempSetpointChanged, [&]() {
        mqtt.publishSetpoint("chillerTemp", backend.chillerTempSetpoint());
    });
    QObject::connect(&backend, &DashboardBackend::co2FlowSetpointChanged, [&]() {
        mqtt.publishSetpoint("co2Flow", backend.co2FlowSetpoint());
    });

    // Pump and run commands → publish via MQTT
    QObject::connect(&backend, &DashboardBackend::requestPumpCommand, [&](const QString &cmd) {
        mqtt.publishPumpCommand(cmd);
    });
    QObject::connect(&backend, &DashboardBackend::requestRunCommand, [&](const QString &cmd) {
        mqtt.publishRunCommand(cmd);
    });

    // Expose to QML
    engine.rootContext()->setContextProperty("backend", &backend);
    engine.rootContext()->setContextProperty("mqtt", &mqtt);

    engine.loadFromModule("MQTTDashboard", "Main");

    if (engine.rootObjects().isEmpty()) {
        qDebug() << "Failed to load QML";
        return -1;
    }

    qDebug() << "Dashboard started successfully";
    return app.exec();
}