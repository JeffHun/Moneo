#include "transactionswindow.h"

TransactionsWindow::TransactionsWindow(QWidget *parent, QList<Transaction> transactions) : QWidget(parent){
    transactions = transactions;
    this->setFixedSize(700,400);
    this->setObjectName("detailWindow");
    this->setStyleSheet("#detailWindow{background-color : #241E38;}");
    QLabel* title = new QLabel(transactions[0].getCategory(), this);
    title->setObjectName("title");
    title->setStyleSheet("#title{"
                         "font-family: Roboto Medium;"
                         "font-size: 25px;"
                         "color: white;}");
    title->setAlignment(Qt::AlignCenter);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setObjectName("scrollArea");
    scrollArea->setStyleSheet("#scrollArea{"
                              "border: none;"
                              "}"
                              "QScrollBar:vertical{"
                              "border: none;"
                              "background: #3D3558;"
                              "width: 10px;"
                              "border-radius: 5px;"
                              "}"
                              "QScrollBar::handle:vertical{"
                              "background: #6954D7;"
                              "border-radius: 5px;"
                              "}"
                              "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical{"
                              "background: none;"
                              "border: none;"
                              "}"
                              "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical{"
                              "background: none;"
                              "}");
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setWidgetResizable(true);

    QWidget* scrollWidget = new QWidget(this);
    scrollWidget->setObjectName("scrollContent");
    scrollWidget->setStyleSheet("#scrollContent{"
                                 "background-color: #241E38;}");
    QVBoxLayout* scrollLayout = new QVBoxLayout(scrollWidget);

    for (const Transaction &transaction : transactions) {
        QLabel* date = new QLabel(transaction.getDate().toString("dd/MM/yyyy"), this);
        date->setObjectName("transactionDetail");
        QLabel* description = new QLabel(transaction.getDescription(), this);
        description->setObjectName("transactionDetail");
        QLabel* amount = new QLabel(QString::number(transaction.getAmount()), this);
        amount->setObjectName("transactionDetail");

        QString labelStyle = "#transactionDetail {"
                             "font-family: Roboto Light;"
                             "font-size: 15px;"
                             "color: white;}";
        date->setStyleSheet(labelStyle);
        description->setStyleSheet(labelStyle);
        amount->setStyleSheet(labelStyle);

        QWidget* transactionWidget = new QWidget(this);
        QHBoxLayout* transactionLayout = new QHBoxLayout(transactionWidget);
        transactionLayout->addWidget(date);
        transactionLayout->addStretch();
        transactionLayout->addWidget(description);
        transactionLayout->addStretch();
        transactionLayout->addWidget(amount);

        scrollLayout->addWidget(transactionWidget);
    }

    scrollWidget->setLayout(scrollLayout);
    scrollArea->setWidget(scrollWidget);

    // Main Layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(title);
    mainLayout->addWidget(scrollArea);

    setLayout(mainLayout);
}
