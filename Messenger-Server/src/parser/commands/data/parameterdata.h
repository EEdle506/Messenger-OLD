#pragma once

#include "server/servernamespace.h"

class _ParameterData
{
public:
    using _ParameterList = QVector<Server::Parameter>;
    using _ParameterMap = std::map<QString, Server::Parameter>;

    _ParameterData();
    _ParameterData(const QVector<Server::Parameter>& params);
    _ParameterData(const std::initializer_list<Server::Parameter>& params);

private:
    _ParameterMap m_Parameters;

public:
    /*
        @param parameter - the enum type to be converted and pushed to the map (use when you have parsed the data)
        @param message - the string type to be pushed with first parameter
    */
    void pushParsedData(Server::Parameter parameter, const QString& message);

    _ParameterMap 
    mappedParameters() const;
    QVector<Server::Parameter> 
    toEnumListParameters() const;
    QStringList toStrListParameters() const;

    bool isEmpty() const;
    qsizetype size() const;
    void clear();
};