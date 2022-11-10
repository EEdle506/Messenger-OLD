#pragma once

#include <QObject> 

class _Buffer {
    qint32* m_size;
    QByteArray* m_buffer;

public:
    _Buffer();
    ~_Buffer();

    qint32 *size() const;
    QByteArray *array() const;

    static QByteArray IntToArray(qint32 source);
    static qint32 ArrayToInt(QByteArray source);
};
