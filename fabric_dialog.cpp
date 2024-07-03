#include "fabric_dialog.h"
#include <qDebug>
fabric_dialog::fabric_dialog(QWidget* pwgt) : QDialog(pwgt, Qt::WindowTitleHint | Qt::WindowSystemMenuHint) {

    spin = new QSpinBox;


    QPushButton* pcmdOk = new QPushButton("&Улучшить фабрики");
    QPushButton* quit = new QPushButton("&Выйти");
    connect(pcmdOk, SIGNAL(clicked()), SLOT(accept()));
    connect(quit, SIGNAL(clicked()), SLOT(close()));

    QLabel *l1 = new QLabel("Введите количество фабрик для улучшения");

    QFont font = l1->font();
    font.setPointSize(13);
    font.setBold(true);

    l1->setFont(font);
    pcmdOk->setFont(font);
    quit->setFont(font);
    spin->setFont(font);


    QGridLayout* grid = new QGridLayout;

    grid->addWidget(l1, 0, 1, 1, 3);
    grid->addWidget(spin, 0, 4, 1, 3);
    grid->addWidget(pcmdOk, 2, 0, 1, 2);
    grid->addWidget(quit, 2, 4, 1, 2);

    setLayout(grid);

    this->setWindowTitle("Запрос на получение кредита и погашение кредита");
    this->setModal(true);

    QPixmap bkgnd("/Economical-Strategy/resources/bg.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Window, bkgnd);
    this->setPalette(palette);

    this->setWindowIcon(QIcon("/Economical-Strategy/resources/logo.png"));
    this->setFixedSize(700, 200);
}

int fabric_dialog::getAmount() const {
    return spin->value();
}
