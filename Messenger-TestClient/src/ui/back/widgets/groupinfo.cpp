#include "groupinfo.h"
#include "ui_groupinfo.h"

#include "client/clientnamespace.h"

using namespace Messenger;

GroupInfo::GroupInfo(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::GroupInfo())
{
	ui->setupUi(this);
}

GroupInfo::~GroupInfo()
{
	delete ui;
}

void GroupInfo::setRoomInfo(const BaseRoomData& room)
{
	ui->groupName->setText(room.name());
	ui->groupTag->setText(room.info(Parameter::Tag));
	ui->groupDate->setDateTime(
		QDateTime::fromString(room.info(Parameter::Date), "yyyy-MM-dd hh:mm:ss")
	);
}
