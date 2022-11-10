#pragma once

#include <QObject>

#include <QCryptographicHash>

class _DataCrypting : public QObject
{
	Q_OBJECT

	QCryptographicHash* m_Hash = nullptr;

public:
	~_DataCrypting();

	static _DataCrypting* instance();

public slots:
	static QByteArray encrypt(const QByteArray& message);
	static QString encrypt(const QString& message);

private:
	_DataCrypting();
	static _DataCrypting* m_instance;
};
