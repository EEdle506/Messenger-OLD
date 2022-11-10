#include "server.h"

#include "parser/commandparser.h"

#include "connectiondata/connectiondata.h"
#include "connectionhandler/connectionhandler.h"

_Server::_Server(QObject* parent)
    : QTcpServer(parent) 
{
    m_Data = new _ConnectionData();
    m_Parser = new _CommandParser();
    m_Handler = new _ConnectionHandler();
}
_Server::~_Server() {}

void _Server::newSocketConnection()
{
    while (hasPendingConnections()) {
        auto socket = static_cast<QTcpSocket*>(nextPendingConnection());
        if (!socket) return;
        m_Handler->pushSocketToQueue(socket);
    }
}

_Server* _Server::instance()
{
    if (!p_instance) p_instance = new _Server();
    return p_instance;
}

void _Server::Start()
{
    using namespace Server;

    qDebug() << "[INFO] Server is starting...";

    DataBase::Connect();
    _Server* serv = _Server::instance();

    // connecting account and room actions
    connect(DataBase::instance(), &DataBase::accountAction,
        serv->connectionData(), &_ConnectionData::onAccountAction);
    connect(DataBase::instance(), &DataBase::roomAction,
        serv->connectionData(), &_ConnectionData::onRoomAction);

    // loading rooms
    DataBase::loadRoomsFromDB(Server::DBField::Group);
    DataBase::loadRoomsFromDB(Server::DBField::Chat);

    // On send message to socket from connection data
    connect(serv->connectionData(), & _ConnectionData::messageSend,
        serv, &_Server::sendMessageToSocket);

    // On socket disconnected
    connect(serv->connectionHandler(), &_ConnectionHandler::socketDisconnected,
        serv->connectionData(), &_ConnectionData::onSocketDisconnected);

    // Parsing and executing commands
    connect(serv->connectionHandler(), &_ConnectionHandler::dataRead,
        serv->m_Parser, &_CommandParser::inputCommand);
    connect(serv->m_Parser, &_CommandParser::dataParsed, [](_BaseCommand* command) {
        try {
            command->execute();
        }
        catch (const std::exception& e) {
            qCritical() << "[ERROR]" << e.what();
        }
    });

    // When new socket is availiable
    connect(serv, &_Server::newConnection, serv, &_Server::newSocketConnection);
    // set server to state Listen
    if (serv->listen(QHostAddress::Any, 32500))
        qDebug() << "[INFO] Server started!";
    else qCritical() << "[ERROR] Info: " << serv->errorString();
}

_ConnectionData* _Server::connectionData() {
    return m_Data; 
}
_ConnectionHandler* _Server::connectionHandler() {
    return m_Handler; 
}

void _Server::sendMessageToSocket(QTcpSocket* socket, const QByteArray& arr)
{
    if (!socket) return;
    if (socket->state() == QAbstractSocket::UnconnectedState) return;

    socket->write(_Buffer::IntToArray(arr.size()));
    socket->write(arr);

    if (!socket->waitForBytesWritten())
        qCritical() << "[ERROR] Message wasn't sent";
}

_Server* _Server::p_instance{ nullptr };