#include "database.h"

#include "parser/parseddata.h"

namespace Server {
    DataBase::DataBase(QObject* parent)
        : QObject{ parent }, m_DB(QSqlDatabase::addDatabase("QMYSQL"))
        , m_Query(m_DB) {}
    DataBase::~DataBase() { m_DB.close(); }

    void DataBase::createInDB(DBField what, const _ParsedData& params)
    {
        QByteArray hash = _DataCrypting::encrypt(params.joined()).toHex();

        switch (what)
        {
        case Account:

            runQuery("insert into account(accountTag, accountName, accountPass, accountHashSum) values(?,?,?,?)", {
                params.data(Parameter::Tag, "_"),
                params.joined(Parameter::Aname),
                params.joined(Parameter::Password),
                hash
            }, QSql::Binary | QSql::In);

            if (!isAccountSumExist(hash))
                throw std::exception("Account (table) can not insert data!");

            runQuery("insert into account_phone(accountID, phoneNumber, phoneCountry, phoneCountryNum) values(?,?,?,?)", {
                // account id
                currentAccount().strID(),
                // account phone
                params.data(Parameter::Phone).first(),
                // account country
                params.data(Parameter::Phone)[1],
                // account country number
                params.data(Parameter::Phone).last()
            }, QSql::Binary | QSql::In);

            qDebug() << "[TYPES] Account: " << currentAccount().strID();

            emit DataBase::instance()->accountAction(AccountAction::Add, currentAccount());

            break;
        case Group:

            runQuery("insert into group_info(groupTag, groupName, groupOwnerSum, groupHashSum) values(?,?,?,?)", {
                params.data(Parameter::Tag, "_"),
                params.joined(Parameter::Gname),
                params.joined(Parameter::Aname),
                hash
            }, QSql::Binary | QSql::In);

            if (!DataBase::isGroupSumExist(hash))
                throw std::exception("Didn't find room after creation");

            qDebug() << "[TYPES] Account: " << currentAccount().strID()
                << "Room: " << currentRoom().strID();

            emit DataBase::instance()->roomAction(RoomAction::Add, currentRoom());
            emit DataBase::instance()->accountAction(AccountAction::Join, currentAccount(), currentRoom());

            break;
        case Chat:
            break;
        case Participant:
 
            runQuery("insert into group_participant(groupID, accountID) values(?,?)", {
                currentRoom().strID(),
                currentAccount().strID()
            }, QSql::Binary | QSql::In);

            runQuery("insert into account_grouplist(accountID, groupHashSum) values(?,?)", {
                currentAccount().strID(),
                currentRoom().hash()
            }, QSql::Binary | QSql::In);

            qDebug() << "[TYPES] Account: " << currentAccount().strID()
                << "Room: " << currentRoom().strID();

            emit DataBase::instance()->accountAction(AccountAction::Join, currentAccount(), currentRoom());

            break;
        case Message:

            runQuery("insert into group_messages(participantID, msg) values((select ID from group_participant where groupID=? and accountID=?),?)", {
                DataBase::getGroupData(params.joined(Parameter::Gname)).strID(),
                currentAccount().strID(),
                params.joined(Parameter::Message)
            }, QSql::Binary | QSql::In);

            break;
        case Picture:

            runQuery("insert into group_pictures(participantID, picture) values((select ID from group_participant where groupID=? and accountID=?),?)", {
                DataBase::getGroupData(params.joined(Parameter::Gname)).strID(),
                currentAccount().strID(),
                params.joined(Parameter::Picture)
            }, QSql::Binary | QSql::In);
            
            break;
        }
    }
    void DataBase::changeInDB(DBField what, const _ParsedData& params)
    {
        QByteArray hash = _DataCrypting::encrypt(params.joined()).toHex();

        switch (what)
        {
        case Account:
            break;
        case Group:
            break;
        case Chat:
            break;
        case Participant:
            break;
        case Message:
            break;
        case Picture:
            break;
        }
    }
    void DataBase::deleteInDB(DBField what, const _ParsedData& params)
    {
        QByteArray hash = _DataCrypting::encrypt(params.joined()).toHex();

        switch (what)
        {
        case Account:
            break;
        case Group:
            break;
        case Chat:
            break;
        case Participant:

            if (!isGroupSumExist(params.joined(Parameter::Gname)))
                return;
            if (!isAccountSumExist(params.joined(Parameter::Aname)))
                return;

            runQuery("DELETE FROM group_participant WHERE groupID=? AND accountID=?", {
                m_currentRoom.strID(),
                m_currentAcc.strID()
            });

            emit DataBase::instance()->accountAction(AccountAction::Leave, currentAccount(), currentRoom());

            break;
        case Message:
            break;
        case Picture:
            break;
        }
    }

