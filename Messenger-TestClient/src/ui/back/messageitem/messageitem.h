#pragma once

#include "basemsgitem.h"

class MessageItem final :
    public MsgData
{
public:
    MessageItem(const std::map <Messenger::Parameter, QByteArray>& data, QListWidget* widget = nullptr)
        : MsgData(data, widget)
    {
        this->setText("<" + data.at(Messenger::Parameter::Name) + "> " + data.at(Messenger::Parameter::Msg));
    }

    ~MessageItem() override {}
};
