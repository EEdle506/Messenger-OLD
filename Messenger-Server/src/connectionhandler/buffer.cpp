#include "buffer.h"

#include <QIODevice>

_Buffer::_Buffer()
    : m_size(new qint32(0))
    , m_buffer(new QByteArray())
{
    m_buffer->clear();
}

_Buffer::~_Buffer()
{
    delete m_size;
    delete m_buffer;
}

qint32 *_Buffer::size() const
{
    return m_size;
}

QByteArray *_Buffer::array() const
{
    return m_buffer;
}


QByteArray _Buffer::IntToArray(qint32 source)
{
    QByteArray temp;
    QDataStream data(&temp, QIODevice::ReadWrite);
    data << source;
    return temp;
}

qint32 _Buffer::ArrayToInt(QByteArray source)
{
    qint32 temp;
    QDataStream data(&source, QIODevice::ReadWrite);
    data >> temp;
    return temp;
}