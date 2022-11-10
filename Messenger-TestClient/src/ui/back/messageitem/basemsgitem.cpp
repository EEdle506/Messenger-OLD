#include "basemsgitem.h"

MsgData::MsgData(const std::map<Messenger::Parameter, QByteArray>& data, QListWidget* parent)
	: QListWidgetItem(parent), m_msgData(data) {}

QString MsgData::senderName() const { return m_msgData.at(Messenger::Parameter::Account); }
QString MsgData::groupHash() const { return m_msgData.at(Messenger::Parameter::Group); }
QDateTime MsgData::dateTime() const
{
	QDateTime date = QDateTime::fromString(m_msgData.at(Messenger::Parameter::Date), "yyyy-MM-dd hh:mm:ss");

	return date;
}

bool MsgData::operator<(const QListWidgetItem& other) const
{
	const MsgData& right = static_cast<const MsgData&>(other);
	return this->dateTime() < right.dateTime();
}

MsgData::~MsgData(){}
