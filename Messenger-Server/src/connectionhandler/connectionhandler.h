#pragma once

#include "server/servernamespace.h"

class _ConnectionHandler
    : public QObject
{
    Q_OBJECT

    // Connections in queue
    QHash <QTcpSocket*, _Buffer*> m_socketQueue;
    // Incoming socket
    QTcpSocket* m_currentSocket = nullptr;

public:
    _ConnectionHandler(QObject *parent = nullptr);
    ~_ConnectionHandler();

    QTcpSocket* currentSocket();

    void pushSocketToQueue(QTcpSocket* socket);

private slots:
    // on incoming data from socket, reads this data
    void readyRead();
    //
    void disconnected();

signals:
    //
    void dataRead(const QByteArray& data);
    //
    void socketDisconnected(QTcpSocket* socket);
};
