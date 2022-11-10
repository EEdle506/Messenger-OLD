#pragma once

#include "servernamespace.h"

class _CommandParser;
class _ConnectionData;
class _ConnectionHandler;

class _Server final
    : public QTcpServer
{
    Q_OBJECT

    // Parser which parse the incoming data from sokcet
    _CommandParser* m_Parser = nullptr;
    // Connections data
    // Stores info about rooms and sockets
    _ConnectionData* m_Data = nullptr;
    // TCP socket handler
    _ConnectionHandler* m_Handler = nullptr;

public:
    ~_Server() override;

    static _Server* instance();
    static void Start();

    _ConnectionData* connectionData();
    _ConnectionHandler* connectionHandler();

public slots:
    static void sendMessageToSocket(QTcpSocket* socket, const QByteArray& arr);

protected:
    explicit _Server(QObject* parent = nullptr);

private slots:
    void newSocketConnection();

public:
    _Server(_Server&) = delete;
    void operator=(const _Server&) = delete;

private:
    static _Server* p_instance;
};