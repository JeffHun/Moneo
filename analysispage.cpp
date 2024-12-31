#include "analysispage.h"
#include "buttonutility.h"
#include "transactionsWindow.h"

#include <cmath>

AnalysisPage::AnalysisPage(QWidget* parent) : QWidget(parent){
    QLabel* title = new QLabel("Analysis", this);
    title->setObjectName("title");
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(title, 0, Qt::AlignCenter);
    m_monthBtnsContainer = new QWidget(this);

    m_axisX = new QValueAxis();
    m_axisY = new QBarCategoryAxis();

    m_globalAxisX = new QBarCategoryAxis();
    m_globalAxisY = new QValueAxis();

    m_chart = new QChart();
    m_chart->setBackgroundBrush(QBrush(QColor("#15131B")));
    m_chart->setAnimationOptions(QChart::SeriesAnimations);

    m_globalChart = new QChart();

    m_chartView = new QChartView(m_chart, this);
    m_chartView->setBackgroundBrush(QBrush(QColor("#15131B")));
    m_chartView->setRenderHint(QPainter::Antialiasing);

    m_globalChartView = new QChartView(m_globalChart, this);
    m_globalChartView->setMinimumWidth(600);

    layout->addWidget(m_monthBtnsContainer);

    m_leftOverWidget = new QWidget(this);
    m_leftOverWidget->setObjectName("leftOverWidgetRed");
    QHBoxLayout* leftOverLayout = new QHBoxLayout(this);
    m_leftOverWidget->setLayout(leftOverLayout);
    m_leftOverLabel = new QLabel("?", m_leftOverWidget);
    leftOverLayout->addWidget(m_leftOverLabel);
    m_leftOverLabel->setObjectName("text");
    m_leftOverLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_leftOverWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    layout->addWidget(m_leftOverWidget, 0, Qt::AlignHCenter);

    QWidget *graphWidget = new QWidget(this);
    m_graphLayout = new QHBoxLayout();
    graphWidget->setLayout(m_graphLayout);

    m_globalChartView->setChart(m_globalChart);
    m_graphLayout->addWidget(m_globalChartView);

    m_globalChartView->setChart(m_globalChart);
    m_graphLayout->addWidget(m_chartView);

    layout->addWidget(graphWidget);
    categoryBtnsGeneration();
}

