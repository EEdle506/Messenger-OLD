#pragma once

#include "server/servernamespace.h"

class _ParsedData
{
	QVector <std::pair <Server::Parameter, QByteArrayList>> m_Parameters;

public:
	_ParsedData();
	~_ParsedData();

	void addParameter(Server::Parameter parameter);
	void addDatatoParameter(Server::Parameter parameter, const QByteArray& data);

	QByteArrayList data() const;
	QByteArrayList data(Server::Parameter parameter) const;
	QByteArray data(Server::Parameter parameter, const QByteArray& sep) const;

	QByteArray joined(const QByteArray& sep = " ") const;
	QByteArray joined(Server::Parameter parameter, const QByteArray& sep = " ") const;

	bool contains(Server::Parameter) const;

	Server::Parameter first() const;
	Server::Parameter last() const;

	qsizetype size() const;
};
