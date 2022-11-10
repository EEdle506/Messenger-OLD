#include "commands.h"

#include "server/server.h"

#include "connectiondata/connectiondata.h"
#include "connectionhandler/connectionhandler.h"

#include "db/database.h"

using namespace Server;

_BaseCommand::_BaseCommand(QObject* parent)
	: QObject(parent) 
{
	connect(this, &_BaseCommand::messageSend, _Server::sendMessageToSocket);
}
_BaseCommand::~_BaseCommand() {}
void _BaseCommand::setParameters(const _ParsedData& data)
{
	if (data.size())
		qDebug() << "[INFO] To execute" << data.first();

	this->params = data;
}
/*
*
*	CHECK
*	Checks socket hash existance in db
*
*/
_CmdCheck::_CmdCheck() : _BaseCommand() {}
void _CmdCheck::execute()
{
	_Server* server = _Server::instance();

	auto socket = server->connectionHandler()->currentSocket();
	auto connections = server->connectionData();

	if (socket->state() == QAbstractSocket::UnconnectedState)
		return;

	// checks if account logged in to the server
	if (!connections->containsSocket(socket))
		throw std::exception("Account does not connected!");

	QByteArray hash = params.data().first();

	if (hash.isEmpty()) {
		emit messageSend(socket, "/error msg=Account does not exist!");
		return;
	}

	switch (params.first())
	{
	case Parameter::Aname: 
		if (DataBase::isAccountSumExist(hash))
			emit messageSend(socket, "/info account=" + hash);

		break;
	case Parameter::Gname: 
		if (DataBase::isGroupSumExist(hash))
			emit messageSend(socket, "/info group=" + hash);

		break;
	}
}
/*
* 
*	CONNECT
*	Connects user to groups and chats
* 
*/
_CmdConnect::_CmdConnect() : _BaseCommand() {}
void _CmdConnect::execute()
{
	_Server* server = _Server::instance();

	auto handler = server->connectionHandler();
	auto connections = server->connectionData();

	auto socket = handler->currentSocket();

	if (socket->state() == QAbstractSocket::UnconnectedState)
		return;

	if (!DataBase::isAccDataAuthExist({ 
			params.joined(Parameter::Tag, "_"),
			params.joined(Parameter::Password)
		})) 
	{
		emit messageSend(socket, "/error msg=Can't login to this account");
		emit messageSend(socket, "/remove account=");

		throw std::exception("Account doesn't exist");
	}
	// if exist gets the account from db
	const _AccountData& account = DataBase::currentAccount();
	// connects socket with account (accessing commands)
	connections->addSocketToAccount(socket, DataBase::currentAccount());
	// sends data to the account
	emit messageSend(socket, "/add account=" + account.hash()
		+ " name=" + account.name()
		+ " tag=" + account.tag() 
		+ " password=" + account.pass()
	);
	// sends place holder group data
	// loads groups to this account
	for (const auto& group : DataBase::getAccountGroupList(account.hash()))
	{
		if (socket->state() == QAbstractSocket::UnconnectedState)
			return;
		emit messageSend(socket, "/add group=" + group.hash()
			+ " name=" + group.name()
		);
		connections->setupSocketToRoom(socket, account, group);
	}
}
/*
*
*	Load
*	Load data to member or group or account
*
*/
_CmdLoad::_CmdLoad() : _BaseCommand() {}
void _CmdLoad::execute()
{
	_Server* server = _Server::instance();

	auto connections = server->connectionData();
	auto socket = server->connectionHandler()->currentSocket();

	if (socket->state() == QAbstractSocket::UnconnectedState)
		return;

	// checks if account logged in to the server
	if (!connections->containsSocket(socket))
		throw std::exception("Account does not connected!");

	const _AccountData& account = connections->currentAccount();

	switch (params.first()) 
	{
	case Parameter::Aname:

		emit messageSend(socket, 
			"/loadinfo account=" + account.hash()
			+ " name=" + account.name()
			+ " phone=" + account.phone()
			+ " email=" + account.mail()
			+ " about=" + account.about()
			+ " date=" + account.strDate());

		for (const auto& img : DataBase::getAccountImages(account.hash())) {
			emit messageSend(socket, 
				"/loadimg account=" + account.hash()
				+ " pic=" + img.value("picture").toByteArray()
				+ " current=" + img.value("currentPic").toByteArray());
		}

		break;
	case Parameter::Gname:

		if (!DataBase::isGroupSumExist(params.joined(Parameter::Gname)))
			throw std::exception("Group sum does not exist");

		switch (params.last()) 
		{
		case Parameter::Aname:

			for (const auto& member : DataBase::getGroupMembers(DataBase::currentRoom().hash()))
			{
				if (socket->state() == QAbstractSocket::UnconnectedState)
					return;
				emit messageSend(socket, "/add member=" + member.hash()
					+ " name=" + member.name()
					+ " group=" + DataBase::currentRoom().hash()
					+ " tag=" + member.tag()
					+ " date=" + member.strDate()
				);
			}

			return;
		case Parameter::Message:

			for (const auto& message : DataBase::getGroupMessages(DataBase::currentRoom().hash())) 
			{
				QByteArray date = message.value(2).toString().toUtf8();

				if (socket->state() == QAbstractSocket::UnconnectedState)
					return;
 				emit messageSend(socket, "/loadinfo msg=" + message.value("msg").toByteArray()
					+ " name=" + message.value("accountName").toByteArray()
					+ " date=" + date
				);
			}

			return;
		case Parameter::Picture:

			for (const auto& picture : DataBase::getGroupImages(DataBase::currentRoom().hash()))
			{
				QByteArray date = picture.value(2).toString().toUtf8();

				if (socket->state() == QAbstractSocket::UnconnectedState)
					return;
				emit messageSend(socket, "/loadimg pic=" + picture.value("picture").toByteArray()
					+ " name=" + picture.value("accountName").toByteArray()
					+ " date=" + date
				);
			}

			return;
		}
		// loading group info on db click
		emit messageSend(socket, "/loadinfo group=" + DataBase::currentRoom().hash()
			+ " tag=" + DataBase::currentRoom().tag()
			+ " name=" + DataBase::currentRoom().name()
			+ " owner=" + DataBase::currentRoom().owner()
			+ " date=" + DataBase::currentRoom().strDate()
		);

		break;
	}

	//throw std::ex
}
/*
*
*	Send
*	Sends message to groups and chats
*
*/
_CmdSend::_CmdSend() {}
void _CmdSend::execute() 
{
	_Server* server = _Server::instance();

	auto connections = server->connectionData();
	auto socket = server->connectionHandler()->currentSocket();

	if (socket->state() == QAbstractSocket::UnconnectedState)
		return;

	// checks if account logged in to the server
	if (!connections->containsSocket(socket))
		throw std::exception("Account does not connected!");
	const auto& account = connections->currentAccount();

	if (!DataBase::isGroupSumExist(params.joined(Parameter::Gname))) {
		emit messageSend(socket, "/error msg=Group doesn't exist");
		throw std::exception("Group doesn't exist");
	}

	const auto& roomData = DataBase::currentRoom();
	auto room = connections->roomAt(roomData);

	if (!room) throw std::exception("Room is not accessible");

	const QByteArray& currentDate = QDateTime::currentDateTime()
		.toString("yyyy-MM-dd hh:mm:ss").toUtf8();

	switch (params.first())
	{
	case Parameter::Gname:

		switch (params.last())
		{
		case Parameter::Message: 

			if (socket->state() == QAbstractSocket::UnconnectedState) 
				return;

			DataBase::createInDB(DBField::Message, params);
			room->sendMessageToRoom("/add msg=" + params.joined(Parameter::Message)
				+ " group=" + roomData.hash()
				+ " name=" + account.name()
				+ " date=" + currentDate
			);
			
			break;
		case Parameter::Picture:

			if (socket->state() == QAbstractSocket::UnconnectedState) 
				return;

			DataBase::createInDB(DBField::Picture, params);
			room->sendMessageToRoom("/add pic=" + params.joined(Parameter::Picture)
				+ " group=" + roomData.hash()
				+ " name=" + account.name()
				+ " date=" + currentDate);

			break;
		}

		return;
	}

	throw std::exception("Unknown parameters were parsed");
}
/*
*
*	CREATE
*	Creates accounts, groups, chats
*
*/
_CmdCreate::_CmdCreate() : _BaseCommand() {}
void _CmdCreate::execute()
{
	_Server* server = _Server::instance();

	auto connections = server->connectionData();
	auto socket = server->connectionHandler()->currentSocket();

	if (socket->state() == QAbstractSocket::UnconnectedState) 
		return;

	switch (params.first())
	{
	if (!connections->containsSocket(socket)) {
	case Parameter::Aname:

		if (DataBase::isAccountTagExist(params.data(Parameter::Tag, "_"))) {
			emit messageSend(socket, "/error msg=Account tag already exist");
			emit messageSend(socket, "/remove account=");
			throw std::exception("Account tag already exist");
		}

		DataBase::createInDB(DBField::Account, params);

		if (!DataBase::isAccountSumExist(DataBase::currentAccount().hash())) {
			emit messageSend(socket, "/error msg=Cloudn't create account");
			emit messageSend(socket, "/remove account=");
			throw std::exception("Didn't find account after creation");
		}

		if (socket->state() == QAbstractSocket::UnconnectedState)
			return;

		emit messageSend(socket, "/info msg=Account successfully created!");
		emit messageSend(socket, "/add account=" + DataBase::currentAccount().hash()
			+ " tag=" + params.joined(Parameter::Tag, "_")
			+ " password=" + params.joined(Parameter::Password)
			+ " name=" + DataBase::currentAccount().name()
			+ " clear="
		);
	}
		return;
	case Parameter::Gname: 

		if (!DataBase::isAccountSumExist(params.joined(Parameter::Aname))) {
			emit messageSend(socket, "/error msg=You didn't log in to this account");
			emit messageSend(socket, "/remove account=");
			throw std::exception("Unknown account hash");
		}
		if (DataBase::isGroupTagExist(params.data(Parameter::Tag, "_"))) {
			emit messageSend(socket, "/error msg=Group tag already exist");
			throw std::exception("Group hash sum already exist");
		}

		if (socket->state() == QAbstractSocket::UnconnectedState)
			return;

		DataBase::createInDB(DBField::Group, params);
		connections->setupSocketToRoom(socket, DataBase::currentAccount(), DataBase::currentRoom());
		//
		emit messageSend(socket, "/info msg=Your group was successfully created!");
		emit messageSend(socket, "/add group=" + DataBase::currentRoom().hash()
			+ " name=" + DataBase::currentRoom().name());


		return;
	}
	throw std::exception("Unknown parameters were parsed");
}
/*
*
*	Change
*	Creates accounts, groups, chats
*
*/
_CmdChange::_CmdChange() : _BaseCommand() {}
void _CmdChange::execute() 
{
	_Server* server = _Server::instance();

	auto connections = server->connectionData();
	auto socket = server->connectionHandler()->currentSocket();

	// checks if account logged in to the server
	if (!connections->containsSocket(socket))
		throw std::exception("Account does not connected!");

	switch (params.first())
	{
	case Parameter::Aname:

		DataBase::changeInDB(DBField::Account, params);

		return;
	case Parameter::Gname:

		switch (params.first()) {
		case Parameter::Aname:

			DataBase::changeInDB(DBField::Participant, params);

			return;
		case Parameter::Message:

			DataBase::changeInDB(DBField::Account, params);

			return;
		}

		return;
	}

	throw std::exception("Unknown parameters were parsed");
}
/*
*
*	Delete
*	Deletes accounts, groups, chats
*
*/
_CmdDelete::_CmdDelete() : _BaseCommand() {}
void _CmdDelete::execute() 
{
	_Server* server = _Server::instance();

	auto connections = server->connectionData();
	auto socket = server->connectionHandler()->currentSocket();

	// checks if account logged in to the server
	if (!connections->containsSocket(socket))
		throw std::exception("Account does not connected!");

	switch (params.first())
	{
	case Parameter::Aname:

		DataBase::deleteInDB(DBField::Account, params);

		return;
	case Parameter::Gname:

		switch (params.first()) {
		case Parameter::Aname:

			DataBase::deleteInDB(DBField::Account, params);


			return;
		case Parameter::Message:

			DataBase::deleteInDB(DBField::Account, params);


			return;
		}

		DataBase::deleteInDB(DBField::Group, params);

		return;
	}

	throw std::exception("Unknown parameters were parsed");
}
/*
*
*	Join
*	join groups, chats
*
*/
_CmdJoin::_CmdJoin() {}
void _CmdJoin::execute() 
{
	_Server* server = _Server::instance();

	auto connections = server->connectionData();
	auto socket = server->connectionHandler()->currentSocket();

	// checks if account logged in to the server
	if (!connections->containsSocket(socket))
		throw std::exception("Account does not connected!");

	const _AccountData& account = connections->currentAccount();

	switch (params.first())
	{
	case Parameter::Tag: 

		if (!DataBase::isGroupTagExist(params.data(Parameter::Tag, "_"))) {
			emit messageSend(socket, "/error msg=Group tag does not exist");
			throw std::exception("Group does not exist");
		}
		if (DataBase::isAccountInRoom(params.joined(Parameter::Tag, "_"), params.joined(Parameter::Aname))) {
			emit messageSend(socket, "/error msg=You already joined to this group");
			throw std::exception("Account already joined to this group");
		}

		DataBase::createInDB(DBField::Participant, params);

		emit messageSend(socket, "/add group=" + DataBase::currentRoom().hash()
			+ " name=" + DataBase::currentRoom().name()
		);
		// loading members to this account
		for (const auto& member : DataBase::getGroupMembers(DataBase::currentRoom().hash()))
		{
			emit messageSend(socket, "/add member=" + member.hash()
				+ " name=" + member.name()
				+ " group=" + DataBase::currentRoom().hash()
			);
		}
		connections->roomAt(DataBase::currentRoom())->sendMessageToRoom(
			"/add member=" + account.hash()
			+ " name=" + account.name()
			+ " group=" + DataBase::currentRoom().hash()
		);
		connections->roomAt(DataBase::currentRoom())->sendMessageToRoom(
			"/add msg=joined to this room group=" + DataBase::currentRoom().hash()
			+ " name=" + account.name()
			+ " date=" + QDateTime::currentDateTime()
			.toString("dd-MM-yyyy.hh:mm:ss").toUtf8()
		);
		connections->setupSocketToRoom(socket, account, DataBase::currentRoom());

		return;
	}

	throw std::exception("Unknown parameters were parsed");
}
/*
*
*	Leave
*	Leaves from accounts, groups, chats
*
*/
_CmdLeave::_CmdLeave() {}
void _CmdLeave::execute()
{
	_Server* server = _Server::instance();

	auto connections = server->connectionData();
	auto socket = server->connectionHandler()->currentSocket();

	// checks if account logged in to the server
	if (!connections->containsSocket(socket))
		throw std::exception("Account does not connected!");

	const _AccountData& account = connections->currentAccount();

	switch (params.first())
	{
	case Parameter::Aname:

		emit messageSend(socket, "/info msg=You left this account");
		emit messageSend(socket, "/remove account=");

		connections->leaveAccount(account);

		return;
	case Parameter::Gname:

		if (!DataBase::isGroupSumExist(params.joined(Parameter::Gname))) {
			emit messageSend(socket, "/error msg=You didn't choose your group!");
			throw std::exception("Account already left this group");
		}

		DataBase::deleteInDB(DBField::Participant, params);

		emit messageSend(socket, "/remove group=" + DataBase::currentRoom().hash());
		connections->roomAt(DataBase::currentRoom())->sendMessageToRoom("/remove member=" + account.hash()
			+ " group=" + DataBase::currentRoom().hash()
		);
		connections->roomAt(DataBase::currentRoom())->sendMessageToRoom("/add msg=" + account.name() + " left this room"
			+ " group=" + DataBase::currentRoom().hash()
		);

		return;
	//case Parameter::Cname: 
	}

	throw std::exception("Unknown parameters were parsed");
}