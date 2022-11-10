#include "startmenu.h"
#include "ui_startmenu.h"

#include "client/clientnamespace.h"
#include "../popupDialogs/dialogusername.h"

StartMenu::StartMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartMenu)
{
    ui->setupUi(this);
}

StartMenu::~StartMenu()
{
    delete ui;
}

void StartMenu::on_linkAlreadyAccount_linkActivated(const QString &link)
{
    if (!ConnectionHandler::sendMessage("", this))
        return;

    DialogUsername* dialog = new DialogUsername(DialogUser::Login, this);

    if (dialog->exec() == QDialog::Accepted) {
        _UserData user = dialog->getUserData();
        ConnectionHandler::sendMessage(QString("/connect tag=" + user.nameTag
            + " password=" + user.password).toUtf8(), this);
    }
    dialog->deleteLater();
}

void StartMenu::on_buttonStartMsg_clicked()
{
    if (!ConnectionHandler::sendMessage("", this))
        return;

    DialogUsername* dialog = new DialogUsername(DialogUser::Create, this);
    if (dialog->exec() == QDialog::Accepted) {
        _UserData user = dialog->getUserData();
        ConnectionHandler::sendMessage(QString("/create aname=" + user.name
                                       + " tag=" + user.nameTag
                                       + " password=" + user.password
                                       + " phone=" + user.phone + ' '
                                       + user.phoneCountry + ' '
                                       + user.phoneCountryNum).toUtf8(), this);
    }
    dialog->deleteLater();
}
