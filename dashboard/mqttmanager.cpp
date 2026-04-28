#include "mqttmanager.h"
#include <QDebug>

MqttManager::MqttManager(QObject* parent)
    : QObject(parent)
    , m_socket(new QTcpSocket(this))
{
    connect(m_socket, &QTcpSocket::connected, this, &MqttManager::onSocketConnected);
    connect(m_socket, &QTcpSocket::disconnected, this, &MqttManager::onSocketDisconnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &MqttManager::onSocketReadyRead);
    connect(m_socket, &QTcpSocket::errorOccurred, this, &MqttManager::onSocketError);

    m_pingTimer.setInterval(30000);
    connect(&m_pingTimer, &QTimer::timeout, this, &MqttManager::sendPingReq);
}

MqttManager::~MqttManager()
{
    if (m_connected) disconnectFromBroker();
}

QString MqttManager::connectionStatus() const
{
    if (m_connected) return "Connected";
    if (m_socket->state() == QAbstractSocket::ConnectingState) return "Connecting...";
    return "Disconnected";
}

void MqttManager::setBrokerHost(const QString& host) {
    if (m_brokerHost == host) return;
    m_brokerHost = host;
    emit brokerHostChanged();
}

void MqttManager::setBrokerPort(int port) {
    if (m_brokerPort == port) return;
    m_brokerPort = port;
    emit brokerPortChanged();
}

void MqttManager::connectToBroker()
{
    if (m_connected || m_socket->state() != QAbstractSocket::UnconnectedState) return;
    qDebug() << "MQTT: Connecting to" << m_brokerHost << ":" << m_brokerPort;
    m_readBuffer.clear();
    m_socket->connectToHost(m_brokerHost, m_brokerPort);
}

void MqttManager::disconnectFromBroker()
{
    if (m_connected) {
        m_socket->write(buildDisconnectPacket());
        m_socket->flush();
    }
    m_pingTimer.stop();
    m_socket->disconnectFromHost();
    m_connected = false;
    emit connectionChanged();
}

void MqttManager::onSocketConnected()
{
    qDebug() << "MQTT: TCP connected, sending CONNECT packet";
    QString clientId = "Dashboard_" + QString::number(QRandomGenerator::global()->bounded(10000));
    m_socket->write(buildConnectPacket(clientId));
}

void MqttManager::onSocketDisconnected()
{
    qDebug() << "MQTT: Disconnected";
    m_connected = false;
    m_pingTimer.stop();
    emit connectionChanged();
}

void MqttManager::onSocketError(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error)
        QString msg = m_socket->errorString();
    qWarning() << "MQTT Error:" << msg;
    m_connected = false;
    m_pingTimer.stop();
    emit connectionChanged();
    emit errorOccurred(msg);
}

void MqttManager::onSocketReadyRead()
{
    m_readBuffer.append(m_socket->readAll());

    while (m_readBuffer.size() >= 2) {
        quint8 header = (quint8)m_readBuffer[0];
        quint8 packetType = (header >> 4) & 0x0F;
        quint8 flags = header & 0x0F;

        int bytesUsed = 0;
        int remainingLength = decodeRemainingLength(m_readBuffer, 1, bytesUsed);
        if (remainingLength < 0) return;

        int totalPacketSize = 1 + bytesUsed + remainingLength;
        if (m_readBuffer.size() < totalPacketSize) return;

        QByteArray packetData = m_readBuffer.mid(1 + bytesUsed, remainingLength);
        m_readBuffer.remove(0, totalPacketSize);

        switch (packetType) {
        case 2: // CONNACK
            if (packetData.size() >= 2 && packetData[1] == 0) {
                qDebug() << "MQTT: Connected to broker!";
                m_connected = true;
                m_pingTimer.start();
                emit connectionChanged();
                subscribe("plant/sensors/#");
                subscribe("plant/pump/status");
                subscribe("plant/run/status");
            }
            else {
                qWarning() << "MQTT: Connection refused";
                m_socket->disconnectFromHost();
            }
            break;
        case 3: // PUBLISH
            parsePublish(packetData, flags);
            break;
        case 9: // SUBACK
            qDebug() << "MQTT: Subscription confirmed";
            break;
        case 13: // PINGRESP
            break;
        }
    }
}

