#include "transactionspage.h"
#include "buttonutility.h"

#include <QDir>
#include <QMessageBox>
#include <QHeaderView>
#include <algorithm>
#include <iostream>>
#include <QRandomGenerator>

TransactionsPage::TransactionsPage(QWidget *parent) : QWidget(parent)
{
    setupUI();
    setupTableView();

    // Test to verify the modification
    QPushButton* btnTest = new QPushButton(this);
    btnTest->setText("Test");
    connect(btnTest, &QPushButton::clicked, [this]() {
        QVector<Transaction> currentTransactions = m_transactionsModel->getTransactions();
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
            m_transactionsView->hide();
        }
        else
        {
            loadTransactionsFromFilesAndSetupModel(dropZone->getFiles());
            m_transactionsView->show();
        }

    });
}

void TransactionsPage::setupTableView()
{
    // MVC creation
    m_transactionsView = new QTableView(this);
    m_transactionsModel = new TransactionModel(this);
    m_transactionsDelegate = new TransactionDelegate(this);

    QVBoxLayout* mainLayout = qobject_cast<QVBoxLayout*>(layout());
    QWidget* tableGraphWidget = new QWidget(this);
    mainLayout->addWidget(tableGraphWidget);
    m_tableGraphLayout = new QHBoxLayout(tableGraphWidget);
    m_tableGraphLayout->addWidget(m_transactionsView);

    // View configuration
    m_transactionsView->setModel(m_transactionsModel);
    m_transactionsView->setItemDelegate(m_transactionsDelegate);
    m_transactionsView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_transactionsView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);

    // Adjust size of table
    m_transactionsView->resizeColumnsToContents();
    m_transactionsView->resizeRowsToContents();
    m_transactionsView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    m_transactionsView->horizontalHeader()->setStretchLastSection(false);

    // Adjust size of columns
    m_transactionsView->setColumnWidth(0, 100);
    m_transactionsView->setColumnWidth(1, 225);
    m_transactionsView->setColumnWidth(2, 100);
    m_transactionsView->setColumnWidth(3, 120);
    m_transactionsView->setColumnWidth(4, 100);
    QHeaderView* header = m_transactionsView->horizontalHeader();
    header->setMaximumSectionSize(250);

    // Delete signal connexion
    connect(m_transactionsDelegate, &TransactionDelegate::deleteClicked, this, &TransactionsPage::handleDelete);

    m_transactionsView->hide();
}

void TransactionsPage::loadTransactionsFromFilesAndSetupModel(QList<QFile*> files)
{
    m_transactionsModel->resetTransactions();

    QList<Transaction> transactions;

    loadTransactionsFromFiles(files, transactions);
    updateBalanceAndModel(transactions);

    // Sort transactions by ascending date
    std::sort(transactions.begin(), transactions.end(), [](const Transaction &a, const Transaction &b) {
        return a.getDate() < b.getDate();
    });
}

void TransactionsPage::loadTransactionsFromFiles(QList<QFile*> files, QList<Transaction>& transactions) {
    for (QFile* file : files) {
        try {
            transactions += transactionGenerator(file->fileName());
        } catch (const std::exception& e) {
            showError("Error processing file : " + file->fileName() + "\n" + e.what());
        }
    }
}

void TransactionsPage::updateBalanceAndModel(QList<Transaction>& transactions) {
    QMap<QDate, int> balance;

    int value = 0;
    QDate date;
    int currentMonth = transactions[0].getDate().month();

    // Add all transactions to model and to balance
    for (Transaction& t : transactions)
    {
        if(t.getDate().month() == currentMonth)
        {
            value += t.getAmount();
            date = t.getDate();
            balance[date] = value;
        }
        else
        {
            value = t.getAmount();
            date = t.getDate();
            balance[date] = value;
            currentMonth = t.getDate().month();
        }

        m_transactionsModel->addTransaction(Transaction(t));
    }
    createGraphLine(balance);
}

void TransactionsPage::showError(const QString& message) {
    QMessageBox messageBox;
    messageBox.setFixedSize(500, 200);
    messageBox.critical(0, "Error", message);
}

