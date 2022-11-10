#pragma once

#include <QListWidgetItem>

#include "client/clientnamespace.h"

class BaseRoomData : public QListWidgetItem
{
protected:
    std::map <Messenger::Parameter, QByteArray> m_roomData;

public:
    BaseRoomData(const std::map <Messenger::Parameter, QByteArray>& data, QListWidget* parent = nullptr);
    ~BaseRoomData() override;

    QString hash() const;
    QString name() const;

    QString info(Messenger::Parameter type) const;
};
