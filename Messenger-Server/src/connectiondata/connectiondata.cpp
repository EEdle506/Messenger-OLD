#include "connectiondata.h"

#include "server/server.h"

_ConnectionData::_ConnectionData(QObject *parent)
    : QObject{parent} {
    connect(this, &_ConnectionData::messageSend, &_Server::sendMessageToSocket);
}
_ConnectionData::~_ConnectionData() {}

bool _ConnectionData::containsSocket(QTcpSocket* socket) const
{
    if (!socket) return false;

    for (const auto& [account, sock] : m_ConnectedAccounts) {
        if (sock == socket) {
            m_currentAccount = account;
            return true;
        }
    }
    return false;
}

_AccountData _ConnectionData::currentAccount() const
{
    return m_currentAccount;
}

_Room* _ConnectionData::roomAt(const _RoomData& roomInfo) const
{
    const auto& room = m_Rooms.find(roomInfo);
    if (room == m_Rooms.end())
        return nullptr;
    return room->second;
}

QVector <_Room*> 
_ConnectionData::roomsAt(const _AccountData& account) const
{
    QVector<_Room*> temp;
    for (const auto& [roomData, room] : m_Rooms)
        if (room->containsAccount(account))
            temp.push_back(room);
    return temp;
}

QVector <_RoomData> 
_ConnectionData::roomsDataAt(const _AccountData& account) const
{
    QVector<_RoomData> temp;
    for (const auto& [roomData, room] : m_Rooms)
        if (room->containsAccount(account))
            temp.push_back(roomData);
    return temp;
}

QTcpSocket*& 
_ConnectionData::socketAt(const _AccountData& account)
{
    const auto& findAccount = m_ConnectedAccounts.find(account);
    if (findAccount == m_ConnectedAccounts.end())
        throw std::exception("Accessing unexisted account socket");
    return findAccount->second;
}

void _ConnectionData::addSocketToAccount(QTcpSocket* socket, const _AccountData& account) {
    const auto& findAcc = m_ConnectedAccounts.find(account);
    // Setups socket to connected accounts
    if (findAcc == m_ConnectedAccounts.end())
        m_ConnectedAccounts.emplace(account, socket);
    else m_ConnectedAccounts.at(findAcc->first) = socket;
}

void _ConnectionData::setupSocketToRoom(QTcpSocket* socket, const _AccountData& account, const _RoomData& room)
{
    auto roomPtr = roomAt(room);
    if (!roomPtr) throw std::exception("Accessing unexisting room.");

    try {
        roomPtr->socketAt(account) = socket;
    }
    catch (const std::exception& e){
        qCritical() << "[ERROR]" << e.what();
        return;
    }

    roomPtr->sendMessageToRoom(QString("/online group=" + room.hash()
        + " member=" + account.hash()
        + " date=" + QDateTime::currentDateTime().toString("dd-MM-yyyy.hh:mm:ss")
    ).toUtf8());
}

void _ConnectionData::leaveAccount(const _AccountData& account)
{
    m_ConnectedAccounts.erase(account);

    for (auto& room : roomsAt(account))
        room->socketAt(account) = nullptr;
}

void _ConnectionData::onSocketDisconnected(QTcpSocket* socket_)
{
    _AccountData account;

    for (const auto& [acc, socket] : m_ConnectedAccounts) {
        if (socket == socket_) {
            account = acc;
            break;
        }
    }

    for (const auto& [roomData, room] : m_Rooms) 
    {
        if (!room->containsSocket(socket_))
            continue;

        auto participant = room->accountAt(socket_);
        room->sendMessageToRoom(QString("/offline group=" + roomData.hash()
            + " member=" + account.hash()
            + " date=" + QDateTime::currentDateTime().toString("dd-MM-yyyy.hh:mm:ss")
        ).toUtf8());
        auto sock = socketAt(participant);
        if (sock) sock->deleteLater();
    } 
    m_ConnectedAccounts.erase(account);
    qInfo() << "[INFO] Socket was disconnected from the queue.";
}

void _ConnectionData::onAccountAction(Server::AccountAction action,
    const _AccountData& account, const _RoomData& roomData)
{
    switch (action)
    {
    case Server::AccountAction::Add: m_ConnectedAccounts.emplace(account, (QTcpSocket*)nullptr);
        break;
    case Server::AccountAction::Delete: m_ConnectedAccounts.erase(account);
        break;
    case Server::AccountAction::Join: m_Rooms.at(roomData)->addAccount(account);
        break;
    case Server::AccountAction::Leave: m_Rooms.at(roomData)->removeAccount(account);
        break;
    }
}
void _ConnectionData::onRoomAction(Server::RoomAction action, const _RoomData& roomData)
{
    switch (action)
    {
    case Server::RoomAction::Add:  m_Rooms.emplace(roomData, new _Room());
        break;
    case Server::RoomAction::Delete: // Deletes room

        m_Rooms.at(roomData)->deleteLater();
        m_Rooms.erase(roomData);

        break;
    }
}