    void DataBase::Connect()
    {
        auto db = instance();

        qDebug() << "[INFO] Connectig to DataBase...";

        db->m_DB.setDatabaseName("dbservermsg");
        db->m_DB.setHostName("localhost");
        db->m_DB.setPort(32455);
        db->m_DB.setUserName("root");
        db->m_DB.setPassword("8f480053a61faaf5526f8d0622adac8d235402e3483bfc6d2331e91d577a12ca");

        if (db->m_DB.open()) 
            qDebug() << "[INFO] DataBase connected!";
        else {
            qDebug() << "[ERROR]" << db->m_DB.lastError().databaseText();
            exit(-1);
        }
    }

    void DataBase::loadRoomsFromDB(DBField roomType)
    {
        static qsizetype counter = 0;
        QString roomInfoQuery;
        // setting room type
        switch (roomType)
        {
        case Group:
            roomInfoQuery = "select group_info.*, DATE_FORMAT(group_info.groupCreationDate, '%Y-%m-%d %T') from group_info";
            break;
        case Chat:
            break;
        }
        // getting rooms from db
        const auto& rooms = runQuery(roomInfoQuery, {}, QSql::Out);

        qInfo() << "[INFO] [PART "<< counter <<"] Rooms to load " << rooms.size();

        // setting room data and adding to connection data
        for (const _RoomData& roomInfo : rooms)
        {
            // adds room to data
            emit DataBase::instance()->roomAction(RoomAction::Add, roomInfo);
            // loads all accounts to the room
            for (const _AccountData& participant : 
                runQuery("SELECT account.*, account_phone.*, group_participant.* from account "
                    "INNER JOIN group_participant ON group_participant.accountID = account.accountID "
                    "INNER JOIN account_phone ON account.accountID = account_phone.accountID "
                    "WHERE group_participant.groupID=?", {
                        roomInfo.strID()
                    }, QSql::Out)
                )
            {
                qDebug() << "[TYPES] Loading Account: " << participant.strID()
                    << "Room: " << roomInfo.strID();
                emit DataBase::instance()->accountAction(AccountAction::Join, participant, roomInfo);
            }
        }
        ++counter;
    }

    QVector <QSqlRecord> DataBase::getAccountImages(const QByteArray& accSum)
    {
        if (!isAccountSumExist(accSum))
            return {};

         return runQuery("select picture, currentPic from account_images "
             "INNER JOIN account ON account_images.accountID=account.accountID "
             "WHERE accountHashSum=?", 
         {
            accSum
         }, QSql::Binary | QSql::Out);
    }
    QVector <_RoomData> DataBase::getAccountGroupList(const QByteArray& accSum)
    {
        if (!isAccountSumExist(accSum))
            return {};

        const auto& groupSumList = runQuery("select groupHashSum from account_grouplist where accountID=?", {
            m_currentAcc.strID()
        }, QSql::Binary | QSql::Out);

        QVector <_RoomData> temp;
        for (const _RoomData& groupSum : groupSumList) 
        {
            const _RoomData& group = runQuery("select groupID, groupName, groupHashSum "
                "from group_info where groupHashSum=?", {
                groupSum.hash()
            }, QSql::Binary | QSql::Out).constFirst();

            qDebug() << "[TYPE] Loading Room: " << group.strID();
            temp.push_back(group);
        }
        return temp;
    }

