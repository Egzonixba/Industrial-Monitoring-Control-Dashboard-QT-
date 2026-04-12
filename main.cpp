#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QDebug>
#include "backend.h"

using namespace Qt::StringLiterals;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQuickStyle::setStyle("Material");

    QQmlApplicationEngine engine;

    // Create and register backend
    DashboardBackend backend;
    engine.rootContext()->setContextProperty("backend", &backend);

    // Load QML from module
    engine.loadFromModule("MQTTDashboard", "Main");

    if (engine.rootObjects().isEmpty()) {
        qDebug() << "Failed to load QML";
        return -1;
    }

    qDebug() << "Application started successfully";

    return app.exec();
}