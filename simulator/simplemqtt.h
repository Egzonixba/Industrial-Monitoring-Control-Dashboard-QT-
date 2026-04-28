#ifndef SIMPLEMQTT_H
#define SIMPLEMQTT_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QRandomGenerator>

class SimpleMqtt : public QObject
{
    Q_OBJECT

public:
    explicit SimpleMqtt(const QString& host, int port,
        const QString& clientId, QObject* parent = nullptr);

    bool isConnected() const { return m_connected; }
    void subscribe(const QString& topicFilter);
    void publish(const QString& topic, const QString& payload, bool retain = false);

signals:
    void connected();
    void disconnected();
    void messageReceived(const QString& topic, const QString& payload);

private slots:
    void onSocketConnected();
    void onSocketDisconnected();
    void onSocketReadyRead();
    void onSocketError(QAbstractSocket::SocketError error);
    void sendPing();

private:
    QByteArray encodeString(const QString& str);
    QByteArray encodeRemainingLength(int length);
    int decodeRemainingLength(const QByteArray& data, int startPos, int& bytesUsed);

    QTcpSocket* m_socket;
    QTimer m_pingTimer;
    QString m_clientId;
    bool m_connected = false;
    quint16 m_packetId = 1;
    QByteArray m_readBuffer;
};

#endif // SIMPLEMQTT_H