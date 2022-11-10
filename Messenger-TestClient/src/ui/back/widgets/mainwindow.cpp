#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStyleFactory>

#include "client/clientnamespace.h"
#include "client/clientclass.h"

#include "ui/back/popupDialogs/dialoggroup.h"
#include "ui/back/widgets/groupmemberinfo.h"

MainWindow::MainWindow(Theme type, BaseAccountData* account, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_sysTray(new QSystemTrayIcon(QIcon(":/app/ui/images/AppIcon.ico"), this))
    , m_account(account)
{
    ui->setupUi(this);

    connect(m_sysTray, &QSystemTrayIcon::activated, this, &MainWindow::onSysTrayActivated);
    connect(m_sysTray, &QSystemTrayIcon::messageClicked, this, &MainWindow::onMsgClicked);

    connect(ui->groupJoin, &QAction::triggered, this, &MainWindow::onJoinGroup);
    connect(ui->groupCreate, &QAction::triggered, this, &MainWindow::onCreateGroup);
    connect(ui->groupLeave, &QAction::triggered, this, &MainWindow::onLeaveGroup);
    connect(ui->accountInfo, &QAction::triggered, this, &MainWindow::onShowAccountInfo);
    connect(ui->accountLeave, &QAction::triggered, this, &MainWindow::onLeaveAccount);

    // about 
    connect(ui->aboutQt, &QAction::triggered, this, [this]() {
        QMessageBox::aboutQt(this);
    });
    connect(ui->aboutAuthor, &QAction::triggered, this, [this]() {
        QMessageBox::information(this, "Author", "Samarin I. V.\nMessenger 0.0.2 (Pre-Alpha)");
    });

    // theme change
    connect(ui->darkTheme, &QAction::triggered, [this]() {
        onShangeTheme(Theme::Dark);
    });
    connect(ui->whiteTheme, &QAction::triggered, [this]() {
        onShangeTheme(Theme::White);
    });

    connect(ui->showMemberList, &QAction::triggered, []() {
        auto list = ClientClass::instance()->memberList();
        if (list && !list->isEmpty()) list->show();
    });
    connect(ui->showGroupList, &QAction::triggered, []() {
        auto list = ClientClass::instance()->groupList();
        if (list && !list->isEmpty()) list->show();
    });

    onShangeTheme(type);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onSetState(const QString &status)
{
    ui->connectState->showMessage(status);
}

void MainWindow::onShowAccountInfo()
{
    using namespace Messenger;

    if (!ConnectionHandler::sendMessage("", this))
        return;

    ConnectionHandler::sendMessage("/load aname=" + m_account->hash().toUtf8());
}
void MainWindow::onLeaveAccount()
{
    using namespace Messenger;

    if (!ConnectionHandler::sendMessage("", this)) {
        return;
    }
    if (QMessageBox::warning(this, "Warning", "Do you want to leave this account?",
        QMessageBox::Ok, QMessageBox::Close)
        == QMessageBox::Close) {
        return;
    }

    ConnectionHandler::sendMessage("/leave aname=" + m_account->hash().toUtf8());
}
void MainWindow::onShangeTheme(Theme type)
{
    if (type == m_theme)
        return;

    m_theme = type;
    auto setting = ClientClass::instance()->settings();
    setting->setValue("theme", (int)type);

    QPalette darkPalette;

    switch (m_theme) {
    case Dark:
        // modify palette to dark
        darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
        darkPalette.setColor(QPalette::WindowText, Qt::white);
        darkPalette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(127, 127, 127));
        darkPalette.setColor(QPalette::Base, QColor(42, 42, 42));
        darkPalette.setColor(QPalette::AlternateBase, QColor(66, 66, 66));
        darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
        darkPalette.setColor(QPalette::ToolTipText, Qt::white);
        darkPalette.setColor(QPalette::Text, Qt::white);
        darkPalette.setColor(QPalette::Disabled, QPalette::Text, QColor(127, 127, 127));
        darkPalette.setColor(QPalette::Dark, QColor(35, 35, 35));
        darkPalette.setColor(QPalette::Shadow, QColor(20, 20, 20));
        darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
        darkPalette.setColor(QPalette::ButtonText, Qt::white);
        darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(127, 127, 127));
        darkPalette.setColor(QPalette::BrightText, Qt::red);
        darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
        darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
        darkPalette.setColor(QPalette::Disabled, QPalette::Highlight, QColor(80, 80, 80));
        darkPalette.setColor(QPalette::HighlightedText, Qt::white);
        darkPalette.setColor(QPalette::Disabled, QPalette::HighlightedText, QColor(127, 127, 127));
        darkPalette.setColor(QPalette::PlaceholderText, Qt::white);
        break;
    case White:
        break;
    }
    qApp->setPalette(darkPalette);
}

void MainWindow::onCreateGroup()
{
    if (!ConnectionHandler::sendMessage("", this))
        return;

    DialogGroup* dialog = new DialogGroup(GroupMenu::Create, this);
    if (dialog->exec() == QDialog::Accepted) {
        _GroupData data = dialog->getGroupInfo();
        auto client = ClientClass::instance();
        ConnectionHandler::sendMessage("/create gname=" + QString(data.groupName
            + " tag=" + data.groupTag
            + " aname=" + client->account()
            ->accountInfo(Messenger::Parameter::Account)
        ).toUtf8(), this);
    }
    dialog->deleteLater();
}
void MainWindow::onJoinGroup()
{
    if (!ConnectionHandler::sendMessage("", this))
        return;

    DialogGroup* dialog = new DialogGroup(GroupMenu::Join, this);
    if (dialog->exec() == QDialog::Accepted) {
        _GroupData data = dialog->getGroupInfo();
        auto client = ClientClass::instance();
        ConnectionHandler::sendMessage("/join tag=" + QString(data.groupTag
            + " aname=" + client->account()->hash()
        ).toUtf8(), this);
    }
    dialog->deleteLater();
}
void MainWindow::onLeaveGroup()
{
    if (!ConnectionHandler::sendMessage("", this))
        return;

    if (QMessageBox::warning(this, "Warning", "Do you want to leave this group?",
        QMessageBox::Ok, QMessageBox::Close)
        == QMessageBox::Close) {
        return;
    }

    auto client = ClientClass::instance();
    auto group = client->groupList();

    ConnectionHandler::sendMessage("/leave gname=" + QString(group->currentGroup()
        + " aname=" + m_account->hash()
    ).toUtf8());
}

void MainWindow::onSysTrayActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Unknown:
        break;
    case QSystemTrayIcon::Context:
        break;
    case QSystemTrayIcon::DoubleClick:
        break;
    case QSystemTrayIcon::Trigger:
        this->show();
        this->raise();
        this->activateWindow();
        m_sysTray->hide();
        this->onShowMessageInTray("Window was minimized. Click to open.");
        break;
    case QSystemTrayIcon::MiddleClick:
        break;
    }
}
void MainWindow::onShowMessageInTray(const QString& msg)
{
    m_sysTray->showMessage("Message", msg, QIcon(":/tray/ui/images/chat_message.ico"));
}
void MainWindow::onMsgClicked()
{
    m_sysTray->hide();
    this->show();
    this->raise();
    this->activateWindow();
}

void MainWindow::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::WindowStateChange)
    {
        if (isMinimized() == true) {
            m_sysTray->show();
            onShowMessageInTray("Program was closed in tray.\nClick to open.");
            this->hide();
        }
    }
    return QMainWindow::changeEvent(event);
}