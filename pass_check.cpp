#include "pass_check.h"
#include "ui_pass_check.h"
#include"sha1.h"
#include"mainwindow.h"

pass_check::pass_check(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::pass_check)
{
    ui->setupUi(this);
    this->setWindowTitle("Password checking");
    ui->lineEdit->setEchoMode(QLineEdit::Password);



    QFont font = ui->label->font();
    font.setPointSize(13);
    font.setBold(true);
    ui->label->setFont(font);
    ui->label->setAlignment(Qt::AlignCenter);
    ui->lineEdit->setFont(font);
    ui->pushButton->setFont(font);



    QPixmap bkgnd(":resources/bg.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Window, bkgnd);

    this->setPalette(palette);
    this->setWindowIcon(QIcon(":resources/logo.png"));

    this->setFixedSize(400, 200);
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
        if( pass=="" || (QString::fromStdString(sha1(pass.toStdString())))!=True_pass){
            //qDebug()<< "here";
            ui->label->setText("Wrong password try again");
            ui->lineEdit->clear();
            //emit signal_pass_check(2);
        }
    }
}

