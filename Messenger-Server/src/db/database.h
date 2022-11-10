#pragma once

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "db/data/dbdata.h"
#include "db/datacrypting.h"

class _ParsedData;

namespace Server {
    
    // DataBase types
    enum DBField
    {
        Account,
        Participant,
        Group,
        Chat,
        Message,
        Picture
    };
    // Actions
    enum class AccountAction
    {
        Add,
        Delete,
        Join,
        Leave
    };
    enum class RoomAction
    {
        Add,
        Delete
    };

    class DataBase : public QObject
    {
        Q_OBJECT

        QSqlDatabase m_DB;
        QSqlQuery m_Query;

        static _AccountData m_currentAcc;
        static _RoomData m_currentRoom;

    public:
        ~DataBase() override;

        // checks if account exist in db (input: account tag or account data)
        static bool isAccountTagExist(const QByteArray& tag);
        static bool isAccDataAuthExist(const QByteArrayList& accAuth);
        static bool isAccountSumExist(const QByteArray& accSum);
        static bool isAccountInRoom(const QByteArray& tag, const QByteArray& accSum);
        // gets current account after check existance of it
        static _AccountData currentAccount();

    public:
        // checks if room exist in db (input: tag)
        static bool isGroupTagExist(const QByteArray& groupTag);
        static bool isGroupSumExist(const QByteArray& groupSum);
        // gets current room after check existance of it
        static _RoomData currentRoom();

        // TO DO ON START UP (loads rooms)
        static void loadRoomsFromDB(DBField roomType);

    private:
        static void setCurrentAccount(const int& id);
        static void setCurrentRoom(const int& id);

    public slots:
        // 
        static QVector <QSqlRecord> getAccountImages(const QByteArray& accSum);
        //
        static QVector <_RoomData> getAccountGroupList(const QByteArray& accSum);
        // 
        static _RoomData getGroupData(const QByteArray& roomSum);
        // 
        static QVector <_AccountData> getGroupMembers(const QByteArray& roomSum);
        // group messages and data
        static QVector<QSqlRecord> getGroupImages(const QByteArray& roomSum);
        static QVector <QSqlRecord> getGroupMessages(const QByteArray& roomSum);

        // Creates instance in db
        static void createInDB(DBField what, const _ParsedData& data);
        //
        static void changeInDB(DBField what, const _ParsedData& data);
        // Deletes instance in db
        static void deleteInDB(DBField what, const _ParsedData& data);
   
    public:
        // static instance of db
        static DataBase* instance();
        // connects to DB
        static void Connect();

    private:
        explicit DataBase(QObject* parent = nullptr);

    signals:
        void accountAction(AccountAction action, const _AccountData& account, const _RoomData& room = _RoomData());
        void roomAction(RoomAction action, const _RoomData& roomData);

    private slots:
        static QVector <QSqlRecord> runQuery(const QString& query, const QByteArrayList& data = QByteArrayList(),
            QSql::ParamType type = QSql::In);

    public:
        void operator=(DataBase&) = delete;
        DataBase(DataBase&) = delete;

    private:
        static DataBase* m_instance;
    };
}
