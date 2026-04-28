#include <QCoreApplication>
#include <QDebug>
#include <QCommandLineParser>
#include "plantsimulator.h"

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);
    app.setApplicationName("PlantSimulator");

    QCommandLineParser parser;
    parser.addHelpOption();
    QCommandLineOption hostOpt("host", "Broker host", "host", "localhost");
    QCommandLineOption portOpt("port", "Broker port", "port", "1883");
    parser.addOption(hostOpt);
    parser.addOption(portOpt);
    parser.process(app);

    qDebug() << "========================================";
    qDebug() << " MQTT Plant Simulator";
    qDebug() << " Broker:" << parser.value(hostOpt) << ":" << parser.value(portOpt);
    qDebug() << "========================================";

    PlantSimulator sim(parser.value(hostOpt), parser.value(portOpt).toInt());

    return app.exec();
}