#include "transactionspage.h"
#include "buttonutility.h"

#include <QDir>
#include <QMessageBox>

TransactionsPage::TransactionsPage(QWidget *parent) : QWidget(parent)
{
    setupUI();
    setupTableView();

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

void TransactionsPage::setupUI()
{
    QLabel* title = new QLabel("Transactions", this);
    title->setObjectName("bigTitle");
    title->setAlignment(Qt::AlignCenter);
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(title);
    FileDropZone* dropZone = new FileDropZone(this);
    layout->addWidget(dropZone, 0, Qt::AlignCenter);

    QPushButton* proceedFilesBtn = new QPushButton("Process file(s)" ,this);
    layout->addWidget(proceedFilesBtn, 0, Qt::AlignCenter);
    ButtonUtility::connectButton(proceedFilesBtn);

    connect(proceedFilesBtn, &QPushButton::clicked, this, [this, dropZone]() {
        if(dropZone->getFiles().isEmpty())
        {
            QMessageBox messageBox;
            messageBox.setFixedSize(500,200);
            messageBox.critical(0,"Error", "No data to process");
            m_tableView->hide();
        }
        else
        {
            createTransactionsMVC(dropZone->getFiles());
            m_tableView->show();
        }

    });
}

void TransactionsPage::setupTableView()
{
    // MVC creation
    m_tableView = new QTableView(this);
    m_model = new TransactionModel(this);
    m_delegate = new TransactionDelegate(this);

    QVBoxLayout* mainLayout = qobject_cast<QVBoxLayout*>(layout());
    if (mainLayout)
        mainLayout->addWidget(m_tableView);

    // View configuration
    m_tableView->setModel(m_model);
    m_tableView->setItemDelegate(m_delegate);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);

    // Delete signal connexion
    connect(m_delegate, &TransactionDelegate::deleteClicked, this, &TransactionsPage::handleDelete);

    m_tableView->hide();
}

void TransactionsPage::createTransactionsMVC(QList<QFile*> files)
{
    m_model->resetTransactions();

    // Procces each file
    for(QFile* file: files)
    {
        // Generate transactions from file
        QString filePath = file->fileName();

        QList<Transaction> transactions;
        try {
            transactions = transactionGenerator(filePath);
        } catch (const std::exception& e) {
            qDebug() << "Error processing file:" << filePath << "->" << e.what();
            continue;
        }

        // Add all transactions to model
        for (Transaction& t : transactions)
        {
            m_model->addTransaction(Transaction(t));
        }
    }
}

void TransactionsPage::handleDelete(int row)
{
    m_model->removeTransaction(row);
}
