#include "transactionspage.h"
#include "buttonutility.h"

#include <QDir>
#include <QMessageBox>
#include <QHeaderView>

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

    // Hide or show view if there is data to process
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
    QWidget* tableGraphWidget = new QWidget(this);
    QHBoxLayout* tableGraphLayout = new QHBoxLayout(tableGraphWidget);
    mainLayout->addStretch();
    mainLayout->addWidget(tableGraphWidget, 0, Qt::AlignCenter);
    mainLayout->addStretch();
    tableGraphLayout->addWidget(m_tableView, 0, Qt::AlignCenter);

    // View configuration
    m_tableView->setModel(m_model);
    m_tableView->setItemDelegate(m_delegate);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);

    // Adjust size of table
    m_tableView->resizeColumnsToContents();
    m_tableView->resizeRowsToContents();
    m_tableView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    m_tableView->horizontalHeader()->setStretchLastSection(true);

    // Adjust size of columns
    m_tableView->setColumnWidth(0, 100);
    m_tableView->setColumnWidth(1, 200);
    m_tableView->setColumnWidth(2, 75);
    m_tableView->setColumnWidth(3, 200);
    m_tableView->setColumnWidth(4, 100);
    QHeaderView* header = m_tableView->horizontalHeader();
    header->setMaximumSectionSize(250);

    // Delete signal connexion
    connect(m_delegate, &TransactionDelegate::deleteClicked, this, &TransactionsPage::handleDelete);

    m_tableView->hide();
}

void TransactionsPage::createTransactionsMVC(QList<QFile*> files)
{
    m_model->resetTransactions();

    QList<Transaction> transactions;

    // Procces each file
    for(QFile* file: files)
    {
        // Generate transactions from file
        QString filePath = file->fileName();
        try {
            transactions += transactionGenerator(filePath);
        } catch (const std::exception& e) {
            QMessageBox messageBox;
            messageBox.setFixedSize(500,200);
            messageBox.critical(0,"Error", "Error processing file : " + filePath + "\n" + e.what());
            continue;
        }
    }

    // Sort transactions by ascending date
    std::sort(transactions.begin(), transactions.end(), [](const Transaction &a, const Transaction &b) {
        return a.getDate() < b.getDate();
    });

    // Add all transactions to model
    for (Transaction& t : transactions)
        m_model->addTransaction(Transaction(t));
}

void TransactionsPage::handleDelete(int row)
{
    m_model->removeTransaction(row);
}

void TransactionsPage::onDeleteClicked(int row)
{
    m_model->removeTransaction(row);
}
