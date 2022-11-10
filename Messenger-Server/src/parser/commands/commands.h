#pragma once

#include "../parseddata.h"

class _BaseCommand
	: public QObject
{
	Q_OBJECT

protected:
	_ParsedData params;
public:
 	explicit _BaseCommand(QObject* parent = nullptr);
	~_BaseCommand();

	virtual void execute() = 0;

	void setParameters(const _ParsedData& data);

signals:
	void messageSend(QTcpSocket* socket, const QByteArray& arr);
};

struct _CmdCheck final
	: public _BaseCommand
{
	_CmdCheck();
	void execute() override;
};
struct _CmdConnect final
	: public _BaseCommand
{
	_CmdConnect();
	void execute() override;
};

struct _CmdLoad final
	: public _BaseCommand
{
	_CmdLoad();
	void execute() override;
};

struct _CmdCreate final
	: public _BaseCommand
{
	_CmdCreate();
	void execute() override;
};
struct _CmdChange final
	: public _BaseCommand
{
	_CmdChange();
	void execute() override;
};
struct _CmdDelete final
	: public _BaseCommand
{
	_CmdDelete();
	void execute() override;
};

struct _CmdJoin final
	: public _BaseCommand
{
	_CmdJoin();
	void execute() override;
};
struct _CmdLeave final
	: public _BaseCommand
{
	_CmdLeave();
	void execute() override;
};

struct _CmdSend final
	: public _BaseCommand
{
	_CmdSend();
	void execute() override;
};