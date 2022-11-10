#pragma once

#include <QMainWindow>
#include <QSystemTrayIcon>

#include "ui/back/widgets/showaccountinfo.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum Theme {
        Dark,
        White
    };
    explicit MainWindow(Theme type, BaseAccountData* account, QWidget *parent = nullptr);
    ~MainWindow();

    void changeEvent(QEvent* event) override;

public slots:
    void onSetState(const QString& status);

    void onMsgClicked();
    void onShowMessageInTray(const QString& msg);
    void onSysTrayActivated(QSystemTrayIcon::ActivationReason reason);

private slots:
    void onShowAccountInfo();
    void onLeaveAccount();

    void onShangeTheme(Theme type);

    void onJoinGroup();
    void onLeaveGroup();
    void onCreateGroup();

private:
    QSystemTrayIcon* m_sysTray = nullptr;
    // Account data
    BaseAccountData* m_account = nullptr;
    // TYPE THEME
    Theme m_theme = White;
    //
    Ui::MainWindow *ui;
};
