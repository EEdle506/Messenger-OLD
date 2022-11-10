#include "messagingmenu.h"
#include "ui_messagingmenu.h"

#include "client/clientclass.h"

#include "ui/back/messageitem/pictureitem.h"

#include <QBuffer>
#include <QLabel>

MessagingMenu::MessagingMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MessagingMenu)
{
    ui->setupUi(this);
}

MessagingMenu::~MessagingMenu()
{
    delete ui;
}

void MessagingMenu::onGroupClicked()
{
    ui->messages->clear();
    auto client = ClientClass::instance();
    auto group = client->groupList();

    ConnectionHandler::sendMessage(QString(
        "/load gname=" + group->currentGroup()).toUtf8()
        + " message="
    );
    ConnectionHandler::sendMessage(QString(
        "/load gname=" + group->currentGroup()).toUtf8()
        + " picture="
    );
}

void MessagingMenu::on_buttonSend_clicked()
{
    using namespace Messenger;

    if (ui->inputedText->text().isEmpty())
        return;

    if (!ConnectionHandler::sendMessage("", this))
        return;

    auto client = ClientClass::instance();
    auto group = client->groupList();

    ConnectionHandler::sendMessage("/send gname=" + group->currentGroup().toUtf8()
        + " aname=" + client->account()->accountInfo(Parameter::Account).toUtf8()
        + " message=" + ui->inputedText->text().toUtf8());

    ui->inputedText->clear();
}
void MessagingMenu::on_inputedText_returnPressed()
{
    on_buttonSend_clicked();
}
void MessagingMenu::on_buttonImage_clicked()
{
    using namespace Messenger;

    if (!ConnectionHandler::sendMessage("", this))
        return;

    auto client = ClientClass::instance();
    QString imagePath = QFileDialog::getOpenFileName(client->mainMenu(), "Choose your image", "C:/Users/Public/Pictures",
                                                    "PNG Image (*.png);");
    // Reading file from device to byte array
    QImage img(imagePath);
    QPixmap pixmap = QPixmap::fromImage(img);

    QByteArray imgArr;
    imgArr.clear();

    QBuffer buffer(&imgArr);
    buffer.open(QIODevice::WriteOnly);

    pixmap.save(&buffer, "PNG");

    if ((imgArr.size() / 1024) > MAX_PICTURE_SEND_SIZE) {
        QMessageBox::critical(this, "Error", "This file is more than 8 MB");
        return;
    }

    if (imgArr.isEmpty()) return;

    auto group = client->groupList();

    ConnectionHandler::sendMessage("/send gname=" + group->currentGroup().toUtf8()
        + " aname=" + client->account()->accountInfo(Parameter::Account).toUtf8()
        + " message=" + ui->inputedText->text().toUtf8()
        + " picture=" + imgArr
    , this);

    ui->inputedText->clear();
}

void MessagingMenu::on_messages_itemClicked(QListWidgetItem* item)
{
    PictureItem *img = static_cast<PictureItem*>(item);

    const auto& msppedData = img->msgInfo();

    const auto& msg = msppedData.find(Messenger::Parameter::Pic);
    if (msg == msppedData.end())
        return;

    QDialog* dialog = new QDialog(this);

    QHBoxLayout* layout = new QHBoxLayout(dialog);

    QLabel* image = new QLabel();

    layout->addWidget(image);

    QByteArray arrImg = img->msgInfo().at(Messenger::Parameter::Pic);
    QPixmap pix;

    pix.loadFromData(arrImg, "PNG");
    image->setPixmap(pix);

    dialog->exec();
}
void MessagingMenu::addWidgetToLayout(int index, QWidget* widget)
{
    ui->infoLayout->insertWidget(index, widget);
}
void MessagingMenu::addMsgItem(MsgData *msg)
{
    ui->messages->addItem(msg);
    ui->messages->setCurrentRow(ui->messages->count());
    ui->messages->scrollToBottom();
}

void MessagingMenu::clear()
{
    ui->messages->clear();
}