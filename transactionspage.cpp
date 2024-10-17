#include "transactionspage.h"

#include <QDir>

TransactionsPage::TransactionsPage(QWidget *parent) : QWidget(parent), m_tableView(new QTableView(this)), m_model(new TransactionModel(this)), m_delegate(new TransactionDelegate(this))
{
    QLabel* title = new QLabel("Transactions", this);
    title->setObjectName("title");
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(title);

    layout->addWidget(m_tableView);

    // View configuration
    m_tableView->setModel(m_model);
    m_tableView->setItemDelegate(m_delegate);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);

    // Delete signal connexion
    connect(m_delegate, &TransactionDelegate::deleteClicked, this, &TransactionsPage::handleDelete);

    // Generate transactions from file
    QString filePath = QDir::current().filePath("../Moneo/data/Test.csv");
    QList<Transaction> transactions = transactionGenerator(filePath);

    // Add all transactions to model
    for (Transaction& t : transactions)
    {
        m_model->addTransaction(Transaction(t));
    }

    // Test to verify the modification
    QPushButton* btnTest = new QPushButton(this);
    btnTest->setText("Test");
    connect(btnTest, &QPushButton::clicked, [this]() {
        QVector<Transaction> currentTransactions = m_model->getTransactions();
        for (const Transaction& t : currentTransactions) {
            qDebug() << "Date:" << t.getDate().day() << "/" << t.getDate().month() << "/" << t.getDate().year() << " | "
                     << "Description:" << t.getDescription() << " | "
                     << "Amount:" << t.getAmount() << " | "
                     << "Catégory:" << t.getCategory() << "\n";
        }
    });
}

void TransactionsPage::handleDelete(int row)
{
    m_model->removeTransaction(row);
}
