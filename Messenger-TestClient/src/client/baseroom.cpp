#include "baseroom.h"

BaseRoomData::BaseRoomData(const std::map<Messenger::Parameter, QByteArray>& data, QListWidget* parent)
	: QListWidgetItem(parent), m_roomData(data) {
	this->setText(this->name());
}
BaseRoomData::~BaseRoomData() {
	m_roomData.clear();
}

QString BaseRoomData::hash() const
{
	return m_roomData.at(Messenger::Parameter::Group);
}

QString BaseRoomData::name() const
{
	return m_roomData.at(Messenger::Parameter::Name);
}

QString BaseRoomData::info(Messenger::Parameter type) const
{
	return m_roomData.at(type);
}
