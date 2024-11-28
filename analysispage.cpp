#include "analysispage.h"
#include "buttonutility.h"

#include <cmath>

AnalysisPage::AnalysisPage(QWidget* parent) : QWidget(parent){
    QLabel* title = new QLabel("Analysis", this);
    title->setObjectName("title");
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(title, 0, Qt::AlignCenter);
    m_monthBtnsContainer = new QWidget(this);

    m_axisX = new QBarCategoryAxis();
    m_axisY = new QValueAxis();

    m_chart = new QChart();
    m_chart->setAnimationOptions(QChart::SeriesAnimations);

    m_chartView = new QChartView(m_chart, this);
    m_chartView->setRenderHint(QPainter::Antialiasing);

    m_chart->setBackgroundBrush(Qt::NoBrush);
    m_chart->setBackgroundPen(Qt::NoPen);
    m_chart->setPlotAreaBackgroundBrush(Qt::NoBrush);
    m_chart->setPlotAreaBackgroundVisible(false);

    layout->addWidget(m_monthBtnsContainer);
    layout->addWidget(m_chartView);
    m_chartView->setChart(m_chart);
}

void AnalysisPage::graphGeneration(int i)
{
    QList<Transaction> transactions = m_months[i];

    // Initialize the layout
    QVBoxLayout* mainLayout = qobject_cast<QVBoxLayout*>(layout());
    if (!mainLayout) {
        mainLayout = new QVBoxLayout(this);
        setLayout(mainLayout);
    }

    // Clear existing series and axes
    m_chart->removeAllSeries();
    QList<QAbstractAxis*> axes = m_chart->axes();
    for (QAbstractAxis* axis : axes) {
        m_chart->removeAxis(axis);
    }

    // Create new series and bar sets
    m_expenses = new QBarSet("Expenses");
    m_budget = new QBarSet("Budget");
    m_series = new QBarSeries();

    int max = 0;
    int incomesTotal = 0, inflowsTotal = 0, refundsTotal = 0, foodsTotal = 0, banksTotal = 0, educationsTotal = 0, taxesTotal = 0, legalsTotal = 0, housingsTotal = 0, leisuresTotal = 0, healthsTotal = 0, shoppingsTotal = 0, excludedsTotal = 0, transportsTotal = 0, outflowsTotal = 0, savingsTotal = 0;

    QVector<Transaction> incomes, inflows, refunds, foods, banks, educations, taxes, legals, housings, leisures, healths, shoppings, excludeds, transports, outflows, savings;

    QMap<QString, QVector<Transaction>*> categoryMap = {
        {"Income and earnings", &incomes},
        {"Inflow of money", &inflows},
        {"Refund", &refunds},
        {"Food", &foods},
        {"Bank and insurance", &banks},
        {"Education and Family", &educations},
        {"Taxes and duties", &taxes},
        {"Legal and Administrative", &legals},
        {"Housing and home", &housings},
        {"Leisure and vacations", &leisures},
        {"Health", &healths},
        {"Shopping and services", &shoppings},
        {"Excluded and transaction", &excludeds},
        {"Transports", &transports},
        {"Outflow of money", &outflows},
        {"Saving", &savings}
    };

    QMap<QString, int*> totalMap = {
        {"Income and earnings", &incomesTotal},
        {"Inflow of money", &inflowsTotal},
        {"Refund", &refundsTotal},
        {"Food", &foodsTotal},
        {"Bank and insurance", &banksTotal},
        {"Education and Family", &educationsTotal},
        {"Taxes and duties", &taxesTotal},
        {"Legal and Administrative", &legalsTotal},
        {"Housing and home", &housingsTotal},
        {"Leisure and vacations", &leisuresTotal},
        {"Health", &healthsTotal},
        {"Shopping and services", &shoppingsTotal},
        {"Excluded and transaction", &excludedsTotal},
        {"Transports", &transportsTotal},
        {"Outflow of money", &outflowsTotal},
        {"Saving", &savingsTotal}
    };

    for (int i = 0; i < transactions.count(); i++) {
        QString category = transactions[i].getCategory();
        if (categoryMap.contains(category)) {
            categoryMap[category]->append(transactions[i]);

            if (category != "Income and earnings" && category != "Inflow of money" && category != "refunds") {
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

    // Display vlaue inside bar
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

    m_axisX = new QBarCategoryAxis();
    m_axisX->append(categories);
    m_chart->addAxis(m_axisX, Qt::AlignBottom);
    m_series->attachAxis(m_axisX);

    m_axisY = new QValueAxis();
    m_axisY->setRange(0, max);
    m_chart->addAxis(m_axisY, Qt::AlignLeft);
    m_series->attachAxis(m_axisY);

    m_chartView->setChart(m_chart);
    m_chartView->setRenderHint(QPainter::Antialiasing);
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
            graphGeneration(0);
        }
        grid->addWidget(btn, i / 12, i % 12);
        m_btns.append(btn);

        connect(btn, &QPushButton::clicked, [this, i]() {
            graphGeneration(i);
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
