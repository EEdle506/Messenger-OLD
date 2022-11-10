#pragma once

#include "room.h"

class _ConnectionData
    : public QObject
{
    Q_OBJECT

    std::map <_AccountData, QTcpSocket*> m_ConnectedAccounts;
    
    mutable _AccountData m_currentAccount;

public:
    explicit _ConnectionData(QObject *parent = nullptr);
    ~_ConnectionData() override;

    // checks if socket connected to account
    bool containsSocket(QTcpSocket* socket) const;
    // return currnet socket before contains method
    _AccountData currentAccount() const;
    // returns socket which was mapped by account info (error prone)
    QTcpSocket*& socketAt(const _AccountData& data);
    // connects socket with account 
    void addSocketToAccount(QTcpSocket* socket, const _AccountData& account);
    // injects socket to specified room
    void setupSocketToRoom(QTcpSocket* socket, const _AccountData& account, const _RoomData& room);

    void leaveAccount(const _AccountData& account);

private:
    std::map <_RoomData, _Room*> m_Rooms;

public:
    // finds room which was mapped by room info
    _Room* roomAt(const _RoomData& room) const;
    // finds all rooms by account
    QVector <_Room*> roomsAt(const _AccountData& account) const;
    // finds all rooms data by account
    QVector <_RoomData> roomsDataAt(const _AccountData& account) const;

public slots:
    // 
    void onSocketDisconnected(QTcpSocket* socket);
    //
    void onAccountAction(Server::AccountAction action, 
        const _AccountData& account, const _RoomData& roomInfo = _RoomData());
    //
    void onRoomAction(Server::RoomAction action,
        const _RoomData& roomData);

signals:
    void messageSend(QTcpSocket* socket, const QByteArray& arr);
};
