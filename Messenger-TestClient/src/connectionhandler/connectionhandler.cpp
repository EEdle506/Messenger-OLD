#include "connectionhandler.h"

#include "client/clientclass.h"
#include "connectionhandler/buffer.h"

#include <QTimer>

ConnectionHandler::ConnectionHandler(QObject *parent)
    : QObject(parent)
    , m_buffer(new _Buffer())
{
    m_Socket = new QTcpSocket();

    connect(m_Socket, &QTcpSocket::stateChanged, this, &ConnectionHandler::socketStateChanged);
    connect(m_Socket, &QTcpSocket::readyRead, this, &ConnectionHandler::readyRead);
    connect(m_Socket, &QTcpSocket::disconnected, this, [this]() {
        m_Socket->deleteLater();
        m_Socket = nullptr;
        QTimer::singleShot(600, this, &ConnectionHandler::tryReconnect);
    });

    tryReconnect();
}
ConnectionHandler::~ConnectionHandler()
{
    m_Socket->deleteLater();
}

ConnectionHandler *ConnectionHandler::instance()
{
    if (!m_instance) m_instance = new ConnectionHandler();
    return m_instance;
}
 
bool ConnectionHandler::sendMessage(const QByteArray& msg, QWidget* widget)
{
    auto handler = instance();
    auto socket = handler->m_Socket;

    if (!socket) return false;

    if (!handler->m_canSend) {
        if (widget) QMessageBox::information(widget, "Critical", "Connection lost!");
        return false;
    }

    socket->write(_Buffer::IntToArray(msg.size()));
    socket->write(msg);

    if (socket->waitForBytesWritten())
        return true;
    if (widget) QMessageBox::information(widget, "Critical", "Message wasn't send");
    return false;
}
void ConnectionHandler::readyRead()
{
    m_Socket = static_cast<QTcpSocket*>(sender());
    if (!m_Socket) return;

    _Buffer* buffer = m_buffer;

    QByteArray* arr = buffer->array();

    qint32 *s = buffer->size();
    qint32 size = *s;

    while (m_Socket->bytesAvailable() > 0)
    {
        arr->append(m_Socket->readAll());
        while ((size == 0 && arr->size() >= 4) || (size > 0 && arr->size() >= size)) //While can process data, process it
        {
            if (size == 0 && arr->size() >= 4) //if size of data has received completely, then store it on our global variable
            {
                size = _Buffer::ArrayToInt(arr->mid(0, 4));
                *s = size;
                arr->remove(0, 4);
            }
            if (size > 0 && arr->size() >= size) // If data has received completely, then emit our SIGNAL with the data
            {
                emit dataRead(arr->mid(0, size));
                arr->remove(0, size);
                size = 0;
                *s = size;
            }
        }
    }
}

void ConnectionHandler::tryReconnect()
{
    if (!m_Socket) {
        m_Socket = new QTcpSocket();

        connect(m_Socket, &QTcpSocket::stateChanged, this, &ConnectionHandler::socketStateChanged);
        connect(m_Socket, &QTcpSocket::readyRead, this, &ConnectionHandler::readyRead);
        connect(m_Socket, &QTcpSocket::disconnected, this, [this]() {
            m_Socket->deleteLater();
            m_Socket = nullptr;
            m_canSend = false;
            QTimer::singleShot(1000, this, &ConnectionHandler::tryReconnect);
        });
    }

    m_Socket->connectToHost("178.158.229.37", 32500);
}
void ConnectionHandler::socketStateChanged(QAbstractSocket::SocketState state)
{
    auto account = ClientClass::instance()->account();
    auto mainmenu = ClientClass::instance()->mainMenu();
    auto groupList = ClientClass::instance()->groupList();
    auto memberList = ClientClass::instance()->memberList();

    switch (state) {
    case QAbstractSocket::UnconnectedState:

        m_canSend = false;
        emit socketDisconnected("Client disconnected from this server");

        break;
    case QAbstractSocket::ConnectingState:

        m_canSend = false;
        emit socketConnected("Connecting to this server...");

        break;
    case QAbstractSocket::ConnectedState:
        m_canSend = true;
        emit socketConnected("Connected to this server!");

        break;
    case QAbstractSocket::ClosingState:

        m_canSend = false;
        emit socketDisconnected("Server is closing...");

        break;
    }
}

ConnectionHandler* ConnectionHandler::m_instance = nullptr;