void AnalysisPage::budgetGraph()
{
    QList<Transaction> transactions = m_months[m_monthIdx];

    // Clear existing series and axes
    m_chart->removeAllSeries();
    QList<QAbstractAxis*> axes = m_chart->axes();
    for (QAbstractAxis* axis : axes) {
        m_chart->removeAxis(axis);
    }

    // Create new series and bar sets
    m_expenses = new QBarSet("Expenses");
    m_expenses->setColor(QColor("#6954D7"));
    m_budget = new QBarSet("Budget");
    m_budget->setColor(QColor("#3D3558"));
    m_chart->legend()->setLabelColor(QColor("#D9D9D9"));
    m_chart->legend()->setAlignment(Qt::AlignTop);

    m_series = new QHorizontalBarSeries();
    m_series->setBarWidth(0.5);

    m_globalSeries = new QStackedBarSeries ();

    int max = 0;
    int incomesTotal = 0, inflowsTotal = 0, refundsTotal = 0, foodsTotal = 0, banksTotal = 0, educationsTotal = 0, taxesTotal = 0, legalsTotal = 0, housingsTotal = 0, leisuresTotal = 0, healthsTotal = 0, shoppingsTotal = 0, excludedsTotal = 0, transportsTotal = 0, outflowsTotal = 0, savingsTotal = 0;

    QVector<Transaction> incomes, inflows, refunds, foods, banks, educations, taxes, legals, housings, leisures, healths, shoppings, excludeds, transports, outflows, savings;

    QMap<QString, QVector<Transaction>*> categoryMap = {
        {"Income", &incomes},
        {"Inflow", &inflows},
        {"Refund", &refunds},
        {"Food", &foods},
        {"Bank", &banks},
        {"Education", &educations},
        {"Taxes", &taxes},
        {"Legal", &legals},
        {"Housing", &housings},
        {"Leisure", &leisures},
        {"Health", &healths},
        {"Shopping", &shoppings},
        {"Excluded", &excludeds},
        {"Transports", &transports},
        {"Outflow", &outflows},
        {"Saving", &savings}
    };

    QMap<QString, int*> totalMap = {
        {"Income", &incomesTotal},
        {"Inflow", &inflowsTotal},
        {"Refund", &refundsTotal},
        {"Food", &foodsTotal},
        {"Bank", &banksTotal},
        {"Education", &educationsTotal},
        {"Taxes", &taxesTotal},
        {"Legal", &legalsTotal},
        {"Housing", &housingsTotal},
        {"Leisure", &leisuresTotal},
        {"Health", &healthsTotal},
        {"Shopping", &shoppingsTotal},
        {"Excluded", &excludedsTotal},
        {"Transports", &transportsTotal},
        {"Outflow", &outflowsTotal},
        {"Saving", &savingsTotal}
    };

    for (int i = 0; i < transactions.count(); i++) {
        QString category = transactions[i].getCategory();
        if (categoryMap.contains(category)) {
            categoryMap[category]->append(transactions[i]);

            if (category != "Income" && category != "Inflow" && category != "Refund") {
                transactions[i].setAmount(std::abs(transactions[i].getAmount()));
            }

            *totalMap[category] += std::round(transactions[i].getAmount());
        }
    }

    QVector<int> totals;
    totals.append(foodsTotal);
    totals.append(excludedsTotal);
    totals.append(leisuresTotal);
    totals.append(educationsTotal);
    totals.append(shoppingsTotal);
    totals.append(transportsTotal);
    totals.append(savingsTotal);
    totals.append(outflowsTotal);
    totals.append(legalsTotal);
    totals.append(banksTotal);
    totals.append(housingsTotal);
    totals.append(healthsTotal);

    // Determine max for Y axis
    for(int i = 0; i < totals.count(); i++)
    {
        if(max < totals[i])
            max = totals[i];
    }

    if(max < m_budgets["food"])
        max = m_budgets["food"];
    if(max < m_budgets["excluded"])
        max = m_budgets["excluded"];
    if(max < m_budgets["leisure"])
        max = m_budgets["leisure"];
    if(max < m_budgets["education"])
        max = m_budgets["education"];
    if(max < m_budgets["shopping"])
        max = m_budgets["shopping"];
    if(max < m_budgets["taxes"])
        max = m_budgets["taxes"];
    if(max < m_budgets["transport"])
        max = m_budgets["transport"];
    if(max < m_budgets["saving"])
        max = m_budgets["saving"];
    if(max < m_budgets["outflow"])
        max = m_budgets["outflow"];
    if(max < m_budgets["legal"])
        max = m_budgets["legal"];
    if(max < m_budgets["bank"])
        max = m_budgets["bank"];
    if(max < m_budgets["housing"])
        max = m_budgets["housing"];
    if(max < m_budgets["health"])
        max = m_budgets["health"];

    *m_expenses <<
        foodsTotal <<
        excludedsTotal <<
        leisuresTotal <<
        educationsTotal <<
        shoppingsTotal <<
        taxesTotal <<
        transportsTotal <<
        savingsTotal <<
        outflowsTotal <<
        legalsTotal <<
        banksTotal <<
        housingsTotal <<
        healthsTotal;

    *m_budget <<
        m_budgets["food"] <<
        m_budgets["excluded"] <<
        m_budgets["leisure"] <<
        m_budgets["education"] <<
        m_budgets["shopping"] <<
        m_budgets["taxes"] <<
        m_budgets["transport"] <<
        m_budgets["saving"] <<
        m_budgets["outflow"] <<
        m_budgets["legal"] <<
        m_budgets["bank"] <<
        m_budgets["housing"] <<
        m_budgets["health"];

    m_series->append(m_expenses);
    m_series->append(m_budget);

    // Display value inside bar
    m_series->setLabelsVisible(true);
    m_series->setLabelsFormat("@value");

    m_chart->addSeries(m_series);
    m_chart->setAnimationOptions(QChart::SeriesAnimations);

    QStringList categories;
    categories <<
        "Food\n" + QString::number(std::floor(m_budgets["food"] - foodsTotal))<<
        "Excluded\n" + QString::number(std::floor(m_budgets["excluded"] - excludedsTotal))<<
        "Leisure\n" + QString::number(std::floor(m_budgets["leisure"] - leisuresTotal))<<
        "Education\n" + QString::number(std::floor(m_budgets["education"] - educationsTotal))<<
        "Shopping\n" + QString::number(std::floor(m_budgets["shopping"] - shoppingsTotal))<<
        "Taxes\n" + QString::number(std::floor(m_budgets["taxes"] - taxesTotal))<<
        "Transports\n" + QString::number(std::floor(m_budgets["transport"] - transportsTotal))<<
        "Saving\n" + QString::number(std::floor(m_budgets["saving"] - savingsTotal))<<
        "Outflow\n" + QString::number(std::floor(m_budgets["outflow"] - outflowsTotal))<<
        "Legal\n" + QString::number(std::floor(m_budgets["legal"] - legalsTotal))<<
        "Bank\n" + QString::number(std::floor(m_budgets["bank"] - banksTotal))<<
        "Housing\n" + QString::number(std::floor(m_budgets["housing"] - housingsTotal))<<
        "Health\n" + QString::number(std::floor(m_budgets["health"] - healthsTotal));

    m_axisX = new QValueAxis();
    m_axisX->setRange(0, max);
    m_axisX->setLabelsBrush(QBrush(QColor("#D9D9D9")));
    m_chart->addAxis(m_axisX, Qt::AlignBottom);
    m_series->attachAxis(m_axisX);

    m_axisY = new QBarCategoryAxis();
    m_axisY->append(categories);
    m_axisY->setLabelsBrush(QBrush(QColor("#D9D9D9")));
    m_chart->addAxis(m_axisY, Qt::AlignLeft);
    m_series->attachAxis(m_axisY);

    m_chartView->setChart(m_chart);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    m_graphLayout->addWidget(m_chartView);
}

