#ifndef CREDIT_DIALOG_H
#define CREDIT_DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QString>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QSpinBox>

class credit_dialog : public QDialog{
    Q_OBJECT
public:
    credit_dialog(QWidget* pwgt = 0);
    int getCredit() const;
    int getLowCredit() const;

private slots:
    void quit();

private:
    QLineEdit *credit;
    QLineEdit *low_credit;
};

#endif // CREDIT_DIALOG_H
