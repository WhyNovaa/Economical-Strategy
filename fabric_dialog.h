#ifndef FABRIC_DIALOG_H
#define FABRIC_DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QString>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QSpinBox>

class fabric_dialog : public QDialog{
    Q_OBJECT
public:
    fabric_dialog(QWidget* pwgt = 0);
    int getAmount() const;

private:
    QSpinBox *spin;
};

#endif // FABRIC_DIALOG_H
