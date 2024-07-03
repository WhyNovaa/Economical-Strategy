#include "product_dialog.h"
#include <qDebug>
product_dialog::product_dialog(QWidget* pwgt) : QDialog(pwgt, Qt::WindowTitleHint | Qt::WindowSystemMenuHint) {

    spin = new QSpinBox;


    QPushButton* pcmdOk = new QPushButton("&Отправить сырье на переработку");
    QPushButton* quit = new QPushButton("&Выйти");
    connect(pcmdOk, SIGNAL(clicked()), SLOT(accept()));
    connect(quit, SIGNAL(clicked()), SLOT(close()));

    QLabel *l1 = new QLabel("Введите количество сырья для переработки");

    QFont font = l1->font();
    font.setPointSize(13);
    font.setBold(true);

    l1->setFont(font);
    pcmdOk->setFont(font);
    quit->setFont(font);
    spin->setFont(font);


    QGridLayout* grid = new QGridLayout;

    grid->addWidget(l1, 0, 0);
    grid->addWidget(spin, 0, 1);
    grid->addWidget(pcmdOk, 1, 0);
    grid->addWidget(quit, 1, 1);

    setLayout(grid);

    this->setWindowTitle("Запрос на получение кредита и погашение кредита");
    this->setModal(true);

    this->setFixedSize(700, 200);

    QPixmap bkgnd("/Economical-Strategy/resources/bg.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Window, bkgnd);
    this->setPalette(palette);

    this->setWindowIcon(QIcon("/Economical-Strategy/resources/logo.png"));
}

int product_dialog::getAmount() const {
    return spin->value();
}
