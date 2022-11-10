#pragma once

#include <QObject>

#include <QDateTime>

#include <QPixmap>
#include <QMessageBox>
#include <QFileDialog>

#include <QSettings>

#include <QListWidgetItem>

#include <QMetaType>
#include <QVariant>
#include <QString>
#include <QVector>

#include <map>
#include <exception>

#include "connectionhandler/connectionhandler.h"
#include "connectionhandler/buffer.h"

namespace Messenger
{
    Q_NAMESPACE

    enum class Command {
        // 
        Exist,
        // command which loads something
        LoadInfo,
        LoadImg,
        //
        Add,
        // command which deletes something from client
        Remove,
        // member status
        Online,
        Offline,
        // Info messages
        WinInfo,
        Info,
        Warning,
        Error,
    };
    Q_ENUM_NS(Command)

    enum class Parameter {
        Account, 
        Member,
        // Account types
        Phone, Email, Password, About,
        // Rooom type (room name)
        Group,
        // Group owner
        Owner,
        // Types
        Tag,
        Name,
        Date,
        //
        //
        Msg,
        //
        Pic,
        Current,
        //
        Clear
    };
    Q_ENUM_NS(Parameter)

    template<typename QEnum>
    static inline QString EnumToString(QEnum value) {
        return QVariant::fromValue(value).toString().toLower();
    }
};
