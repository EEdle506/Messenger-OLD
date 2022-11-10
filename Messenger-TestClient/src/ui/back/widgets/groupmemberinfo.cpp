#include "groupmemberinfo.h"
#include "ui_groupmemberinfo.h"

#include "client/clientnamespace.h"
#include "client/baseaccount.h"

GroupMemberInfo::GroupMemberInfo(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GroupMemberInfo)
{
    ui->setupUi(this);
}

GroupMemberInfo::~GroupMemberInfo()
{
    delete ui;
}

void GroupMemberInfo::setInfo(BaseAccountData* member)
{
    using namespace Messenger;

    if (!member->images().isEmpty()) {
        ui->memberPhoto->setPixmap(*member->images().first());
    }

    ui->memberName->setText(member->accountInfo(Parameter::Account));
    ui->lastOnline->setText("Last seen " + member->accountInfo(Parameter::Date));
    ui->memberTag->setText(member->accountInfo(Parameter::Tag));
    ui->memberAbout->setText(member->accountInfo(Parameter::About));
}
