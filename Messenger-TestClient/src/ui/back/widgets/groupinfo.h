#pragma once

#include <QWidget>

#include "client/baseroom.h"

namespace Ui { 
	class GroupInfo; 
};

class GroupInfo : public QWidget
{
	Q_OBJECT

public:
	explicit GroupInfo(QWidget *parent = nullptr);
	~GroupInfo() override;

	void setRoomInfo(const BaseRoomData& room);

private:
	Ui::GroupInfo *ui;
};
