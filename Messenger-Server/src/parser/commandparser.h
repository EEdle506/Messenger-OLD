#pragma once

#include "commands/data/commanddata.h"
#include "commands/commands.h"

class _CommandParser
    : public QObject
{
    Q_OBJECT

    QByteArray m_incomingRow;

public:
    const std::map<_CommandData, _BaseCommand*> m_Commands
    {
        // CHECK
        {
            // 
            _CommandData(Server::Command::Check,
            {
                Server::Parameter::Aname,
                Server::Parameter::Gname,
            }),
            new _CmdCheck()
        },
        // CONNECT
        {
            // Deletes client from queue and 
            // Connects to chats and setups hash for client
            _CommandData(Server::Command::Connect,
            {
                Server::Parameter::Tag,
                Server::Parameter::Password
            }),
            new _CmdConnect()
        },
        {
            _CommandData(Server::Command::Load, 
            {
                Server::Parameter::Aname,
                Server::Parameter::Gname,
                // fields
                Server::Parameter::Message,
                Server::Parameter::Picture,
            }),
            new _CmdLoad()
        },
        // CREATE
        {
            _CommandData(Server::Command::Create,
            {
                Server::Parameter::Phone,
                Server::Parameter::Password,
                Server::Parameter::Tag,
                // Account creation
                Server::Parameter::Aname,
                // Group creation
                Server::Parameter::Gname,
            }),
            new _CmdCreate()
        },
        // CHANGE
        {
            _CommandData(Server::Command::Change,
            {
                Server::Parameter::Tag,
                // Account creation
                Server::Parameter::Aname,
                // Group creation
                Server::Parameter::Gname,
            }),
            new _CmdCreate()
        },
        // DELETE
        {
            _CommandData(Server::Command::Delete,
            {
                Server::Parameter::Aname,
                Server::Parameter::Gname,
            }),
            nullptr
        },
        // JOIN
        {
            _CommandData(Server::Command::Join,
            {
                 Server::Parameter::Password,
                 //
                 Server::Parameter::Tag,
                 Server::Parameter::Aname,
                 // hash to join
                 Server::Parameter::Gname
            }),
            new _CmdJoin()
        },
        // LEAVE
        {
            _CommandData(Server::Command::Leave,
            {
                 Server::Parameter::Tag,
                 // account leave hash
                 Server::Parameter::Aname,
                 // from room hash
                 Server::Parameter::Gname,
            }),
            new _CmdLeave()
        },
        // SEND
        {
            _CommandData(Server::Command::Send,
            {
                Server::Parameter::Aname,
                // 
                Server::Parameter::Gname,
                // message type 
                Server::Parameter::Message,
                Server::Parameter::Picture
            }),
            new _CmdSend()
        }
    };

    _CommandParser(QObject* parent = nullptr);
    ~_CommandParser();

signals:
    void dataParsed(_BaseCommand* command);

public slots:
    void inputCommand(const QByteArray& data);

private:
    void runParsing();
};
