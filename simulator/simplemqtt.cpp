#include "simplemqtt.h"
#include <QDebug>

SimpleMqtt::SimpleMqtt(const QString& host, int port,
    const QString& clientId, QObject* parent)
    : QObject(parent)
    , m_socket(new QTcpSocket(this))
    , m_clientId(clientId)
{
    connect(m_socket, &QTcpSocket::connected, this, &SimpleMqtt::onSocketConnected);
    connect(m_socket, &QTcpSocket::disconnected, this, &SimpleMqtt::onSocketDisconnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &SimpleMqtt::onSocketReadyRead);
    connect(m_socket, &QTcpSocket::errorOccurred, this, &SimpleMqtt::onSocketError);

    m_pingTimer.setInterval(30000);
    connect(&m_pingTimer, &QTimer::timeout, this, &SimpleMqtt::sendPing);

    m_socket->connectToHost(host, port);
}

void SimpleMqtt::onSocketConnected()
{
    // Build CONNECT packet
    QByteArray varHeader;
    varHeader.append(encodeString("MQTT"));
    varHeader.append((char)4);
    varHeader.append((char)0x02);
    varHeader.append((char)0);
    varHeader.append((char)60);

    QByteArray payload = encodeString(m_clientId);

    QByteArray packet;
    packet.append((char)0x10);
    packet.append(encodeRemainingLength(varHeader.size() + payload.size()));
    packet.append(varHeader);
    packet.append(payload);

    m_socket->write(packet);
}

void SimpleMqtt::onSocketDisconnected()
{
    m_connected = false;
    m_pingTimer.stop();
    emit disconnected();
}

void SimpleMqtt::onSocketError(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error)
        qWarning() << "MQTT Error:" << m_socket->errorString();
}

void SimpleMqtt::onSocketReadyRead()
{
    m_readBuffer.append(m_socket->readAll());

    while (m_readBuffer.size() >= 2) {
        quint8 header = (quint8)m_readBuffer[0];
        quint8 packetType = (header >> 4) & 0x0F;
        quint8 flags = header & 0x0F;

        int bytesUsed = 0;
        int remLen = decodeRemainingLength(m_readBuffer, 1, bytesUsed);
        if (remLen < 0) return;

        int total = 1 + bytesUsed + remLen;
        if (m_readBuffer.size() < total) return;

        QByteArray data = m_readBuffer.mid(1 + bytesUsed, remLen);
        m_readBuffer.remove(0, total);

        switch (packetType) {
        case 2: // CONNACK
            if (data.size() >= 2 && data[1] == 0) {
                m_connected = true;
                m_pingTimer.start();
                qDebug() << "MQTT: Connected!";
                emit connected();
            }
            break;

        case 3: { // PUBLISH
            Q_UNUSED(flags)
                if (data.size() < 4) break;
            quint16 topicLen = ((quint8)data[0] << 8) | (quint8)data[1];
            if (data.size() < 2 + topicLen) break;
            QString topic = QString::fromUtf8(data.mid(2, topicLen));
            QString payload = QString::fromUtf8(data.mid(2 + topicLen)).trimmed();
            emit messageReceived(topic, payload);
            break;
        }

        case 9: // SUBACK
            break;
        case 13: // PINGRESP
            break;
        }
    }
}

void SimpleMqtt::subscribe(const QString& topicFilter)
{
    if (!m_connected) return;

    QByteArray varHeader;
    varHeader.append((char)(m_packetId >> 8));
    varHeader.append((char)(m_packetId & 0xFF));
    m_packetId++;

    QByteArray payload;
    payload.append(encodeString(topicFilter));
    payload.append((char)0);

    QByteArray packet;
    packet.append((char)0x82);
    packet.append(encodeRemainingLength(varHeader.size() + payload.size()));
    packet.append(varHeader);
    packet.append(payload);

    m_socket->write(packet);
}

void SimpleMqtt::publish(const QString& topic, const QString& payload, bool retain)
{
    if (!m_connected) return;

    QByteArray topicBytes = encodeString(topic);
    QByteArray payloadBytes = payload.toUtf8();

    quint8 header = 0x30;
    if (retain) header |= 0x01;

    QByteArray packet;
    packet.append((char)header);
    packet.append(encodeRemainingLength(topicBytes.size() + payloadBytes.size()));
    packet.append(topicBytes);
    packet.append(payloadBytes);

    m_socket->write(packet);
}

void SimpleMqtt::sendPing()
{
    if (!m_connected) return;
    QByteArray packet;
    packet.append((char)0xC0);
    packet.append((char)0x00);
    m_socket->write(packet);
}

QByteArray SimpleMqtt::encodeString(const QString& str) {
    QByteArray utf8 = str.toUtf8();
    QByteArray result;
    result.append((char)(utf8.size() >> 8));
    result.append((char)(utf8.size() & 0xFF));
    result.append(utf8);
    return result;
}

QByteArray SimpleMqtt::encodeRemainingLength(int length) {
    QByteArray result;
    do {
        quint8 b = length % 128;
        length /= 128;
        if (length > 0) b |= 0x80;
        result.append((char)b);
    } while (length > 0);
    return result;
}

int SimpleMqtt::decodeRemainingLength(const QByteArray& data, int startPos, int& bytesUsed) {
    int multiplier = 1, value = 0;
    bytesUsed = 0;
    for (int i = startPos; i < data.size() && i < startPos + 4; i++) {
        bytesUsed++;
        quint8 b = (quint8)data[i];
        value += (b & 0x7F) * multiplier;
        if ((b & 0x80) == 0) return value;
        multiplier *= 128;
    }
    return -1;
}