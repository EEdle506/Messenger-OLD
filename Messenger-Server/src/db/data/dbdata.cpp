#include "dbdata.h"

#include <QDate>

_AccountData::_AccountData(const QSqlRecord& accRecord)
    : m_accountData(accRecord) {}

QSqlRecord& _AccountData::record()
{
    return m_accountData;
}

int _AccountData::id() const
{
    return m_accountData.value("accountID").toInt();
}

QByteArray _AccountData::strID() const
{
    return m_accountData.value("accountID").toByteArray();
}

QByteArray _AccountData::tag() const
{
    return m_accountData.value("accountTag").toByteArray();
}

QByteArray _AccountData::name() const
{
    return m_accountData.value("accountName").toByteArray();
}

QByteArray _AccountData::phone() const
{
    QByteArray temp = m_accountData.value("phoneNumber").toByteArray()
                  + ' ' + m_accountData.value("phoneCountry").toByteArray()
                  + ' ' + m_accountData.value("phoneCountryNum").toByteArray();
    return temp;
}

QByteArray _AccountData::mail() const
{
    return m_accountData.value("accountEmail").toByteArray();
}

QByteArray _AccountData::about() const
{
    return m_accountData.value("accountAbout").toByteArray();
}

QByteArray _AccountData::pass() const
{
    return m_accountData.value("accountPass").toByteArray();
}

QByteArray _AccountData::hash() const
{
    return m_accountData.value("accountHashSum").toByteArray();
}

QByteArray _AccountData::strDate() const
{
    return m_accountData.value("adate").toString().toUtf8();
}

void _AccountData::clear()
{
    m_accountData.clear();
}


/*
*
* 
* 
*/


_RoomData::_RoomData(const QSqlRecord& roomRecord)
    : m_roomData(roomRecord) {}
QSqlRecord& _RoomData::record()
{
    return m_roomData;
}

int _RoomData::id() const
{
    return m_roomData.value("groupID").toInt();
}

QByteArray _RoomData::strID() const
{
    return m_roomData.value("groupID").toByteArray();
}

QByteArray _RoomData::strDate() const
{
    QString date = m_roomData.value("cdate").toString();
    return date.toUtf8();
}

QByteArray _RoomData::tag() const
{
    return m_roomData.value("groupTag").toByteArray();
}

QByteArray _RoomData::name() const
{
    return m_roomData.value("groupName").toByteArray();
}

QByteArray _RoomData::owner() const
{
    return m_roomData.value("groupOwnerSum").toByteArray();
}

QByteArray _RoomData::hash() const
{
    return m_roomData.value("groupHashSum").toByteArray();
}

void _RoomData::clear()
{
    m_roomData.clear();
}