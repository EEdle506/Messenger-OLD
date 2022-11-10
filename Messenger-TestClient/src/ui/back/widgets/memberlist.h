#pragma once

#include <QWidget>

#include <qlistwidget.h>

#include "ui/back/widgets/groupmemberinfo.h"

namespace Ui {
class GroupMemberList;
}

class BaseAccountData;
class GroupMemberList : public QWidget
{
    Q_OBJECT

    QVector <BaseAccountData*> m_members;
    BaseAccountData* m_currentMember;

public:
    explicit GroupMemberList(QWidget *parent = nullptr);
    ~GroupMemberList();

    void addMember(BaseAccountData* member);

    void removeMember(const QString& memHash);

    BaseAccountData* currentMemberData() const;
    QString currentMemberHash() const;

    bool isEmpty() const;
    void clear();

public slots:
    void onGroupClicked();

private slots:
    void on_closeList_clicked();
    //void on_membersList_itemClicked(QListWidgetItem* item);

private:
    Ui::GroupMemberList *ui;
};