void MqttManager::parsePublish(const QByteArray& data, quint8 flags)
{
    Q_UNUSED(flags)
        if (data.size() < 4) return;

    quint16 topicLen = ((quint8)data[0] << 8) | (quint8)data[1];
    if (data.size() < 2 + topicLen) return;

    QString topic = QString::fromUtf8(data.mid(2, topicLen));
    QString payload = QString::fromUtf8(data.mid(2 + topicLen)).trimmed();

    if (topic == "plant/sensors/vesselTemp") {
        bool ok; double v = payload.toDouble(&ok); if (ok) emit vesselTempReceived(v);
    }
    else if (topic == "plant/sensors/pumpPressure") {
        bool ok; double v = payload.toDouble(&ok); if (ok) emit pumpPressureReceived(v);
    }
    else if (topic == "plant/sensors/pumpRpm") {
        bool ok; double v = payload.toDouble(&ok); if (ok) emit pumpRpmReceived(v);
    }
    else if (topic == "plant/sensors/chillerTemp") {
        bool ok; double v = payload.toDouble(&ok); if (ok) emit chillerTempReceived(v);
    }
    else if (topic == "plant/sensors/co2Flow") {
        bool ok; double v = payload.toDouble(&ok); if (ok) emit co2FlowReceived(v);
    }
    else if (topic == "plant/pump/status") {
        emit pumpStatusReceived(payload);
    }
}

void MqttManager::subscribe(const QString& topicFilter)
{
    if (!m_connected) return;
    m_socket->write(buildSubscribePacket(m_nextPacketId++, topicFilter));
    qDebug() << "MQTT: Subscribed to" << topicFilter;
}

void MqttManager::publish(const QString& topic, const QString& payload, bool retain)
{
    if (!m_connected) return;
    m_socket->write(buildPublishPacket(topic, payload.toUtf8(), retain));
}

void MqttManager::publishSetpoint(const QString& param, double value)
{
    publish("plant/setpoints/" + param, QString::number(value, 'f', 2), true);
}

void MqttManager::publishPumpCommand(const QString& command)
{
    publish("plant/pump/command", command);
}

void MqttManager::publishRunCommand(const QString& command)
{
    publish("plant/run/command", command);
}

void MqttManager::sendPingReq()
{
    if (!m_connected) return;
    m_socket->write(buildPingReqPacket());
}

// ─── Packet Builders ─────────────────────────────────────

QByteArray MqttManager::encodeString(const QString& str) {
    QByteArray utf8 = str.toUtf8();
    QByteArray r;
    r.append((char)(utf8.size() >> 8));
    r.append((char)(utf8.size() & 0xFF));
    r.append(utf8);
    return r;
}

QByteArray MqttManager::encodeRemainingLength(int length) {
    QByteArray r;
    do {
        quint8 b = length % 128;
        length /= 128;
        if (length > 0) b |= 0x80;
        r.append((char)b);
    } while (length > 0);
    return r;
}

int MqttManager::decodeRemainingLength(const QByteArray& data, int startPos, int& bytesUsed) {
    int mult = 1, val = 0;
    bytesUsed = 0;
    for (int i = startPos; i < data.size() && i < startPos + 4; i++) {
        bytesUsed++;
        quint8 b = (quint8)data[i];
        val += (b & 0x7F) * mult;
        if ((b & 0x80) == 0) return val;
        mult *= 128;
    }
    return -1;
}

QByteArray MqttManager::buildConnectPacket(const QString& clientId) {
    QByteArray vh;
    vh.append(encodeString("MQTT"));
    vh.append((char)4);
    vh.append((char)0x02);
    vh.append((char)0);
    vh.append((char)60);
    QByteArray pl = encodeString(clientId);
    QByteArray pkt;
    pkt.append((char)0x10);
    pkt.append(encodeRemainingLength(vh.size() + pl.size()));
    pkt.append(vh);
    pkt.append(pl);
    return pkt;
}

QByteArray MqttManager::buildSubscribePacket(quint16 packetId, const QString& topicFilter) {
    QByteArray vh;
    vh.append((char)(packetId >> 8));
    vh.append((char)(packetId & 0xFF));
    QByteArray pl;
    pl.append(encodeString(topicFilter));
    pl.append((char)0);
    QByteArray pkt;
    pkt.append((char)0x82);
    pkt.append(encodeRemainingLength(vh.size() + pl.size()));
    pkt.append(vh);
    pkt.append(pl);
    return pkt;
}

QByteArray MqttManager::buildPublishPacket(const QString& topic, const QByteArray& payload, bool retain) {
    QByteArray tb = encodeString(topic);
    quint8 h = 0x30;
    if (retain) h |= 0x01;
    QByteArray pkt;
    pkt.append((char)h);
    pkt.append(encodeRemainingLength(tb.size() + payload.size()));
    pkt.append(tb);
    pkt.append(payload);
    return pkt;
}

QByteArray MqttManager::buildPingReqPacket() {
    QByteArray p;
    p.append((char)0xC0);
    p.append((char)0x00);
    return p;
}

QByteArray MqttManager::buildDisconnectPacket() {
    QByteArray p;
    p.append((char)0xE0);
    p.append((char)0x00);
    return p;
}