    _RoomData DataBase::getGroupData(const QByteArray& roomSum)
    {
        if (!isGroupSumExist(roomSum))
            return {};
        return m_currentRoom;
    }
    QVector<_AccountData> DataBase::getGroupMembers(const QByteArray& roomSum)
    {
        if (!isGroupSumExist(roomSum))
            return {};

        const auto& accounts = runQuery("SELECT account.*, group_participant.* FROM account "
            "LEFT JOIN group_participant ON account.accountID=group_participant.accountID "
            "LEFT JOIN group_info ON group_participant.groupID=group_info.groupID "
            "WHERE group_info.groupID=?", 
        {
            m_currentRoom.strID()
        }, QSql::Binary | QSql::Out);

        return QVector<_AccountData>(accounts.begin(), accounts.end());
    }

    QVector <QSqlRecord> DataBase::getGroupImages(const QByteArray& roomSum)
    {
        if (!isGroupSumExist(roomSum))
            return {};

        const QVector <QSqlRecord>& images = runQuery("SELECT account.accountName, group_pictures.picture, "
            "DATE_FORMAT(group_pictures.picDate, '%Y-%m-%d %T') from group_pictures "
            "RIGHT JOIN group_participant on group_pictures.participantID = group_participant.ID "
            "LEFT JOIN account on group_participant.accountID = account.accountID "
            "LEFT JOIN group_info on group_info.groupID = group_participant.groupID "
            "WHERE group_info.groupID=? and picture IS NOT NULL",
            {
                m_currentRoom.strID()
            }, QSql::Binary | QSql::Out);

        return images;
    }
    QVector <QSqlRecord> DataBase::getGroupMessages(const QByteArray& roomSum)
    {
        if (!isGroupSumExist(roomSum))
            return {};

        const QVector <QSqlRecord>& messages = runQuery("SELECT account.accountName, group_messages.msg, "
            "DATE_FORMAT(group_messages.msgDate, '%Y-%m-%d %T') from group_messages "
            "RIGHT JOIN group_participant on group_messages.participantID = group_participant.ID "
            "LEFT JOIN account on group_participant.accountID = account.accountID "
            "LEFT JOIN group_info on group_info.groupID = group_participant.groupID "
            "WHERE group_info.groupID=? and msg IS NOT NULL",
            {
                m_currentRoom.strID()
            }, QSql::Out);
        return messages;
    }

    bool DataBase::isAccountTagExist(const QByteArray& accTag)
    {
        if (accTag.isEmpty())
            return false;

        const auto& accounts = runQuery("SELECT accountID FROM account WHERE accountTag=?", {
            accTag
        }, QSql::Out);
        if (accounts.isEmpty())
            return false;

        const auto& account = accounts.first();
        if (account.isEmpty())
            return false;

        setCurrentAccount(account.value(0).toInt());

        return true;
    }
    bool DataBase::isAccDataAuthExist(const QByteArrayList& accAuth)
    {
        if (accAuth.size() != 2)
            return false;
        if (accAuth.first().isEmpty() && accAuth.last().isEmpty())
            return false;

        const auto& accounts = runQuery("SELECT account.accountID from account "
            "LEFT JOIN group_participant ON group_participant.accountID = account.accountID "
            "LEFT JOIN account_phone ON account.accountID = account_phone.accountID "
            "where accountTag=? and accountPass=?", {
            accAuth.first(),
            accAuth.last()
        }, QSql::Out);

        if (accounts.isEmpty())
            return false;

        const auto& account = accounts.first();
        if (account.isEmpty())
            return false;

        setCurrentAccount(account.value(0).toInt());

        return true;
    }
    bool DataBase::isAccountSumExist(const QByteArray& accSum)
    {
        if (accSum.isEmpty())
            return false;

        const auto& accounts = runQuery("SELECT accountID from account where accountHashSum=?", {  
            accSum
        }, QSql::Out);
        if (accounts.isEmpty())
            return false;

        const auto& account = accounts.first();
        if (account.isEmpty())
            return false;

        setCurrentAccount(account.value(0).toInt());

        return true;
    }
    bool DataBase::isAccountInRoom(const QByteArray& groupTag, const QByteArray& accSum)
    {
        if (groupTag.isEmpty())
            return false;
        if (accSum.isEmpty())
            return false;
        if (!isGroupTagExist(groupTag))
            return false;
        if (!isAccountSumExist(accSum))
            return false;

        const auto& accounts = runQuery("SELECT accountHashSum FROM account "
            "LEFT JOIN group_participant ON group_participant.accountID = account.accountID "
            "LEFT JOIN account_phone ON account.accountID = account_phone.accountID "
            "where account.accountID=? and group_participant.groupID=?", {
                m_currentAcc.strID(),
                m_currentRoom.strID()
            }, QSql::Out);
        if (accounts.isEmpty())
            return false;

        const auto& account = accounts.first();
        if (account.isEmpty())
            return false;
       
        return true;
    }

