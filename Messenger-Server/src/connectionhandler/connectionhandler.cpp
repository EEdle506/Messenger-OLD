#include "connectionhandler.h"

_ConnectionHandler::_ConnectionHandler(QObject *parent)
    : QObject(parent) {}
_ConnectionHandler::~_ConnectionHandler() {}

QTcpSocket* _ConnectionHandler::currentSocket() { return m_currentSocket; }

void _ConnectionHandler::pushSocketToQueue(QTcpSocket* socket)
{
    if (!socket) return;

    connect(socket, &QTcpSocket::readyRead, this, &_ConnectionHandler::readyRead);
    connect(socket, &QTcpSocket::disconnected, this, &_ConnectionHandler::disconnected);

    m_socketQueue.insert(socket, new _Buffer());

    qDebug() << "[INFO] Socket was added to queue: " << socket->socketDescriptor();
}

void _ConnectionHandler::readyRead()
{
    auto socket = static_cast<QTcpSocket*>(sender());
    if (!socket) return;

    m_currentSocket = socket;

    _Buffer* buffer = m_socketQueue.value(socket, (_Buffer*)nullptr);

    qint32 *s = buffer->size();
    qint32 size = *s;

    while (socket->bytesAvailable() > 0)
    {
        buffer->array()->append(socket->readAll());
        while ((size == 0 && buffer->array()->size() >= 4) || (size > 0 && buffer->array()->size() >= size)) //While can process data, process it
        {
            if (size == 0 && buffer->array()->size() >= 4) //if size of data has received completely, then store it on our global variable
            {
                size = _Buffer::ArrayToInt(buffer->array()->mid(0, 4));
                *s = size;
                buffer->array()->remove(0, 4);
            }
            if (size > 0 && buffer->array()->size() >= size) // If data has received completely, then emit our SIGNAL with the data
            {
                emit dataRead(buffer->array()->mid(0, size));
                buffer->array()->remove(0, size);
                size = 0;
                *s = size;
            }
        }
    }
}

void _ConnectionHandler::disconnected()
{
    auto socket = static_cast<QTcpSocket*>(sender());

    _Buffer* buffer = m_socketQueue.value(socket);
    m_socketQueue.remove(socket);
    delete buffer;

    emit socketDisconnected(socket);
}
