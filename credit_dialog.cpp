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
    connect(quit, SIGNAL(clicked()), SLOT(quit()));

    QLabel *l1 = new QLabel("Введите сумму, на которую хотите взять кредит");
    QLabel *l2 = new QLabel("Введите сумму, на которую хотите погасить кредит");

    QFont font = l1->font();
    font.setPointSize(13);
    font.setBold(true);

    l1->setFont(font);
    l2->setFont(font);
    pcmdOk->setFont(font);
    pcmdCancel->setFont(font);
    quit->setFont(font);
    credit->setFont(font);
    low_credit->setFont(font);


    QGridLayout* grid = new QGridLayout;

    grid->addWidget(l1, 0, 3, 1, 3);
    grid->addWidget(l2, 1, 3, 1, 3);
    grid->addWidget(credit, 0, 0, 1, 3);
    grid->addWidget(low_credit, 1, 0, 1, 3);
    grid->addWidget(pcmdOk, 2, 0, 1, 2);
    grid->addWidget(pcmdCancel, 2, 2, 1, 2);
    grid->addWidget(quit, 2, 4, 1, 2);

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

int credit_dialog:: getCredit() const {
    if(credit->text().isEmpty() || credit->text().toInt() == 0) {
        return -1;
    }
    return(credit->text().toInt());
}

int credit_dialog::getLowCredit() const {
    if(low_credit->text().isEmpty() || low_credit->text().toInt() == 0) {
        return -1;
    }
    return(low_credit->text().toInt());
}

void credit_dialog::quit(){
    low_credit->setText("-4");
    this->close();
}
