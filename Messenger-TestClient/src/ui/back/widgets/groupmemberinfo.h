#pragma once

#include <QWidget>

namespace Ui {
    class GroupMemberInfo;
}

class BaseAccountData;

class GroupMemberInfo : public QWidget
{
    Q_OBJECT

public:
    explicit GroupMemberInfo(QWidget *parent = nullptr);
    ~GroupMemberInfo() override;

    void setInfo(BaseAccountData* member);

private:
    Ui::GroupMemberInfo *ui;
};
