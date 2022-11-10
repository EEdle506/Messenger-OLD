#include "parseddata.h"

_ParsedData::_ParsedData(){}
_ParsedData::~_ParsedData(){}

void _ParsedData::addParameter(Server::Parameter parameter)
{
	for (const auto& found : m_Parameters) {
		if (found.first == parameter) {
			throw std::exception("Parameter already exist");
		}
	}
	m_Parameters.push_back({ parameter, {} });
}

void _ParsedData::addDatatoParameter(Server::Parameter parameter, const QByteArray& _data)
{	
	for (auto& found : m_Parameters) {
		if (found.first == parameter) {
			found.second.push_back(_data);
			return;
		}
	}
}

QByteArrayList _ParsedData::data(Server::Parameter parameter) const
{
	for (const auto& found : m_Parameters) {
		if (found.first == parameter) {
			return found.second;
		}
	}
	return {};
}

QByteArray _ParsedData::data(Server::Parameter parameter, const QByteArray& sep) const
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

QByteArray _ParsedData::joined(Server::Parameter parameter, const QByteArray& sep) const
{
	return data(parameter).join(sep);
}

bool _ParsedData::contains(Server::Parameter parameter) const
{
	for (const auto& [enm, str] : m_Parameters)
		if (enm == parameter)
			return true;
	return false;
}

Server::Parameter _ParsedData::first() const 
{ 
	return m_Parameters.first().first;
}

Server::Parameter _ParsedData::last() const
{
	return m_Parameters.last().first;
}

qsizetype _ParsedData::size() const
{
	return m_Parameters.size();
}
