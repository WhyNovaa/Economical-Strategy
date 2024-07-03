#include "bet_dialog.h"
#include <QRegularExpressionValidator>
bet_dialog:: bet_dialog(int cur_raw_count, int cur_prod_count, int cur_raw_price, int cur_prod_price, QWidget* pwgt) : QDialog(pwgt, Qt::WindowTitleHint | Qt::WindowSystemMenuHint) {
    QRegularExpression re("[0-9]{6}");
    QRegularExpressionValidator *val1 = new QRegularExpressionValidator;
    val1->setRegularExpression(re);
    raw_price = new QLineEdit;
    prod_price = new QLineEdit;
    raw_price->setValidator(val1);
    prod_price->setValidator(val1);
    raw_price->setSizePolicy(QSizePolicy());
    prod_price->setSizePolicy(QSizePolicy());
    QPushButton* pcmdOk = new QPushButton("&Сделать ставку");
    QPushButton* pcmdCancel = new QPushButton("&Выйти");
    connect(pcmdOk, SIGNAL(clicked()), SLOT(accept()));
    connect(pcmdCancel, SIGNAL(clicked()), SLOT(reject()));
    QLabel *lrc = new QLabel("Кол-во сырья, продаваемого банком: " + QString::number(cur_raw_count) );
    QLabel *lpc = new QLabel("Кол-во продукции, покупаемой банком: " + QString::number(cur_prod_count));
    QLabel *lrp = new QLabel("Цена за всё сырьё, предлагаемая банком: " + QString::number(cur_raw_price));
    QLabel *lpp = new QLabel("Цена за всю продукцию, предлагаемая банком: " + QString::number(cur_prod_price));
    QLabel *advice = new QLabel("Сделайте самое выгодное предложение мреди всех игроков, чтобы банк принял его!");
    QHBoxLayout *h1 = new  QHBoxLayout;
    QHBoxLayout *h2 = new  QHBoxLayout;
    QHBoxLayout *h3 = new  QHBoxLayout;
    QHBoxLayout *h4 = new  QHBoxLayout;
    QHBoxLayout *h5 = new  QHBoxLayout;
    QVBoxLayout *v1 =  new  QVBoxLayout;
    QLabel *l1 = new QLabel("Введите сумму, за которую хотите купить сырьё");
    QLabel *l2 = new QLabel("Введите сумму, за которую хотите продать продукцию");
    h2->addWidget(lrc); h2->addWidget(lpc);
    h3->addWidget(lrp); h3->addWidget(lpp);
    h4->addWidget(l1);  h4->addWidget(raw_price);  h5->addWidget(l2);  h5->addWidget(prod_price);
    h1->addWidget(pcmdOk); h1->addWidget(pcmdCancel);
    v1->addLayout(h2); v1->addLayout(h3); v1->addLayout(h4); v1->addLayout(h5); v1->addWidget(advice); v1->addLayout(h1);
    setLayout(v1);
    this->resize(800, 200);
    this->setMinimumSize(800, 200);
    this->setMaximumSize(800, 200);
    this->setWindowTitle("Аукцион в банке");
    this->setModal(true);
}

int bet_dialog:: getRawPrice() const {
    if(raw_price->text().isEmpty() || raw_price->text().toInt() == 0) {
        return -1;
    }
    return(raw_price->text().toInt());
}
int bet_dialog::getProdPrice() const {
    if(prod_price->text().isEmpty() || prod_price->text().toInt() == 0) {
        return -1;
    }
    return(prod_price->text().toInt());
}
