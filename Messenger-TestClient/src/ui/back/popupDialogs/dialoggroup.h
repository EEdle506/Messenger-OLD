#pragma once

#include <QDialog>

namespace Ui {
class DialogGroup;
}

enum GroupMenu {
    Create,
    Join
};

struct _GroupData {
    QString groupName, groupTag;
};

class DialogGroup : public QDialog
{
    Q_OBJECT

public:
    explicit DialogGroup(GroupMenu type, QWidget *parent = nullptr);
    ~DialogGroup();

    _GroupData getGroupInfo();

private slots:
    void on_buttonCreateGroup_clicked();

private:
    Ui::DialogGroup *ui;
};
