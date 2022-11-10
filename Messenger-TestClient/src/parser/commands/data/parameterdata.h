#pragma once

#include "client/clientnamespace.h"

class _ParameterData
{
public:
    using _ParameterList = QVector<Messenger::Parameter>;
    using _ParameterMap = std::map<QString, Messenger::Parameter>;

    _ParameterData();
    _ParameterData(const QVector<Messenger::Parameter>& params);
    _ParameterData(const std::initializer_list<Messenger::Parameter>& params);

private:
    _ParameterMap m_Parameters;

public:
    /*
        @param parameter - the enum type to be converted and pushed to the map (use when you have parsed the data)
        @param message - the string type to be pushed with first parameter
    */
    void pushParsedData(Messenger::Parameter parameter, const QString& message);

    _ParameterMap mappedParameters() const;
    QVector<Messenger::Parameter> toEnumListParameters() const;
    QStringList toStrListParameters() const;

    bool isEmpty() const;
    qsizetype size() const;
    void clear();
};
