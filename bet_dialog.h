#ifndef BET_DIALOG_H
#define BET_DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QString>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
class bet_dialog : public QDialog{
    Q_OBJECT
public:
    bet_dialog(int cur_raw_count, int cur_prod_count, int cur_raw_price, int cur_prod_price, QWidget* pwgt = 0);
    int getRawPrice() const;
    int getProdPrice() const;

private:
    QLineEdit *raw_price;
    QLineEdit *prod_price;
};


#endif // BET_DIALOG_H