    bool DataBase::isGroupTagExist(const QByteArray& tag)
    {
        if (tag.isEmpty())
            return false;

        const auto& groups = runQuery("SELECT groupID FROM group_info WHERE groupTag=?", { tag }, QSql::Binary | QSql::Out);
        if (groups.isEmpty())
            return false;

        const auto& group = groups.first();
        if (group.isEmpty())
            return false;

        setCurrentRoom(group.value(0).toInt());

        return true;
    }
    bool DataBase::isGroupSumExist(const QByteArray& groupSum)
    {
        if (groupSum.isEmpty())
            return false;

        const auto& groups = runQuery("SELECT groupID FROM group_info WHERE groupHashSum=?", { 
                groupSum
        }, QSql::Out);
        if (groups.isEmpty())
            return false;

        const auto& group = groups.first();
        if (group.isEmpty())
            return false;

        setCurrentRoom(group.value(0).toInt());

        return true;
    }
    
    _AccountData DataBase::currentAccount(){
        return m_currentAcc;
    }
    _RoomData DataBase::currentRoom() {
        return m_currentRoom;
    }

    void DataBase::setCurrentAccount(const int& accID)
    {
        const auto& accounts = runQuery("SELECT account.*, DATE_FORMAT(account.accountDate, '%Y-%m-%d %T') as adate, account_phone.*, "
            "group_participant.*, DATE_FORMAT(group_participant.joinDate, '%Y-%m-%d %T') as jdate from account "
            "LEFT JOIN group_participant ON group_participant.accountID = account.accountID "
            "LEFT JOIN account_phone ON account.accountID = account_phone.accountID "
            "where account.accountID=?", {
                QByteArray::number(accID)
            }, QSql::Out);
        if (accounts.isEmpty()) 
            return;

        const _AccountData& account = accounts.first();
        m_currentAcc = account;
    }
    void DataBase::setCurrentRoom(const int& roomID)
    {
        const auto& groups = runQuery("SELECT group_info.*, DATE_FORMAT(group_info.groupCreationDate, '%Y-%m-%d %T') as cdate FROM group_info WHERE groupID=?", {
                QByteArray::number(roomID)
        }, QSql::Out);

        const _RoomData& group = groups.first();
        m_currentRoom = group;
    }

    /*
    *
    *   Queries
    *
    */
    QVector <QSqlRecord> DataBase::runQuery(const QString& query, const QByteArrayList& data, QSql::ParamType type)
    {
        auto db = DataBase::instance();

        if (query.isEmpty()) 
            return {};

        db->m_Query.prepare(query);

        int m_paramCounter = 0;
        for (const auto& el : data) {
            db->m_Query.bindValue(m_paramCounter, el, type);
            ++m_paramCounter;
        }

        if (!db->m_Query.exec()) {
            qWarning() << "[QUERY] Info " << db->m_Query.lastError().databaseText();
            return {};
        }

        QVector<QSqlRecord> records;
        while (db->m_Query.next())
            records.push_back(db->m_Query.record());

        return records;
    }

    DataBase* DataBase::instance()
    {
        if (!m_instance) m_instance = new DataBase();
        return m_instance;
    }
}

Server::DataBase* Server::DataBase::m_instance{ nullptr };

_RoomData Server::DataBase::m_currentRoom{};
_AccountData Server::DataBase::m_currentAcc{};