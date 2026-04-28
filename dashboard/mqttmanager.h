#ifndef MQTTMANAGER_H
#define MQTTMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QRandomGenerator>
#include <QByteArray>

class MqttManager : public QObject
{
    Q_OBJECT

        Q_PROPERTY(bool connected READ connected NOTIFY connectionChanged)
        Q_PROPERTY(QString brokerHost READ brokerHost WRITE setBrokerHost NOTIFY brokerHostChanged)
        Q_PROPERTY(int brokerPort READ brokerPort WRITE setBrokerPort NOTIFY brokerPortChanged)
        Q_PROPERTY(QString connectionStatus READ connectionStatus NOTIFY connectionChanged)

public:
    explicit MqttManager(QObject* parent = nullptr);
    ~MqttManager();

    bool connected() const { return m_connected; }
    QString brokerHost() const { return m_brokerHost; }
    int brokerPort() const { return m_brokerPort; }
    QString connectionStatus() const;

    void setBrokerHost(const QString& host);
    void setBrokerPort(int port);

    Q_INVOKABLE void connectToBroker();
    Q_INVOKABLE void disconnectFromBroker();
    Q_INVOKABLE void publishSetpoint(const QString& param, double value);
    Q_INVOKABLE void publishPumpCommand(const QString& command);
    Q_INVOKABLE void publishRunCommand(const QString& command);

signals:
    void connectionChanged();
    void brokerHostChanged();
    void brokerPortChanged();
    void vesselTempReceived(double value);
    void pumpPressureReceived(double value);
    void pumpRpmReceived(double value);
    void chillerTempReceived(double value);
    void co2FlowReceived(double value);
    void pumpStatusReceived(const QString& status);
    void errorOccurred(const QString& message);

private slots:
    void onSocketConnected();
    void onSocketDisconnected();
    void onSocketReadyRead();
    void onSocketError(QAbstractSocket::SocketError error);
    void sendPingReq();

private:
    QByteArray buildConnectPacket(const QString& clientId);
    QByteArray buildSubscribePacket(quint16 packetId, const QString& topicFilter);
    QByteArray buildPublishPacket(const QString& topic, const QByteArray& payload, bool retain = false);
    QByteArray buildPingReqPacket();
    QByteArray buildDisconnectPacket();

    void parsePublish(const QByteArray& data, quint8 flags);
    QByteArray encodeRemainingLength(int length);
    int decodeRemainingLength(const QByteArray& data, int startPos, int& bytesUsed);
    QByteArray encodeString(const QString& str);

    void subscribe(const QString& topicFilter);
    void publish(const QString& topic, const QString& payload, bool retain = false);

    QTcpSocket* m_socket;
    QTimer m_pingTimer;
    QString m_brokerHost = "localhost";
    int m_brokerPort = 1883;
    bool m_connected = false;
    quint16 m_nextPacketId = 1;
    QByteArray m_readBuffer;
};

#endif