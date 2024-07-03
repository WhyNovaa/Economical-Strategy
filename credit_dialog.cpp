#include <credit_dialog.h>
#include <QRegularExpressionValidator>
credit_dialog:: credit_dialog(QWidget* pwgt) : QDialog(pwgt, Qt::WindowTitleHint | Qt::WindowSystemMenuHint) {
    QRegularExpression re("[0-9]{6}");
    QRegularExpressionValidator *val1 = new QRegularExpressionValidator;
    val1->setRegularExpression(re);
    credit = new QLineEdit;
    low_credit = new QLineEdit;
    credit->setValidator(val1);
    low_credit->setValidator(val1);
    QPushButton* pcmdOk = new QPushButton("&Взять кредит");
    QPushButton* pcmdCancel = new QPushButton("&Погасить кредит");
     QPushButton* quit = new QPushButton("&Выйти");
    connect(pcmdOk, SIGNAL(clicked()), SLOT(accept()));
    connect(pcmdCancel, SIGNAL(clicked()), SLOT(reject()));
    connect(quit, SIGNAL(clicked()), SLOT(close()));
    QHBoxLayout *h1 = new  QHBoxLayout;
    QHBoxLayout *h2 = new  QHBoxLayout;
    QHBoxLayout *h3 = new  QHBoxLayout;
    QVBoxLayout *v1 =  new  QVBoxLayout;
    QLabel *l1 = new QLabel("Введите сумму, на которую хотите взять кредит");
    QLabel *l2 = new QLabel("Введите сумму, на которую хотите погасить кредит");
     h2->addWidget(credit); h2->addWidget(l1);
    h3->addWidget(low_credit); h3->addWidget(l2);
    h1->addWidget(pcmdOk); h1->addWidget(pcmdCancel); h1->addWidget(quit);
    v1->addLayout(h2); v1->addLayout(h3); v1->addLayout(h1);
    setLayout(v1);
    this->resize(400, 100);
    this->setMinimumSize(400, 100);
    this->setMaximumSize(400, 100);
    this->setWindowTitle("Запрос на получение кредита и погашение кредита");
    this->setModal(true);
}

int credit_dialog:: getCredit() const {
    if(credit->text().isEmpty() || credit->text().toInt() <= 0) {
        return -1;
    }
    return(credit->text().toInt());
}
int credit_dialog::getLowCredit() const {
    if(low_credit->text().isEmpty() || low_credit->text().toInt() <= 0) {
        return -1;
    }
    return(low_credit->text().toInt());
}
