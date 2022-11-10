#pragma once

#include "clientnamespace.h"

class BaseAccountData
{
protected:
    // Account data
    QVector<QPixmap*> m_imgs;
    std::map<Messenger::Parameter, QByteArray> m_accInfo;

public:
    BaseAccountData() = default;
    BaseAccountData(const std::map<Messenger::Parameter, QByteArray>& data);
    virtual ~BaseAccountData();

    void loadImage(const QByteArray& img, bool main);

    QString accountInfo(Messenger::Parameter type);

    void setAccountData(const std::map<Messenger::Parameter, QByteArray>& data);

    void addAccountData(Messenger::Parameter type, QByteArray info);
    void addAccountData(const std::map <Messenger::Parameter, QByteArray>& info);

    void removeAccountData(Messenger::Parameter type);

    QString name() const;
    QString hash() const;
    QVector<QPixmap*> images() const;

    bool isEmpty() const;
    void clear();
};