void AnalysisPage::globalGraph()
{
    QList<Transaction> transactions = m_months[m_monthIdx];

    m_globalChart->removeAllSeries();
    QList<QAbstractAxis*> globalAxes = m_globalChart->axes();
    for (QAbstractAxis* axis : globalAxes) {
        m_globalChart->removeAxis(axis);
    }

    m_globalSeries = new QStackedBarSeries(this);

    QBarSet *foodBar = new QBarSet("Food");
    QBarSet *excludedBar = new QBarSet("Excluded");
    QBarSet *leisureBar = new QBarSet("Leisure");
    QBarSet *educationBar = new QBarSet("Education");
    QBarSet *shoppingBar = new QBarSet("Shopping");
    QBarSet *taxesBar = new QBarSet("Taxes");
    QBarSet *transportBar = new QBarSet("Transport");
    QBarSet *savingBar = new QBarSet("Saving");
    QBarSet *outflowBar = new QBarSet("Outflow");
    QBarSet *legalBar = new QBarSet("Legal");
    QBarSet *bankBar = new QBarSet("Bank");
    QBarSet *housingBar = new QBarSet("Housing");
    QBarSet *healthBar = new QBarSet("Health");
    QBarSet *refundBar = new QBarSet("Refund");
    QBarSet *incomeBar = new QBarSet("Income");
    QBarSet *inflowBar = new QBarSet("Inflow");

    foodBar->setColor("#B35652");
    excludedBar->setColor("#B3527C");
    leisureBar->setColor("#B352AE");
    educationBar->setColor("#7B52B3");
    shoppingBar->setColor("#5254B3");
    taxesBar->setColor("#527DB3");
    transportBar->setColor("#52A6B3");
    savingBar->setColor("#52B393");
    outflowBar->setColor("#52B363");
    legalBar->setColor("#71B352");
    bankBar->setColor("#9EB352");
    housingBar->setColor("#B3A752");
    healthBar->setColor("#B38952");
    refundBar->setColor("#535353");
    incomeBar->setColor("#767676");
    inflowBar->setColor("#A0A0A0");

    int incomesTotal = 0, inflowsTotal = 0, refundsTotal = 0, foodsTotal = 0, banksTotal = 0, educationsTotal = 0, taxesTotal = 0, legalsTotal = 0, housingsTotal = 0, leisuresTotal = 0, healthsTotal = 0, shoppingsTotal = 0, excludedsTotal = 0, transportsTotal = 0, outflowsTotal = 0, savingsTotal = 0;

    QVector<Transaction> incomes, inflows, refunds, foods, banks, educations, taxes, legals, housings, leisures, healths, shoppings, excludeds, transports, outflows, savings;

    QMap<QString, QVector<Transaction>*> categoryMap = {
        {"Income", &incomes},
        {"Inflow", &inflows},
        {"Refund", &refunds},
        {"Food", &foods},
        {"Bank", &banks},
        {"Education", &educations},
        {"Taxes", &taxes},
        {"Legal", &legals},
        {"Housing", &housings},
        {"Leisure", &leisures},
        {"Health", &healths},
        {"Shopping", &shoppings},
        {"Excluded", &excludeds},
        {"Transports", &transports},
        {"Outflow", &outflows},
        {"Saving", &savings}
    };

    QMap<QString, int*> totalMap = {
        {"Income", &incomesTotal},
        {"Inflow", &inflowsTotal},
        {"Refund", &refundsTotal},
        {"Food", &foodsTotal},
        {"Bank", &banksTotal},
        {"Education", &educationsTotal},
        {"Taxes", &taxesTotal},
        {"Legal", &legalsTotal},
        {"Housing", &housingsTotal},
        {"Leisure", &leisuresTotal},
        {"Health", &healthsTotal},
        {"Shopping", &shoppingsTotal},
        {"Excluded", &excludedsTotal},
        {"Transports", &transportsTotal},
        {"Outflow", &outflowsTotal},
        {"Saving", &savingsTotal}
    };

    for (int i = 0; i < transactions.count(); i++) {
        QString category = transactions[i].getCategory();
        if (categoryMap.contains(category)) {
            categoryMap[category]->append(transactions[i]);

            if (category != "Income" && category != "Inflow" && category != "Refund") {
                transactions[i].setAmount(std::abs(transactions[i].getAmount()));
            }

            *totalMap[category] += std::round(transactions[i].getAmount());
        }
    }

    *foodBar<<foodsTotal<<0;
    *excludedBar<<excludedsTotal<<0;
    *leisureBar<<leisuresTotal<<0;
    *educationBar<<educationsTotal<<0;
    *shoppingBar<<shoppingsTotal<<0;
    *taxesBar<<taxesTotal<<0;
    *transportBar<<transportsTotal<<0;
    *savingBar<<savingsTotal<<0;
    *outflowBar<<outflowsTotal<<0;
    *legalBar<<legalsTotal<<0;
    *bankBar<<banksTotal<<0;
    *housingBar<<housingsTotal<<0;
    *healthBar<<healthsTotal<<0;
    *refundBar<<0<<refundsTotal;
    *incomeBar<<0<<incomesTotal;
    *inflowBar<<0<<inflowsTotal;

    m_globalSeries->append(foodBar);
    m_globalSeries->append(excludedBar);
    m_globalSeries->append(leisureBar);
    m_globalSeries->append(educationBar);
    m_globalSeries->append(shoppingBar);
    m_globalSeries->append(taxesBar);
    m_globalSeries->append(transportBar);
    m_globalSeries->append(savingBar);
    m_globalSeries->append(outflowBar);
    m_globalSeries->append(legalBar);
    m_globalSeries->append(bankBar);
    m_globalSeries->append(housingBar);
    m_globalSeries->append(healthBar);
    m_globalSeries->append(refundBar);
    m_globalSeries->append(incomeBar);
    m_globalSeries->append(inflowBar);

    // Display value inside bar
    m_globalSeries->setLabelsVisible(true);
    m_globalSeries->setLabelsFormat("@value");

    m_globalChart->addSeries(m_globalSeries);
    m_globalChart->setAnimationOptions(QChart::SeriesAnimations);
    m_globalChart->setBackgroundBrush(QBrush(QColor("#15131B")));

    QStringList globalCategories = {"Expensed", "Earned"};
    m_globalAxisX->append(globalCategories);
    m_globalChart->legend()->hide();
    m_globalAxisX->setLabelsBrush(QBrush(QColor("#D9D9D9")));
    m_globalChart->addAxis(m_globalAxisX, Qt::AlignBottom);
    m_globalSeries->attachAxis(m_globalAxisX);

    float globalMax = foodsTotal +
                      excludedsTotal +
                      leisuresTotal +
                      educationsTotal +
                      shoppingsTotal +
                      taxesTotal +
                      transportsTotal +
                      savingsTotal +
                      outflowsTotal +
                      legalsTotal +
                      banksTotal +
                      housingsTotal +
                      healthsTotal;


    if(globalMax < (refundsTotal + incomesTotal + inflowsTotal))
        globalMax = refundsTotal + incomesTotal + inflowsTotal;
    m_globalAxisY->setRange(0, globalMax);
    m_globalAxisY->setLabelsBrush(QBrush(QColor("#D9D9D9")));
    m_globalChart->addAxis(m_globalAxisY, Qt::AlignLeft);

    m_globalChartView->setRenderHint(QPainter::Antialiasing);
    m_globalChartView->setBackgroundBrush(QBrush(QColor("#15131B")));

    m_globalChartView->setChart(m_globalChart);
    m_graphLayout->addWidget(m_globalChartView);

    float leftOverValue =  (refundsTotal +
                           incomesTotal +
                           inflowsTotal) -
                          (foodsTotal +
                           excludedsTotal +
                           leisuresTotal +
                           educationsTotal +
                           shoppingsTotal +
                           taxesTotal +
                           transportsTotal +
                           savingsTotal +
                           outflowsTotal +
                           legalsTotal +
                           banksTotal +
                           housingsTotal +
                           healthsTotal);

    m_leftOverLabel->setText("Balance :" + QString::number(leftOverValue));
    if(leftOverValue >= 0)
        m_leftOverWidget->setObjectName("leftOverWidgetGreen");
    else
        m_leftOverWidget->setObjectName("leftOverWidgetRed");

    m_leftOverWidget->style()->unpolish(m_leftOverWidget);
    m_leftOverWidget->style()->polish(m_leftOverWidget);
}

