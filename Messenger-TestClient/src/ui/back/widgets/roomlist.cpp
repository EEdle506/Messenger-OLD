#include "roomlist.h"
#include "ui_grouplist.h"

#include "client/clientclass.h"

GroupList::GroupList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RoomList)
{
    ui->setupUi(this);
}

GroupList::~GroupList()
{
    delete ui;
}

void GroupList::addGroupItem(BaseRoomData *room)
{
    m_groupNames.push_back(room);
    ui->groups->addItem(room);
}

BaseRoomData* GroupList::roomAtHash(const QString& hash)
{
    for (const auto& room : m_groupNames)
        if (room->hash() == hash)
            return room;
    return nullptr;
}

BaseRoomData* GroupList::currentData() const
{
    return m_currentGroup;
}

QString GroupList::currentGroup() const
{
    if (m_currentGroup) return m_currentGroup->hash();
    return "";
}

bool GroupList::isEmpty() const
{
    return m_groupNames.isEmpty();
}

qsizetype GroupList::size() const
{
    return m_groupNames.size();
}

void GroupList::clear()
{
    for (auto& group : m_groupNames)
        delete group;
    m_groupNames.clear();
    ui->groups->clear();
}

void GroupList::removeItem(BaseRoomData* room) 
{
    ui->groups->takeItem(ui->groups->currentRow());
    m_groupNames.removeOne(room);
}

void GroupList::on_groups_itemClicked(QListWidgetItem* item)
{
    BaseRoomData* room = static_cast<BaseRoomData*>(item);

    m_currentGroup = room;

    emit groupClicked();
}
void GroupList::on_groups_itemDoubleClicked(QListWidgetItem* item)
{
    BaseRoomData* room = static_cast<BaseRoomData*>(item);
    m_currentGroup = room;

    ConnectionHandler::sendMessage(QString(
        "/load gname=" + room->hash()
    ).toUtf8());
}
void GroupList::on_closeList_clicked()
{
    hide();
}
