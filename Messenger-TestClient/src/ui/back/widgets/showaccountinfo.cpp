#include "showaccountinfo.h"
#include "ui_showaccountinfo.h"

using namespace Messenger;

ShowAccountInfo::ShowAccountInfo(BaseAccountData* account, QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::ShowAccountInfo())
	, m_account(account)
{
	ui->setupUi(this);
}

ShowAccountInfo::~ShowAccountInfo()
{
	delete ui;
}

void ShowAccountInfo::on_loadImage_clicked()
{
}

void ShowAccountInfo::on_changeButton_clicked()
{
}

void ShowAccountInfo::on_hideButton_clicked()
{
	hide();
}

void ShowAccountInfo::on_refreshButton_clicked()
{
	ui->tagLine->setText(m_account->accountInfo(Parameter::Tag));
	ui->nameLine->setText(m_account->accountInfo(Parameter::Name));
	ui->emailLine->setText(m_account->accountInfo(Parameter::Email));
	ui->aboutLine->setText(m_account->accountInfo(Parameter::About));

	QStringList phone = m_account->accountInfo(Parameter::Phone).split(' ');
	if (!phone.isEmpty())
		ui->phoneLine->setText('+' + phone.last() + phone.first());

	QDateTime time = QDateTime::fromString(m_account->accountInfo(Parameter::Date), "yyyy-MM-dd hh:mm:ss");
	ui->createDate->setDateTime(time);

	const auto& images = m_account->images();
	if (images.isEmpty()) return;

	ui->mainImage->setPixmap(*images.first());
}

void ShowAccountInfo::on_showImages_clicked()
{

}
