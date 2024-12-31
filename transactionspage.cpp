#include "transactionspage.h"
#include "buttonutility.h"

#include <QDir>
#include <QMessageBox>
#include <QHeaderView>
#include <algorithm>
#include <iostream>>
#include <QRandomGenerator>

TransactionsPage::TransactionsPage(QWidget *parent) : QWidget(parent), m_balanceChart(nullptr)
{
    setupUI();
    setupTableView();
}

QVector<Transaction> TransactionsPage::getTransactions()
{
    return m_transactionsModel->getTransactions();
}

void TransactionsPage::setupUI()
{
    QLabel* title = new QLabel("Transactions", this);
    title->setObjectName("bigTitle");
    title->setAlignment(Qt::AlignCenter);
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(title);
    m_dropZone = new FileDropZone(this);
    layout->addWidget(m_dropZone, 0, Qt::AlignCenter);

    QPushButton* proceedFilesBtn = new QPushButton("Process file(s)" ,this);
    layout->addWidget(proceedFilesBtn, 0, Qt::AlignCenter);
    ButtonUtility::connectButton(proceedFilesBtn);

    // Hide or show view if there is data to process
    connect(proceedFilesBtn, &QPushButton::clicked, this, [this]() {
        if(m_dropZone->getFiles().isEmpty())
        {
            QMessageBox messageBox;
            messageBox.setFixedSize(500,200);
            messageBox.critical(0,"Error", "No data to process");
        }
        else
        {
            loadTransactionsFromFilesAndSetupModel(m_dropZone->getFiles());
            m_transactionsView->show();
        }

    });
}

int TransactionsPage::getNbrFile()
{
    return m_dropZone->getFileNbr();
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

    // Sort from newest to oldest
    std::sort(transactions.begin(), transactions.end(), [](const Transaction &a, const Transaction &b) {
        return a.getDate() < b.getDate();
    });

    updateBalanceAndModel(transactions);
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
    if (!m_balanceChart) {
        m_balanceChart = new QChart();
        m_balanceChart->setAnimationOptions(QChart::SeriesAnimations);
        m_balanceChart->setBackgroundBrush(QBrush(QColor("#241E38")));
        m_balanceChart->setTitleFont(QFont("Roboto Light", 10));
        m_balanceChart->legend()->setLabelBrush(QBrush(QColor("#D9D9D9")));

        m_chartView = new QChartView(m_balanceChart);
        m_chartView->setObjectName("chartViewTransaction");
        m_chartView->setRenderHint(QPainter::Antialiasing);

        m_balanceContainer = new QWidget(this);
        m_balanceContainerLayout = new QVBoxLayout(m_balanceContainer);
        m_monthBtnsContainer = new QWidget(this);
        m_gridLayout = new QGridLayout(m_monthBtnsContainer);

        m_balanceContainerLayout->addWidget(m_chartView);
        m_balanceContainerLayout->addWidget(m_monthBtnsContainer);
        m_tableGraphLayout->addWidget(m_balanceContainer);
    }else
    {
        // Clear previous chart data
        m_balanceChart->removeAllSeries();
        qDeleteAll(m_monthBtnsContainer->findChildren<QPushButton*>());
        m_gridLayout->update();
    }

    // Add zero line
    QLineSeries *zeroLine = new QLineSeries(m_balanceChart);
    qreal yValue = 0.0;
    zeroLine->append(0, yValue);
    zeroLine->append(100, yValue);
    QPen zeroPen(Qt::gray);
    zeroPen.setWidth(1);
    zeroPen.setStyle(Qt::DashLine);
    zeroLine->setPen(zeroPen);
    m_balanceChart->addSeries(zeroLine);
    m_balanceChart->legend()->markers(zeroLine)[0]->setVisible(false);

    // Create new series for each month
    QList<QLineSeries*> series;
    series.append(new QLineSeries(m_balanceChart));
    int i = 0;
    QMap<QDate, int>::const_iterator it = balance.constBegin();
    int currentMonth = it.key().month();
    series[0]->setName(QString::number(currentMonth)+"/"+QString::number(it.key().year()));
    for (const QDate &key : balance.keys()) {
        if(key.month() == currentMonth)
            series[i]->append(key.day(), balance[key]);
        else
        {
            series.append(new QLineSeries(m_balanceChart));
            i++;
            currentMonth = key.month();
            series[i]->setName(QString::number(key.month())+"/"+QString::number(key.year()));
            QPen pen(generateRandomColor());
            series[i]->setPen(pen);
        }
    }

    // Add series and create month buttons
    for (int j = 0; j < series.size(); ++j) {
        m_balanceChart->addSeries(series[j]);
        QPushButton* btn = new QPushButton(series[j]->name(), m_monthBtnsContainer);
        m_gridLayout->addWidget(btn, j / 6, j % 6);
        ButtonUtility::connectToggleActiveProperty(btn);
        btn->setProperty("active", true);
        QLineSeries* serie = series[j];
        connect(btn, &QPushButton::clicked, this, [btn, serie]() {
            bool isActive = btn->property("active").toBool();
            serie->setVisible(isActive);
        });
    }

    // Create axes
    m_balanceChart->createDefaultAxes();
    QValueAxis *axisX = qobject_cast<QValueAxis*>(m_balanceChart->axes(Qt::Horizontal).first());
    QValueAxis *axisY = qobject_cast<QValueAxis*>(m_balanceChart->axes(Qt::Vertical).first());
    configureAxis(axisX, "Day");
    configureAxis(axisY, "Balance");

    // Adjust the range of axes based on balance data
    auto minmax = std::minmax_element(balance.begin(), balance.end());
    int minValue = *minmax.first;
    int maxValue = *minmax.second;
    axisY->setRange(minValue, maxValue);
    axisX->setRange(1, 31);
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

TransactionsPage::~TransactionsPage()
{
    delete m_balanceChart;
}

void TransactionsPage::handleDelete(int row)
{
    m_transactionsModel->removeTransaction(row);
}
