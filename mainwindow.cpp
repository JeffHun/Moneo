#include "mainwindow.h"
#include "transaction.h"

#include <QTableView>
#include <QVBoxLayout>
#include <QWidget>
#include <QHeaderView>
#include <QDir>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), tableView(new QTableView(this)), model(new TransactionModel(this)), delegate(new TransactionDelegate(this))
{
    // UI configuration
    QWidget* central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout* layout = new QVBoxLayout(central);
    layout->addWidget(tableView);

    // View configuration
    tableView->setModel(model);
    tableView->setItemDelegate(delegate);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);

    // Delete signal connexion
    connect(delegate, &TransactionDelegate::deleteClicked, this, &MainWindow::handleDelete);

    // Generate transactions from file
    QString filePath = QDir::current().filePath("../Moneo/data/Test.csv");
    QList<Transaction> transactions = transactionGenerator(filePath);

    // Add all transactions to model
    for (Transaction& t : transactions){model->addTransaction(Transaction(t));}

    // Test to verify the modification
    QPushButton* btnTest = new QPushButton(central);
    btnTest->setText("Test");
    connect(btnTest, &QPushButton::clicked, [this]() {
        QVector<Transaction> currentTransactions = model->getTransactions();
        for (const Transaction& t : currentTransactions) {
            qDebug() << "Date:" << t.getDate().day() << "/" << t.getDate().month() << "/" << t.getDate().year() << " | "
                     << "Description:" << t.getDescription() << " | "
                     << "Amount:" << t.getAmount() << " | "
                     << "Catégory:" << t.getCategory() << "\n";
        }
    });

}

MainWindow::~MainWindow()
{
}

void MainWindow::handleDelete(int row)
{
    model->removeTransaction(row);
}
