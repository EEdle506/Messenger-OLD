#pragma once

#include <QWidget>
#include <QListWidgetItem>

#include "groupinfo.h"
#include "ui/back/widgets/groupmemberinfo.h"

namespace Ui {
    class RoomList;
}

class GroupList : public QWidget
{
    Q_OBJECT

    QVector <BaseRoomData*> m_groupNames;
    BaseRoomData* m_currentGroup = nullptr;

public:
    explicit GroupList(QWidget *parent = nullptr);
    ~GroupList() override;

    void addGroupItem(BaseRoomData* room);
    void removeItem(BaseRoomData* room);

    BaseRoomData* roomAtHash(const QString& hash);

    BaseRoomData* currentData() const;
    QString currentGroup() const;

    bool isEmpty() const;
    qsizetype size() const;

    void clear();

signals:
    void groupClicked();

private slots:
     void on_groups_itemClicked(QListWidgetItem* item);
     void on_groups_itemDoubleClicked(QListWidgetItem* item);

     void on_closeList_clicked();

private:
    Ui::RoomList *ui = nullptr;
};
