#include "password_menu.h"
#include "ui_password_menu.h"
#include"sha1.h"

password_menu::password_menu(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::password_menu)
{
    ui->setupUi(this);
    this->setWindowTitle("Password menu");
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

password_menu::~password_menu()
{
    delete ui;
}

int a;
void password_menu::slot(int x)
{
    a=x;
}
int i = 1;
void password_menu::on_pushButton_clicked()
{
    if(i<=a){
        QString text = "игрок " + QString::number(i+1) + " введите пароль для этой игры";
        ui->label->setText(text);

        QString pass = ui->lineEdit->text();
        ui->lineEdit->clear();
        emit signal_pass_back(QString::fromStdString(sha1(pass.toStdString())));
        i++;
        if(i==(a+1)){
            emit signal_pass_back("end");
        }
    }
}