void AnalysisPage::categoryBtnsGeneration()
{
    QWidget* categoriesWidget = new QWidget(this);
    m_graphLayout->addWidget(categoriesWidget);
    QVBoxLayout* categoriesLayout = new QVBoxLayout(categoriesWidget);
    categoriesLayout->addStretch();
    createCategoryButton(categoriesWidget, categoriesLayout, "Inflow", ":/resources/style/img/inflow.png", "#A0A0A0");
    createCategoryButton(categoriesWidget, categoriesLayout, "Income", ":/resources/style/img/income.png", "#767676");
    createCategoryButton(categoriesWidget, categoriesLayout, "Refund", ":/resources/style/img/refund.png", "#535353");
    createCategoryButton(categoriesWidget, categoriesLayout, "Health", ":/resources/style/img/health.png", "#B38952");
    createCategoryButton(categoriesWidget, categoriesLayout, "Housing", ":/resources/style/img/housing.png", "#B3A752");
    createCategoryButton(categoriesWidget, categoriesLayout, "Bank", ":/resources/style/img/bank.png", "#9EB352");
    createCategoryButton(categoriesWidget, categoriesLayout, "Legal", ":/resources/style/img/legal.png", "#71B352");
    createCategoryButton(categoriesWidget, categoriesLayout, "Outflow", ":/resources/style/img/outflow.png", "#52B363");
    createCategoryButton(categoriesWidget, categoriesLayout, "Saving", ":/resources/style/img/saving.png", "#52B393");
    createCategoryButton(categoriesWidget, categoriesLayout, "Transports", ":/resources/style/img/transport.png", "#52A6B3");
    createCategoryButton(categoriesWidget, categoriesLayout, "Taxes", ":/resources/style/img/taxes.png", "#527DB3");
    createCategoryButton(categoriesWidget, categoriesLayout, "Shopping", ":/resources/style/img/shopping.png", "#5254B3");
    createCategoryButton(categoriesWidget, categoriesLayout, "Education", ":/resources/style/img/education.png", "#7B52B3");
    createCategoryButton(categoriesWidget, categoriesLayout, "Leisure", ":/resources/style/img/leisure.png", "#B352AE");
    createCategoryButton(categoriesWidget, categoriesLayout, "Excluded", ":/resources/style/img/excluded.png", "#B3527C");
    createCategoryButton(categoriesWidget, categoriesLayout, "Food", ":/resources/style/img/food.png", "#B35652");
}

