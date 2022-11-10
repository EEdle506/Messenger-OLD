#pragma once

#include <QWidget>

#include "client/baseaccount.h"

namespace Ui { 
	class ShowAccountInfo; 
};

class ShowAccountInfo : public QWidget
{
	Q_OBJECT

public:
	ShowAccountInfo(BaseAccountData* account, QWidget *parent = nullptr);
	~ShowAccountInfo() override;

private slots:
	void on_showImages_clicked();
	void on_loadImage_clicked();

	void on_changeButton_clicked();
	void on_hideButton_clicked();
	void on_refreshButton_clicked();

private:
	BaseAccountData* m_account = nullptr;
	Ui::ShowAccountInfo *ui;
};
