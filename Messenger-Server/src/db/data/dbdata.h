#pragma once

#include <QVariant>
#include <QSqlRecord>

class _AccountData
{
protected:
	QSqlRecord m_accountData;

public:
	_AccountData(const QSqlRecord& accRecord = {});

	QSqlRecord& record();

	virtual int id() const;
	virtual QByteArray strID() const;

	QByteArray strDate() const;

	QByteArray tag() const;
	QByteArray name() const;
	QByteArray phone() const;
	QByteArray mail() const;
	QByteArray about() const;
	QByteArray pass() const;
	QByteArray hash() const;

	void clear();

	virtual operator int() const {
		return id();
	}
};

//class _ParticipantData : public _AccountData 
//{
//public:
//	_ParticipantData(const QSqlRecord& accRecord = {});
//
//	int id() const override;
//	QString strID() const override;
//
//	QString groupID() const;
//
//	QString joinDate() const;
//
//	operator int() const override {
//		return id();
//	}
//};

class _RoomData
{
protected:
	QSqlRecord m_roomData;

public:
	_RoomData(const QSqlRecord& roomRecord = {});

	QSqlRecord& record();

	virtual int id() const;
	virtual QByteArray strID() const;

	QByteArray strDate() const;

	QByteArray tag() const;
	QByteArray name() const;
	QByteArray owner() const;

	QByteArray hash() const;

	void clear();

	virtual operator int() const {
		return id();
	}
};
