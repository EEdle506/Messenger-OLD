#include "dialogusername.h"
#include "ui_dialogusername.h"

#include <QMessageBox>

DialogUsername::DialogUsername(DialogUser type, QWidget *parent) :
    QDialog(parent), m_type(type),
    ui(new Ui::DialogUsername)
{
    ui->setupUi(this);
    setModal(true);

    switch (m_type)
    {
    case Create: ui->pushButton->setText("Create an account");
        break;
    case Login:
        delete ui->layoutName;
        ui->lineName->setVisible(false);
        ui->labelName->setVisible(false);
        delete ui->layoutPhone;
        ui->lineEditPhone->setVisible(false);
        ui->labelPhone->setVisible(false);
        ui->phonenumCountry->setVisible(false);
        ui->pushButton->setText("Login to account");
        break;
    }
}
DialogUsername::~DialogUsername()
{
    delete ui;
}

_UserData DialogUsername::getUserData()
{
    _UserData data;

    data.nameTag = ui->nameTag->text();
    data.name = ui->lineName->text();
    data.phone = ui->lineEditPhone->text();
    data.password = ui->linePassword->text();

    if (m_type == DialogUser::Create) {
        QStringList splStr = ui->phonenumCountry->currentText().split(' ');

        data.phoneCountry = splStr.first();
        data.phoneCountryNum = splStr.back().removeIf([](const QChar& sym)->bool {
            if (sym == '(' || sym == ')' || sym == '+')
                return true;
            return false;
            });
    }
    return data;
}


void DialogUsername::on_pushButton_clicked()
{
    if (ui->nameTag->text().size() < 4) {
        QMessageBox::information(this, "Info", "Tag must be more than 4 symbols");
        return;
    }
    if (ui->lineName->text().isEmpty() && m_type == DialogUser::Create) {
        QMessageBox::information(this, "Info", "Name is empty");
        return;
    }
    if (ui->lineEditPhone->displayText().size() != ui->lineEditPhone->inputMask().size() 
        && m_type == DialogUser::Create) 
    {
        QMessageBox::information(this, "Info", "Phone is uncompleted");
        return;
    }
    accept();
}

void DialogUsername::on_buttonSeePass_pressed()
{
    ui->buttonSeePass->setIcon(QIcon(":/start/ui/images/eye.ico"));
    ui->linePassword->setEchoMode(QLineEdit::EchoMode::Normal);
}

void DialogUsername::on_buttonSeePass_released()
{
    ui->buttonSeePass->setIcon(QIcon(":/start/ui/images/blind.ico"));
    ui->linePassword->setEchoMode(QLineEdit::EchoMode::Password);
}

