#pragma once

#include "commands/data/commanddata.h"
#include "commands/commands.h"

class CommandParser
    : public QObject
{
    Q_OBJECT

    QByteArray m_incomingRow;

public:
    const std::map<_CommandData, _BaseCommand*> m_Commands {
        /*
        *   Command: LoadInfo 
        */
        {
            _CommandData(Messenger::Command::LoadInfo, {
                             // tag
                             Messenger::Parameter::Tag,
                             // type
                             Messenger::Parameter::Account,
                             Messenger::Parameter::Group,
                             Messenger::Parameter::Member,
                             // account types
                             Messenger::Parameter::Name,
                             // account or member phone
                             Messenger::Parameter::Phone,
                             // account email
                             Messenger::Parameter::Email,
                             // account password
                             Messenger::Parameter::Password,
                             // group owner
                             Messenger::Parameter::Owner,
                             // date and info
                             Messenger::Parameter::Date,
                             Messenger::Parameter::About,
                             Messenger::Parameter::Msg
            }),
            new _CmdLoadInfo()
        },
        /*
        *   Command: LoadImage 
        */
        {
            _CommandData(Messenger::Command::LoadImg, {
                             Messenger::Parameter::Account,
                             Messenger::Parameter::Member,
                             //Messenger::Parameter::Group,
                             Messenger::Parameter::Pic,
                             Messenger::Parameter::Date,
                             Messenger::Parameter::Current,
                             Messenger::Parameter::Name
                         }),
            new _CmdLoadImg()
        },
        /*
        *   Command: Add (add members, group, messages)
        */
        {
            _CommandData(Messenger::Command::Add, {
                            Messenger::Parameter::Account,
                            Messenger::Parameter::Member,
                            Messenger::Parameter::Group,
                            Messenger::Parameter::Name,
                            // fields
                            Messenger::Parameter::Tag,
                            Messenger::Parameter::Password,
                            Messenger::Parameter::Msg,
                            Messenger::Parameter::Pic,
                            Messenger::Parameter::Date,
                            Messenger::Parameter::Clear
                        }),
            new _CmdAdd()
        },
        /*
        *   Command: Remove (remove members, group, messages)
        */
        {
            _CommandData(Messenger::Command::Remove, {
                             Messenger::Parameter::Account,
                             Messenger::Parameter::Member,
                             Messenger::Parameter::Group,
                             Messenger::Parameter::Msg
                         }),
            new _CmdRemove()
        },
        /*
        *   Command: Online
        */
        {
            _CommandData(Messenger::Command::Online, {
                             Messenger::Parameter::Group,
                             Messenger::Parameter::Member,
                             Messenger::Parameter::Date
                         }),
            new _CmdOnline()
        },
        /*
        *   Command: Offline
        */
        {
            _CommandData(Messenger::Command::Offline, {
                             Messenger::Parameter::Group,
                             Messenger::Parameter::Member,
                             Messenger::Parameter::Date
                         }),
            new _CmdOffline()
        },
        /*
        *   
        */
        {
            _CommandData(Messenger::Command::WinInfo, {
                             Messenger::Parameter::Msg
                         }),
            new _CmdWindowsInfo()
        },
        /*
        *   Command: Info 
        */
        {
            _CommandData(Messenger::Command::Info, {
                             Messenger::Parameter::Msg
                         }),
            new _CmdInfo()
        },
        /*
        *   Command: Warning 
        */
        {
            _CommandData(Messenger::Command::Warning, {
                             Messenger::Parameter::Msg
                         }),
            new _CmdWarning()
        },
        /*
        *   Command: Error 
        */
        {
            _CommandData(Messenger::Command::Error, {
                             Messenger::Parameter::Msg
                         }),
            new _CmdError()
        },
    };

    CommandParser(QObject* parent = nullptr);
    ~CommandParser();

signals:
    void dataParsed(_BaseCommand* command);

public slots:
    void inputCommand(const QByteArray& data);

private:
    void runParsing();
};
