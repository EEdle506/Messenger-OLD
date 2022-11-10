#pragma once

#include <QWidget>

#include "ui/back/messageitem/basemsgitem.h"

namespace Ui {
    class MessagingMenu;
}

class MessagingMenu : public QWidget
{
    Q_OBJECT

public:
    explicit MessagingMenu(QWidget *parent = nullptr);
    ~MessagingMenu() override;

    void addMsgItem(MsgData* msg);

    void clear();

public slots:
    void addWidgetToLayout(int index, QWidget* widget);

    void onGroupClicked();

private slots:
    void on_messages_itemClicked(QListWidgetItem* item);

    void on_inputedText_returnPressed();
    void on_buttonSend_clicked();
    
    void on_buttonImage_clicked();

private:
    Ui::MessagingMenu *ui;
};
