#include "month_end_dialog.h"
month_end_dialog:: month_end_dialog(QWidget* pwgt) : QDialog(pwgt, Qt::WindowTitleHint | Qt::WindowSystemMenuHint) {

    credit = new QLabel;
    auction = new QLabel;
    random_event = new QLabel;


    QPushButton* pcmdOk = new QPushButton("&Перейти на следующий раунд");
    connect(pcmdOk, SIGNAL(clicked()), SLOT(accept()));
    QLabel *l1 = new QLabel("Итоги аукциона: ");
    QLabel *l2 = new QLabel("Просрочившие кредит: ");
    QLabel *l3 = new QLabel("Произошедшие события: ");

    QFont font = l1->font();
    font.setPointSize(10);
    font.setBold(true);

    l1->setFont(font);
    l2->setFont(font);
    pcmdOk->setFont(font);
    credit->setFont(font);
    auction->setFont(font);
    random_event->setFont(font);
    l3->setFont(font);


    QGridLayout* grid = new QGridLayout;

    grid->addWidget(l1, 0, 0, 1, 3);
    grid->addWidget(auction, 0, 3, 1, 3);
    grid->addWidget(l2, 1, 0, 1, 3);
    grid->addWidget(credit, 1, 3, 1, 3);
    grid->addWidget(l3, 2, 0, 1, 3);
    grid->addWidget(random_event, 2, 3, 1, 3);
    grid->addWidget(pcmdOk, 3, 0, 1, 6);


    setLayout(grid);

    this->setWindowTitle("Конец месяца");
    this->setModal(true);

    this->setFixedSize(1000, 400);

    QPixmap bkgnd(":resources/bg.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Window, bkgnd);
    this->setPalette(palette);

    this->setWindowIcon(QIcon(":resources/logo.png"));
    this->setWindowFlag(Qt::WindowStaysOnTopHint);
}
void month_end_dialog:: setAuction(QString text) {
    auction->setText(text);
}
void month_end_dialog::setCredit(QString text) {
    credit->setText(text);
}
void month_end_dialog:: setRandomEvent(QString text) {
    random_event->setText(text);
}


