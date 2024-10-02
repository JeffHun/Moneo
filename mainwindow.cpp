#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPushButton>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), model(new FinanceModel(this)), controller(new FinanceController(model, this))
{
    ui->setupUi(this);

    ui->tableViewTransactions->setModel(model);

    connect(ui->btnAddTransaction, &QPushButton::clicked, this, [this]() {
        Transaction t(QDate::currentDate(), "Salary", 3000.0, "Income");
        controller->addTransaction(t);
    });

    connect(ui->btnRemoveTransaction, &QPushButton::clicked, this, [this]() {
        QModelIndex currentIndex = ui->tableViewTransactions->currentIndex();
        if (currentIndex.isValid()) {
            controller->removeTransaction(currentIndex.row());
        } else {
            QMessageBox::warning(this, "Suppression", "Please select a transaction to delete.");
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
