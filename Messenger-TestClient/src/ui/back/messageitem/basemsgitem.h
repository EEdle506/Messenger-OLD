#pragma once

#include "client/clientnamespace.h"

class MsgData : public QListWidgetItem
{
    std::map <Messenger::Parameter, QByteArray> m_msgData;

public:
    MsgData(const std::map <Messenger::Parameter, QByteArray>& data, QListWidget* parent = nullptr);

    QString senderName() const;
    QString groupHash() const;
    QDateTime dateTime() const;

    std::map <Messenger::Parameter, QByteArray> msgInfo() const { 
        return m_msgData; 
    }

    bool operator<(const QListWidgetItem& other) const override;

    virtual ~MsgData() override;
};