void AnalysisPage::createCategoryButton(QWidget* parent,QVBoxLayout* layout, const QString& text, const QString& path, const QString& color)
{
    auto* button = new QPushButton(text, parent);
    button->setObjectName("btn");
    QIcon buttonIcn(path);
    button->setIcon(buttonIcn);
    button->setIconSize(QSize(15,15));
    button->setStyleSheet(QString("#btn{"
                                  "text-align: left;"
                                  "background-color: %1};").arg(color));
    button->setFixedWidth(125);
    layout->addWidget(button);
    connect(button, &QPushButton::clicked, this, [this, text](){
        showTransactionsCategory(text);
    });
}

void AnalysisPage::showTransactionsCategory(const QString& text)
{
    qDebug()<<text;
    QList<Transaction> transactions;
    for(int i = 0; i < m_months[m_monthIdx].count(); i++)
        if(m_months[m_monthIdx][i].getCategory() == text)
        {
            transactions.append(m_months[m_monthIdx][i]);
        }
    if(transactions.count()>0)
    {
        TransactionsWindow *transactionsWindow = new TransactionsWindow(nullptr, transactions);
        transactionsWindow->setWindowTitle("Transaction details");
        transactionsWindow->resize(400, 300);
        transactionsWindow->show();
    }
    else
    {
        QMessageBox messageBox;
        messageBox.setFixedSize(500,200);
        messageBox.critical(0,"Error", "No transaction for this category");
    }
}

