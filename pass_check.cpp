#include "pass_check.h"
#include "ui_pass_check.h"
#include"sha1.h"

pass_check::pass_check(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::pass_check)
{
    ui->setupUi(this);
    ui->lineEdit->setEchoMode(QLineEdit::Password);

    QPixmap bkgnd("/Economical-Strategy/resources/bg.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::KeepAspectRatioByExpanding);
    QPalette palette;
    palette.setBrush(QPalette::Window, bkgnd);

    this->setPalette(palette);
    this->setWindowIcon(QIcon("/Economical-Strategy/resources/logo.png"));
}

pass_check::~pass_check()
{
    delete ui;
}

QString True_pass;
void pass_check::slot_index(QString true_pass, int i)
{
    True_pass = true_pass;
    QString text = "игрок " + QString::number(i+1) + " введите ваш пароль";
    ui->label->setText(text);
}

void pass_check::on_pushButton_clicked()
{
    QString pass = ui->lineEdit->text();
    if((QString::fromStdString(sha1(pass.toStdString()))) == True_pass){
        emit signal_pass_check(1);
    }else{
        ui->label->setText("Wrong password try again");
        ui->lineEdit->clear();
        emit signal_pass_check(0);
    }
}

