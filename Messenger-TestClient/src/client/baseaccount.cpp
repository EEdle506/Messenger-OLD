#include "baseaccount.h"

using namespace Messenger;

BaseAccountData::BaseAccountData(const std::map<Messenger::Parameter, QByteArray>& data)
    : m_accInfo(data) {}
BaseAccountData::~BaseAccountData(){
    for (auto img : m_imgs) {
        delete img;
    }
}

void BaseAccountData::loadImage(const QByteArray &img, bool main)
{
    if (main) {
        m_imgs.push_front(new QPixmap);
        m_imgs.first()->loadFromData(img, "PNG");
        return;
    }
    m_imgs.push_back(new QPixmap);
    m_imgs.last()->loadFromData(img, "PNG");
}

void BaseAccountData::setAccountData(const std::map <Messenger::Parameter, QByteArray>& data)
{
    m_accInfo = data;
}

void BaseAccountData::addAccountData(const std::map <Messenger::Parameter, QByteArray>& info)
{
    m_accInfo.insert(info.begin(), info.end());
}

void BaseAccountData::addAccountData(Messenger::Parameter type, QByteArray info)
{
    m_accInfo.emplace(type, info);
}

void BaseAccountData::removeAccountData(Messenger::Parameter type)
{
    m_accInfo.erase(type);
}

QString BaseAccountData::accountInfo(Messenger::Parameter type)
{
    const auto& data = m_accInfo.find(type);
    if (data == m_accInfo.end())
        return {};
    return data->second;
}

QString BaseAccountData::name() const
{
    return m_accInfo.at(Parameter::Name);
}

QString BaseAccountData::hash() const
{
    return m_accInfo.at(Parameter::Account);
}

QVector<QPixmap*> BaseAccountData::images() const
{
    return m_imgs;
}

bool BaseAccountData::isEmpty() const
{
    return m_accInfo.empty();
}

void BaseAccountData::clear()
{
    m_accInfo.clear();
    for (auto img : m_imgs)
        delete img;
    m_imgs.clear();
}
