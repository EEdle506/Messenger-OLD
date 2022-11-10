#include "commands.h"

#include "client/clientclass.h"

#include "ui/back/messageitem/messageitem.h"
#include "ui/back/messageitem/pictureitem.h"

using namespace Messenger;

_BaseCommand::_BaseCommand(QObject* parent)
    : QObject(parent){}
_BaseCommand::~_BaseCommand() {}
void _BaseCommand::setParameters(const _ParsedData& data)
{
	this->params = data;
}


_CmdExist::_CmdExist() : _BaseCommand() {}
void _CmdExist::execute()
{

}

_CmdWindowsInfo::_CmdWindowsInfo() : _BaseCommand() {}
void _CmdWindowsInfo::execute()
{
    using namespace Messenger;
    auto client = ClientClass::instance();
    auto mainwindow = client->mainMenu();

    QString msg = params.joined(Parameter::Msg);
    if (msg.isEmpty()) return;
    mainwindow->onShowMessageInTray(msg);
}
_CmdInfo::_CmdInfo() : _BaseCommand() {}
void _CmdInfo::execute()
{
    using namespace Messenger;
    auto client = ClientClass::instance();

    QString msg = params.joined(Parameter::Msg);
    if (msg.isEmpty()) return;
    QMessageBox::information(client->mainMenu(), "Info", msg);
}
_CmdWarning::_CmdWarning() : _BaseCommand() {}
void _CmdWarning::execute()
{

}
_CmdError::_CmdError() : _BaseCommand() {}
void _CmdError::execute()
{
	using namespace Messenger;
	auto client = ClientClass::instance();

    auto account = client->account();

    QString msg = params.joined(Parameter::Msg);
    if (msg.isEmpty()) return;
    QMessageBox::critical(client->mainMenu(), "Critical", msg);
}

_CmdLoadInfo::_CmdLoadInfo() : _BaseCommand(){}
void _CmdLoadInfo::execute()
{
    auto client = ClientClass::instance();

    auto account = client->account();

    auto groups = client->groupList();
    auto msgList = client->msgMenu();
    auto members = client->memberList();

    auto accountShow = client->accountInfo();
    auto memberShow = client->memberInfo();
    auto groupShow = client->groupInfo();

    switch (params.first()) {
    case Parameter::Account:

        account->addAccountData(params.mappedData());
        msgList->addWidgetToLayout(-1, accountShow);
        accountShow->show();

        break;
    case Parameter::Group:

        groupShow->setRoomInfo(BaseRoomData(params.mappedData()));
        groupShow->show();

        break;
    case Parameter::Member:

        memberShow->setInfo(new BaseAccountData(params.mappedData()));

        break;
    case Parameter::Msg:

        msgList->addMsgItem(new MessageItem(params.mappedData()));

        break;
    }
}
_CmdLoadImg::_CmdLoadImg() : _BaseCommand() {}
void _CmdLoadImg::execute()
{
    auto client = ClientClass::instance();
    auto account = client->account();

    auto msgList = client->msgMenu();

    auto accountInfo = client->accountInfo();

    switch (params.first()) {
    case Parameter::Account:

        account->loadImage(params.joined(Parameter::Pic), params.joined(Parameter::Current).toInt());

        break;
    case Parameter::Member:

        //member images

        break;
    case Parameter::Pic:

        msgList->addMsgItem(new PictureItem(params.mappedData()));

        break;
    }
}

_CmdAdd::_CmdAdd() : _BaseCommand() {}
void _CmdAdd::execute() 
{
    auto client = ClientClass::instance();
    auto account = client->account();

    auto groups = client->groupList();
    auto members = client->memberList();

    auto msgList = client->msgMenu();

    auto mainMenu = client->mainMenu();

    switch (params.first())
    {
    case Parameter::Account:
        groups->clear();
        members->clear();

        if (params.contains(Parameter::Clear)) {
            ConnectionHandler::sendMessage(QString(
                "/connect tag=" + params.joined(Parameter::Tag)
                + " password=" + params.joined(Parameter::Password)
            ).toUtf8(), client->mainMenu());
        }

        account->setAccountData(params.mappedData());

        client->settings()->beginGroup("accountData");
        client->settings()->setValue("account", QStringList({
            params.joined(Parameter::Tag),
            params.joined(Parameter::Password)
        }));
        client->settings()->endGroup();

        client->startMenu()->hide();
        client->mainMenu()->show();

        client->mainMenu()->onSetState("Welcome back, " + params.joined(Parameter::Name));

        break;
    case Parameter::Group:

        if (groups->isEmpty()) {
            client->msgMenu()->addWidgetToLayout(0, groups);
        }
        groups->addGroupItem(new BaseRoomData(params.mappedData()));
        groups->show();
        members->clear();
        msgList->clear();

        break;
    case Parameter::Member:

        if (members->isEmpty()) {
            client->msgMenu()->addWidgetToLayout(2, members);
        }
        members->addMember(new BaseAccountData(params.mappedData()));
        members->show();

        break;
    case Parameter::Msg:

        msgList->addMsgItem(new MessageItem(params.mappedData()));
        mainMenu->onShowMessageInTray(
            groups->roomAtHash(params.joined(Parameter::Group))->name()
            + "\nMessage: " + params.joined(Parameter::Msg)
        );

        break;
    case Parameter::Pic:

        msgList->addMsgItem(new PictureItem(params.mappedData()));
        mainMenu->onShowMessageInTray(
            groups->currentData()->name()
            + "\nPicture"
        );

        break;
    }
}
_CmdRemove::_CmdRemove() : _BaseCommand(){}
void _CmdRemove::execute()
{
    auto client = ClientClass::instance();
    auto account = client->account();

    auto groups = client->groupList();
    auto members = client->memberList();
    auto msgMenu = client->msgMenu();

    switch (params.first())
    {
    case Parameter::Account:

        // account data and settings
        client->settings()->remove("accountData");
        account->clear();
        // removing main widget
        client->mainMenu()->hide();
        client->startMenu()->show();
        // room and members
        client->groupList()->clear();
        client->memberList()->clear();
        client->msgMenu()->clear();

        break;
    case Parameter::Group:

        if (groups->roomAtHash(params.joined(Parameter::Group))) {
            auto room = groups->roomAtHash(params.joined(Parameter::Group));
            groups->removeItem(room);
            members->clear();
            msgMenu->clear();
        }

        break;
    case Parameter::Member:

        members->removeMember(params.joined(Parameter::Member));

        break;
    }
}

_CmdOnline::_CmdOnline() : _BaseCommand(){}
void _CmdOnline::execute()
{
    switch (params.first())
    {
    //case 
    }
}
_CmdOffline::_CmdOffline() : _BaseCommand(){}
void _CmdOffline::execute()
{
    switch (params.first())
    {

    }
}