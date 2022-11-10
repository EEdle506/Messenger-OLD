#pragma once

#include <QDialog>

namespace Ui {
    class DialogUsername;
}


enum DialogUser {
    Create,
    Login
};

struct _UserData {
    QString nameTag, name, password,
    phone, phoneCountry,
    phoneCountryNum;
};

class DialogUsername : public QDialog
{
    Q_OBJECT

public:
    explicit DialogUsername(DialogUser type, QWidget *parent = nullptr);
    ~DialogUsername();

    _UserData getUserData();

private slots:
    void on_pushButton_clicked();

    void on_buttonSeePass_pressed();

    void on_buttonSeePass_released();

private:
    DialogUser m_type;
    Ui::DialogUsername *ui;
};
