#pragma once

#include <QWidget>

namespace Ui {
    class StartMenu;
}

class StartMenu : public QWidget
{
    Q_OBJECT

public:
    explicit StartMenu(QWidget *parent = nullptr);
    ~StartMenu() override;

private slots:
    void on_linkAlreadyAccount_linkActivated(const QString &link);
    void on_buttonStartMsg_clicked();

private:
    Ui::StartMenu *ui;
};
