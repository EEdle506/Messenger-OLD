#include "memberlist.h"
#include "ui_groupmemberlist.h"

#include "client/clientclass.h"

using namespace Messenger;

GroupMemberList::GroupMemberList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GroupMemberList)
{
    ui->setupUi(this);
}

GroupMemberList::~GroupMemberList()
{
    delete ui;
}

void GroupMemberList::addMember(BaseAccountData* member)
{
    m_members.push_back(member);
    ui->membersList->addItem(member->name());
}

void GroupMemberList::removeMember(const QString& memHash)
{
    QString name;
    for (const auto& member : m_members) {
        if (member->hash() == memHash)
            name = member->name();
    }

    QList <QListWidgetItem*> temp = ui->membersList->findItems(name, Qt::MatchExactly);
    if (temp.isEmpty()) return;

    ui->membersList->removeItemWidget(temp.first());
}

BaseAccountData* GroupMemberList::currentMemberData() const
{
    return m_currentMember;
}

QString GroupMemberList::currentMemberHash() const
{
    return m_currentMember->accountInfo(Messenger::Parameter::Account);
}

bool GroupMemberList::isEmpty() const
{
    return m_members.isEmpty();
}

void GroupMemberList::clear()
{
    m_members.clear();
    ui->membersList->clear();
}

//void GroupMemberList::on_membersList_itemClicked(QListWidgetItem* item)
//{
//
//    auto client = ClientClass::instance();
//    auto group = client->groupList();
//}

void GroupMemberList::onGroupClicked()
{
    ui->membersList->clear();

    auto client = ClientClass::instance();
    auto group = client->groupList();

    ConnectionHandler::sendMessage(QString(
        "/load gname=" + group->currentGroup()).toUtf8()
        + " aname="
    );
}

void GroupMemberList::on_closeList_clicked()
{
    hide();
}
