#include "parameterdata.h"


_ParameterData::_ParameterData(){}

_ParameterData::_ParameterData(const QVector<Server::Parameter>& parameters)
{
    for (auto const& el : parameters)
        m_Parameters.insert(std::make_pair(Server::EnumToString(el), el));
}
_ParameterData::_ParameterData(const std::initializer_list<Server::Parameter>& parameters)
{
    for (auto const& el : parameters)
         m_Parameters.insert(std::make_pair(Server::EnumToString(el), el));
}

void _ParameterData::pushParsedData(Server::Parameter parameter,
    const QString& message)
{
    m_Parameters.insert(std::make_pair(message, parameter));
}

_ParameterData::_ParameterMap 
_ParameterData::mappedParameters() const
{
    return m_Parameters;
}

_ParameterData::_ParameterList
_ParameterData::toEnumListParameters() const
{
    QVector<Server::Parameter> temp;
    for (auto const& el : m_Parameters)
        temp.append(el.second);
    return temp;
}

QStringList 
_ParameterData::toStrListParameters() const
{
    QStringList temp;
    for (auto const& el : m_Parameters)
        temp.append(el.first);
    return temp;
}

bool _ParameterData::isEmpty() const
{
    return m_Parameters.empty();
}

qsizetype _ParameterData::size() const
{
    return m_Parameters.size();
}

void _ParameterData::clear()
{
    m_Parameters.clear();
}
