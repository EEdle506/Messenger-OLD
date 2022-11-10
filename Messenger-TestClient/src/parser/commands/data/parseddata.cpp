#include "parseddata.h"

_ParsedData::_ParsedData(){}
_ParsedData::~_ParsedData(){}

void _ParsedData::addParameter(Messenger::Parameter parameter)
{
    for (const auto& found : m_Parameters) {
        if (found.first == parameter) {
            return;
        }
    }
    m_Parameters.push_back({ parameter, {} });
}

void _ParsedData::addDatatoParameter(Messenger::Parameter parameter, const QByteArray& _data)
{
    for (auto& found : m_Parameters) {
        if (found.first == parameter) {
            found.second.push_back(_data);
            return;
        }
    }
}

QByteArrayList _ParsedData::data(Messenger::Parameter parameter) const
{
    for (const auto& [enm, data] : m_Parameters) {
        if (enm == parameter) {
            return data;
        }
    }
    return {};
}

QByteArray _ParsedData::data(Messenger::Parameter parameter, const QByteArray& sep) const
{
    return data(parameter).join(sep);
}

QByteArrayList _ParsedData::data() const
{
    QByteArrayList temp;
    for (const auto& el : m_Parameters)
        temp += el.second;
    return temp;
}

QByteArray _ParsedData::joined(const QByteArray& sep) const
{
    return data().join(sep);
}

QByteArray _ParsedData::joined(Messenger::Parameter parameter, const QByteArray& sep) const
{
    return data(parameter).join(sep);
}

bool _ParsedData::contains(Messenger::Parameter parameter) const
{
    for (const auto& [enm, str] : m_Parameters)
        if (enm == parameter)
            return true;
    return false;
}

Messenger::Parameter _ParsedData::first() const
{
    return m_Parameters.first().first;
}

Messenger::Parameter _ParsedData::last() const
{
    return m_Parameters.last().first;
}

std::map<Messenger::Parameter, QByteArray> _ParsedData::mappedData(const QByteArray& sep)
{
    std::map<Messenger::Parameter, QByteArray> temp;
    for (const auto& pair : m_Parameters) {
        temp.emplace(pair.first, pair.second.join(sep));
    }
    return temp;
}

qsizetype _ParsedData::size() const
{
    return m_Parameters.size();
}
