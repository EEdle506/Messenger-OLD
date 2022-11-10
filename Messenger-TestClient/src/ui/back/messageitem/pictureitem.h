#pragma once

#include "basemsgitem.h"

class PictureItem final : 
    public MsgData
{
public:
    PictureItem(const std::map <Messenger::Parameter, QByteArray>& data, QListWidget* widget = nullptr)
        : MsgData(data, widget)
    {
        QPixmap pixmap;
        pixmap.loadFromData(msgInfo().at(Messenger::Parameter::Pic), "PNG");
        pixmap.scaled(QSize(172, 172), Qt::KeepAspectRatio);
        this->setIcon(QIcon(pixmap));
        this->setText('<' + data.at(Messenger::Parameter::Name) + '>');
        this->setTextAlignment(Qt::AlignLeft | Qt::AlignTop);
    }

    ~PictureItem() override {}
};