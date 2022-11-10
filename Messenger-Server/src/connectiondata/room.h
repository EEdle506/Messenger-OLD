#pragma once

#include "server/servernamespace.h"

class _Room
	: public QObject
{
	Q_OBJECT

	std::map <_AccountData, QTcpSocket*> m_accounts;

public:
	explicit _Room(QObject *parent = nullptr);
	~_Room() override;

	// adds account to the room
	void addAccount(const _AccountData& account);
	// removes all data about account including socket
	void removeAccount(const _AccountData& account);
	// returns socket which connected to account data
	QTcpSocket*& socketAt(const _AccountData& account);
	// 
	_AccountData accountAt(QTcpSocket* socket);
	// returns true if account exist in room	
	bool containsAccount(const _AccountData& account) const;
	bool containsSocket(QTcpSocket* socket) const;

	qsizetype socketSize() const;
	qsizetype accountSize() const;

public slots:
	void sendMessageToRoom(const QByteArray& message);

signals:
	void messageSend(QTcpSocket* socket, const QByteArray& msg);
};