void AnalysisPage::headerGeneration()
{
    // Check if m_transactions is empty
    if (m_transactions.isEmpty())
        return;

    // Remove last btns
    for(int i = 0; i < m_btns.count(); ++i)
    {
        m_btns[i]->disconnect();
        delete m_btns[i];
    }

    m_btns.clear();

    if (m_monthBtnsContainer->layout()) {
        QLayout* oldLayout = m_monthBtnsContainer->layout();
        QLayoutItem* item;
        while ((item = oldLayout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete oldLayout;
    }

    QVector<Transaction> month;
    int currentMonth = m_transactions[0].getDate().month();

    m_months.clear();

    // Group transactions by month
    for (Transaction& t : m_transactions)
    {
        if (t.getDate().month() == currentMonth) {
            month.append(t);
        } else {
            m_months.append(month);
            month.clear();
            month.append(t);
            currentMonth = t.getDate().month();
        }
    }
    if (!month.isEmpty())
        m_months.append(month);

    // Create btn for each month and place them in a grid
    QGridLayout* grid = new QGridLayout(m_monthBtnsContainer);
    m_monthBtnsContainer->setLayout(grid);

    for (int i = 0; i < m_months.count(); ++i) {
        QString name = QString::number(m_months[i].at(0).getDate().month()) + "/" +
                       QString::number(m_months[i].at(0).getDate().year());
        QPushButton* btn = new QPushButton(name, m_monthBtnsContainer);
        btn->setProperty("active", false);
        if(i == 0)
        {
            btn->setProperty("active", true);
            m_monthIdx = 0;
            globalGraph();
            budgetGraph();
        }
        grid->addWidget(btn, i / 12, i % 12);
        m_btns.append(btn);

        connect(btn, &QPushButton::clicked, [this, i]() {
            m_monthIdx = i;
            globalGraph();
            budgetGraph();
        });
    }

    for(int i = 0; i < m_btns.count(); i++)
    {
        connect(m_btns[i], &QPushButton::clicked, [this, i]() {
            ButtonUtility::connectUniqueToggleActiveProperty(m_btns, i);
        });
    }

    // Update the btns container visualization
    m_monthBtnsContainer->adjustSize();
    layout()->invalidate();
}

void AnalysisPage::setBudgets(QSettings* settings)
{
    QStringList categories = {"food", "excluded", "leisure", "education", "shopping", "taxes", "transport", "saving", "outflow", "legal", "bank", "housing", "health"};
    foreach (const QString &category, categories) {
        if(settings->contains(category))
            m_budgets[category] = settings->value(category).toFloat();
    }
}

void AnalysisPage::setTransactions(QVector<Transaction> transactions)
{
    m_transactions = transactions;
}
