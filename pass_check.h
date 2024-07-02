#ifndef PASS_CHECK_H
#define PASS_CHECK_H

#include <QDialog>

namespace Ui {
class pass_check;
}

class pass_check : public QDialog
{
    Q_OBJECT

public:
    explicit pass_check(QWidget *parent = nullptr);
    ~pass_check();
signals:
    void signal_pass_check(int);
public slots:
    void slot_index(QString true_pass, int i);
private slots:
    void on_pushButton_clicked();

private:
    Ui::pass_check *ui;
};

#endif // PASS_CHECK_H
