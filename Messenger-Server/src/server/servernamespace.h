#pragma once

#include <QObject>

#include <QMetaType>
#include <QString>
#include <QVector>

#include <QtAlgorithms>

#include <QDateTime>

// DATA BASE CONNECTION

#include "db/database.h"

// SERVER CONNECTION

#include <QTcpServer>
#include <QTcpSocket>

#include "connectionhandler/buffer.h"

namespace Server 
{
    Q_NAMESPACE
    
    enum class Command 
    {
        Check,
        // Connects and setups client to chats, groups etc.
        Connect,
        Load,
        // Client can add/change/delete groups, chats, account 
        Create,
        Change,
        Delete,
        // Client can join/leave to/from group, chat etc.
        Join,
        Leave,
        // Client sends message to group, chat etc.
        Send
    };
    Q_ENUM_NS(Command)
    
    enum class Parameter 
    {
        // User type
        Aname,          // User hash
        //Mname,          // Member hash
        // 
        Phone,          // User phone
        // Room type
        Gname,          // Group hash
        // Cname,          // Chat hash
        // Security types
        Email,          // 
        Password,       // Password to lock account or rooms
        // 
        Tag,            // (unique name)
        // Data types
        Picture,        // mediumblob
        Message,        // VARCHAR
        // Connection status
        Online,
        Offline
    };
    Q_ENUM_NS(Parameter)
    
    template<typename QEnum>
    static inline QString EnumToString(QEnum value) {
        return QVariant::fromValue(value).toString().toLower();
    }
};
