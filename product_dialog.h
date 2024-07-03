#ifndef PRODUCT_DIALOG_H
#define PRODUCT_DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QString>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QSpinBox>

class product_dialog : public QDialog{
    Q_OBJECT
public:
    product_dialog(QWidget* pwgt = 0);
    int getAmount() const;

private:
    QSpinBox *spin;
};

#endif
