#pragma once

#include "client/clientnamespace.h"

class _ParsedData
{
    QVector <std::pair <Messenger::Parameter, QByteArrayList>> m_Parameters;

public:
	_ParsedData();
	~_ParsedData();

    void addParameter(Messenger::Parameter parameter);
    void addDatatoParameter(Messenger::Parameter parameter, const QByteArray& data);

    QByteArrayList data() const;
    QByteArrayList data(Messenger::Parameter parameter) const;
    QByteArray data(Messenger::Parameter parameter, const QByteArray& sep) const;

	QByteArray joined(const QByteArray& sep = " ") const;
    QByteArray joined(Messenger::Parameter parameter, const QByteArray& sep = " ") const;

    bool contains(Messenger::Parameter) const;

    Messenger::Parameter first() const;
    Messenger::Parameter last() const;

    std::map<Messenger::Parameter, QByteArray> mappedData(const QByteArray& sep = " ");

	qsizetype size() const;
};