void TransactionsPage::createGraphLine(QMap<QDate, int> balance)
{
    QChart* chart = new QChart();
    chart->setBackgroundBrush(QBrush(QColor("#241E38")));
    chart->setTitleFont(QFont("Roboto Light", 10));
    chart->legend()->setLabelBrush(QBrush(QColor("#D9D9D9")));

    //horizontal line of zero value
    QLineSeries *zeroLine = new QLineSeries(this);
    qreal yValue = 0.0;
    zeroLine->append(0, yValue);
    zeroLine->append(100, yValue);
    QPen pen(Qt::gray);
    pen.setWidth(1);
    pen.setStyle(Qt::DashLine);
    zeroLine->setPen(pen);
    chart->addSeries(zeroLine);
    chart->legend()->markers(zeroLine)[0]->setVisible(false);

    QList<QLineSeries*> series;
    series.append(new QLineSeries(this));
    int i = 0;
    QMap<QDate, int>::const_iterator it = balance.constBegin();
    int currentMonth = it.key().month();
    series[0]->setName(QString::number(currentMonth)+"/"+QString::number(it.key().year()));
    for (const QDate &key : balance.keys()) {
        if(key.month() == currentMonth)
            series[i]->append(key.day(), balance[key]);
        else
        {
            series.append(new QLineSeries(this));
            i++;
            currentMonth = key.month();
            series[i]->setName(QString::number(key.month())+"/"+QString::number(key.year()));
            QPen pen(generateRandomColor());
            series[i]->setPen(pen);
        }
    }


    for(QLineSeries* serie: series)
        chart->addSeries(serie);

    chart->createDefaultAxes();
    auto minmax = std::minmax_element(balance.begin(), balance.end());
    int minValue = *minmax.first;
    int maxValue = *minmax.second;
    chart->axes(Qt::Vertical).first()->setRange(minValue, maxValue);
    chart->axes(Qt::Horizontal).first()->setRange(1, 31);

    QValueAxis *axisX = qobject_cast<QValueAxis*>(chart->axes(Qt::Horizontal).first());
    QValueAxis *axisY = qobject_cast<QValueAxis*>(chart->axes(Qt::Vertical).first());

    configureAxis(axisX, "Day");
    configureAxis(axisY, "Balance");

    chart->setVisible(true);

    QChartView* chartView = new QChartView(chart);
    chartView->setObjectName("chartViewTransaction");
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setVisible(true);
    m_tableGraphLayout->addWidget(chartView);

    QWidget* monthBtns = new QWidget(this);
    m_tableGraphLayout->addWidget(monthBtns);

    QVBoxLayout* monthBtnsLayout = new QVBoxLayout(monthBtns);

    for(QLineSeries* serie: series)
    {
        QPushButton* btn = new QPushButton(serie->name(), this);
        monthBtnsLayout->addWidget(btn);
        ButtonUtility::connectToggleActiveProperty(btn);
        btn->setProperty("active", true);
        connect(btn, &QPushButton::clicked, this, [btn, chart, serie]() {
            bool isActive = btn->property("active").toBool();
            serie->setVisible(isActive);
        });
    }
}

QColor TransactionsPage::generateRandomColor() {
    int r = QRandomGenerator::global()->bounded(256);
    int g = QRandomGenerator::global()->bounded(256);
    int b = QRandomGenerator::global()->bounded(256);
    return QColor(r, g, b);
}

void TransactionsPage::configureAxis(QValueAxis *axis, const QString &title) {
    if (axis) {
        axis->setTitleText(title);
        axis->setTitleBrush(QBrush(QColor("#D9D9D9")));
        axis->setLabelsBrush(QBrush(QColor("#D9D9D9")));
        axis->setLabelFormat("%d");
        axis->setGridLineVisible(false);
    }
}

void TransactionsPage::handleDelete(int row)
{
    m_transactionsModel->removeTransaction(row);
}

void TransactionsPage::onDeleteClicked(int row)
{
    m_transactionsModel->removeTransaction(row);
}
