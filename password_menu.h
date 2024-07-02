#ifndef PASSWORD_MENU_H
#define PASSWORD_MENU_H

#include <QDialog>

namespace Ui {
class password_menu;
}

class password_menu : public QDialog
{
    Q_OBJECT

public:
    explicit password_menu(QWidget *parent = nullptr);
    ~password_menu();
public slots:
    void slot(int x);
private slots:
    void on_pushButton_clicked();
signals:
    void signal_pass_back(QString);
private:
    Ui::password_menu *ui;
};

#endif // PASSWORD_MENU_H
