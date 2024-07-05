#ifndef MONTH_END_DIALOG_H
#define MONTH_END_DIALOG_H


#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QString>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QSpinBox>

class month_end_dialog : public QDialog{
    Q_OBJECT
public:
    month_end_dialog(QWidget* pwgt = 0);
    void setAuction(QString text);
    void setCredit(QString text);
    void setRandomEvent(QString text);
private:
    QLabel *credit;
    QLabel *auction;
    QLabel *random_event;
};



#endif // MONTH_END_DIALOG_H
