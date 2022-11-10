#include "clientclass.h"

#include "parser/parser.h"

ClientClass::ClientClass(QObject *parent)
    : QObject(parent), m_settings(new QSettings(this))
    , m_account(new BaseAccountData())
{
    m_parser = new CommandParser();
    m_start = new StartMenu();

    m_groupList = new GroupList();
    m_groupMembers = new GroupMemberList();
    m_msgMenu = new MessagingMenu();

    m_showgroupInfo = new GroupInfo(m_mainWindow);
    m_showmemInfo = new GroupMemberInfo(m_mainWindow);

    m_showaccInfo = new ShowAccountInfo(m_account);

    connect(m_groupList, &GroupList::groupClicked, m_groupMembers, &GroupMemberList::onGroupClicked);
    connect(m_groupList, &GroupList::groupClicked, m_msgMenu, &MessagingMenu::onGroupClicked);
}

ClientClass::~ClientClass() 
{
    m_parser->deleteLater();
    m_mainWindow->deleteLater();
    m_start->deleteLater();
}

ClientClass *ClientClass::instance()
{
    if (!m_instance) m_instance = new ClientClass();
    return m_instance;
}

void ClientClass::Start()
{
    auto client = instance();

    auto account = client->account();
    auto sett = client->settings();

    client->m_mainWindow = new MainWindow((MainWindow::Theme)client->m_settings->value("theme", (int)MainWindow::Theme::Dark).toInt(), client->m_account);

    client->m_mainWindow->setCentralWidget(client->m_msgMenu);

    ConnectionHandler* handler = ConnectionHandler::instance();

    connect(handler, &ConnectionHandler::socketConnected, client, &ClientClass::onConnected);
    connect(handler, &ConnectionHandler::socketConnected, client->mainMenu(), &MainWindow::onSetState);
    connect(handler, &ConnectionHandler::socketDisconnected, client, &ClientClass::onDisconnected);
    connect(handler, &ConnectionHandler::socketDisconnected, client->mainMenu(), &MainWindow::onSetState);
    
    // parsing incoming message
    connect(handler, &ConnectionHandler::dataRead,
        client->m_parser, &CommandParser::inputCommand);
    // execute incoming command
    connect(client->m_parser, &CommandParser::dataParsed, [](_BaseCommand* command) {
        try {
            command->execute();
        }
        catch (const std::exception& e) {
            qCritical() << e.what();
        }
    });

    sett->beginGroup("accountData");
    QStringList acc = sett->value("account", QStringList()).toStringList();
    sett->endGroup();

    if (acc.isEmpty())
        client->m_start->show();
    else client->m_mainWindow->show();

    if (acc.isEmpty()) return;

    account->addAccountData(Messenger::Parameter::Tag, acc.first().toUtf8());
    account->addAccountData(Messenger::Parameter::Password, acc.last().toUtf8());
}

void ClientClass::onConnected()
{
    if (m_account->isEmpty()) return;
    ConnectionHandler::sendMessage(QString("/connect tag=" + m_account->accountInfo(Messenger::Parameter::Tag)
        + " password=" + m_account->accountInfo(Messenger::Parameter::Password)).toUtf8(),
        mainMenu());
}
void ClientClass::onDisconnected()
{
    groupList()->hide();
    memberList()->hide();

    groupList()->clear();
    memberList()->clear();
    msgMenu()->clear();
}

QSettings* ClientClass::settings()
{
    return m_settings;
}
BaseAccountData *&ClientClass::account()
{
    return m_account;
}
StartMenu *ClientClass::startMenu() const
{
    return m_start;
}
MainWindow *ClientClass::mainMenu() const
{
    return m_mainWindow;
}
MessagingMenu* ClientClass::msgMenu() const
{
    return m_msgMenu;
}
GroupInfo* ClientClass::groupInfo() const
{
    return m_showgroupInfo;
}
ShowAccountInfo* ClientClass::accountInfo() const
{
    return m_showaccInfo;
}
GroupMemberInfo* ClientClass::memberInfo()
{
    return m_showmemInfo;
}
GroupList*& ClientClass::groupList()
{
    return m_groupList;
}
GroupMemberList*& ClientClass::memberList() 
{
    return m_groupMembers;
}

ClientClass* ClientClass::m_instance = nullptr;