#include "room.h"

#include "server/server.h"

_Room::_Room(QObject* parent)
	: QObject(parent) {
	connect(this, &_Room::messageSend, &_Server::sendMessageToSocket);
}
_Room::~_Room() {}

void _Room::addAccount(const _AccountData& account)
{
	m_accounts.emplace(account, (QTcpSocket*)nullptr);
}

void _Room::removeAccount(const _AccountData& account)
{
	if (!this->containsAccount(account))
		return;
	m_accounts.erase(account);
}

QTcpSocket*& _Room::socketAt(const _AccountData& account)
{
	if (!this->containsAccount(account))
		throw std::exception("Accessing bad socket");
	return m_accounts.at(account);
}

_AccountData _Room::accountAt(QTcpSocket* socket)
{
	for (const auto& [account, sock] : m_accounts)
		if (socket == sock)
			return account;
	return {};
}

bool _Room::containsAccount(const _AccountData& account) const
{
	const auto& foundAccount = m_accounts.find(account);
	if (foundAccount == m_accounts.end())
		return false;
	return true;
}

bool _Room::containsSocket(QTcpSocket* socket) const
{
	for (const auto& s : m_accounts)
		if (s.second == socket)
			return true;
	return false;
}

qsizetype _Room::socketSize() const
{
	qsizetype counter = 0;
	for (const auto& [account, socket] : m_accounts)
		if (socket) ++counter;
	return counter;
}

qsizetype _Room::accountSize() const
{
	return m_accounts.size();
}

void _Room::sendMessageToRoom(const QByteArray& message)
{
	for (auto [account, socket] : m_accounts)
		if (socket) emit messageSend(socket, message);
}