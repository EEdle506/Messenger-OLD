#include "datacrypting.h"

#include <QByteArray>

_DataCrypting::_DataCrypting()
{
	m_Hash = new QCryptographicHash(QCryptographicHash::Algorithm::Sha256);
}

_DataCrypting::~_DataCrypting()
{
	delete m_Hash;
}

_DataCrypting* _DataCrypting::instance()
{
	if (!m_instance) m_instance = new _DataCrypting();
	return m_instance;
}

QByteArray _DataCrypting::encrypt(const QByteArray& message)
{
	auto crypter = _DataCrypting::instance();
	crypter->m_Hash->reset();
	crypter->m_Hash->addData(message.data(), message.size());
	return crypter->m_Hash->result();
}

QString _DataCrypting::encrypt(const QString& message)
{
	auto crypter = _DataCrypting::instance();
	crypter->m_Hash->reset();
	crypter->m_Hash->addData(message.toUtf8().data(), message.toUtf8().size());
	return crypter->m_Hash->result().toHex();
}

_DataCrypting* _DataCrypting::m_instance = nullptr;