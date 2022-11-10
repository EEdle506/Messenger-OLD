#include "dialoggroup.h"
#include "ui_dialoggroup.h"

DialogGroup::DialogGroup(GroupMenu type, QWidget *parent) :
    QDialog(parent), 
    ui(new Ui::DialogGroup)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    setModal(true);

    switch (type)
    {
    case Create:
        break;
    case Join:
        ui->name->setVisible(false);
        ui->groupName->setVisible(false);
        ui->buttonCreateGroup->setText("Join to group");
        break;
    }
}

DialogGroup::~DialogGroup()
{
    delete ui;
}

_GroupData DialogGroup::getGroupInfo()
{
    return { ui->groupName->text(), ui->groupTag->text() };
}

void DialogGroup::on_buttonCreateGroup_clicked()
{
    accept();
}