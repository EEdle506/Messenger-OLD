#pragma once

#include <QObject>

#include <QTcpSocket>

#define MAX_PICTURE_SEND_SIZE 8192

class _Buffer;
class ConnectionHandler : public QObject
{
    Q_OBJECT

    QTcpSocket* m_Socket = nullptr;
    _Buffer* m_buffer = nullptr;

    bool m_canSend = false;

public:
    ~ConnectionHandler() override;

    static ConnectionHandler* instance();

public slots:
    static bool sendMessage(const QByteArray& msg, QWidget* widget = nullptr);

private:
    explicit ConnectionHandler(QObject* parent = nullptr);
    static ConnectionHandler* m_instance;

private slots:
    void readyRead();
    void tryReconnect();

    void socketStateChanged(QTcpSocket::SocketState state);

signals:
    void dataRead(const QByteArray& data);

    void socketConnected(const QString& info);
    void socketDisconnected(const QString& info);
};
