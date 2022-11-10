#include "client/clientnamespace.h"

#include "client/baseaccount.h"

#include "ui/back/widgets/mainwindow.h"

#include "ui/back/widgets/startmenu.h"
#include "ui/back/widgets/messagingmenu.h"

#include "ui/back/widgets/roomlist.h"
#include "ui/back/widgets/memberlist.h"

class CommandParser;
class ClientClass : public QObject
{
    Q_OBJECT

    CommandParser* m_parser = nullptr;
    BaseAccountData* m_account = nullptr;

public:
    ~ClientClass() override;

    static void Start();
    static ClientClass *instance();

    QSettings* settings();
    BaseAccountData *&account();

    StartMenu* startMenu() const;
    MainWindow* mainMenu() const;
    MessagingMenu* msgMenu() const;

    GroupInfo* groupInfo() const;
    ShowAccountInfo* accountInfo() const;
    GroupMemberInfo* memberInfo();

    GroupList*& groupList();
    GroupMemberList*& memberList();

public slots:
    void onConnected();
    void onDisconnected();

private:
    explicit ClientClass(QObject *parent = nullptr);

    MainWindow* m_mainWindow = nullptr;

    // ui which displays main window with message item
    MessagingMenu* m_msgMenu = nullptr;

    // lists (groups, members)
    GroupList* m_groupList = nullptr;
    GroupMemberList* m_groupMembers = nullptr;

    // Loads when that's a first start up of a applacation 
    StartMenu* m_start = nullptr;
    
    // INFO DIALOGS
    // shows info about group
    GroupInfo* m_showgroupInfo = nullptr;
    // show account data
    ShowAccountInfo* m_showaccInfo = nullptr;
    // 
    GroupMemberInfo* m_showmemInfo = nullptr;

    //
    QSettings* m_settings;
    static ClientClass* m_instance;
};
