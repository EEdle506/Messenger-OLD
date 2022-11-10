#pragma once

#include "data/parseddata.h"

class _BaseCommand
        : public QObject
{
    Q_OBJECT

protected:
	_ParsedData params;

public:
    explicit _BaseCommand(QObject* parent = nullptr);
    ~_BaseCommand() override;

	virtual void execute() = 0;

	void setParameters(const _ParsedData& data);
};

/*
*
*   
* 
*/

struct _CmdExist : public _BaseCommand
{
    _CmdExist();
    void execute() override;
};

struct _CmdWindowsInfo : public _BaseCommand
{
    _CmdWindowsInfo();
    void execute() override;
};
struct _CmdInfo : public _BaseCommand
{
    _CmdInfo();
    void execute() override;
};
struct _CmdWarning : public _BaseCommand
{
    _CmdWarning();
    void execute() override;
};
struct _CmdError : public _BaseCommand
{
    _CmdError();
	void execute() override;
};

struct _CmdLoadInfo : public _BaseCommand
{
    _CmdLoadInfo();
	void execute() override;
};
struct _CmdLoadImg : public _BaseCommand
{
    _CmdLoadImg();
    void execute() override;
};

struct _CmdAdd : public _BaseCommand
{
    _CmdAdd();
    void execute() override;
};
struct _CmdRemove : public _BaseCommand
{
    _CmdRemove();
    void execute() override;
};

struct _CmdOnline : public _BaseCommand
{
    _CmdOnline();
    void execute() override;
};
struct _CmdOffline : public _BaseCommand
{
    _CmdOffline();
    void execute() override;
};
