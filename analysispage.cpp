#include "analysispage.h"
#include <cmath>

AnalysisPage::AnalysisPage(QWidget* parent) : QWidget(parent) {
    QLabel* title = new QLabel("Analysis", this);
    title->setObjectName("title");
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(title, 0, Qt::AlignCenter);
}

void AnalysisPage::graphGeneration(QVector<Transaction> transactions)
{
    QBarSet *expenses = new QBarSet("Expenses");
    QBarSet *budget = new QBarSet("Budget");

    int max = 0;
    int incomesTotal = 0;
    int inflowsTotal = 0;
    int refundsTotal = 0;
    int foodsTotal = 0;
    int banksTotal = 0;
    int educationsTotal = 0;
    int taxesTotal = 0;
    int legalsTotal = 0;
    int housingsTotal = 0;
    int leisuresTotal = 0;
    int healthsTotal = 0;
    int shoppingsTotal = 0;
    int excludedsTotal = 0;
    int transportsTotal = 0;
    int outflowsTotal = 0;
    int savingsTotal = 0;

    QVector<Transaction> incomes;
    QVector<Transaction> inflows;
    QVector<Transaction> refunds;
    QVector<Transaction> foods;
    QVector<Transaction> banks;
    QVector<Transaction> educations;
    QVector<Transaction> taxes;
    QVector<Transaction> legals;
    QVector<Transaction> housings;
    QVector<Transaction> leisures;
    QVector<Transaction> healths;
    QVector<Transaction> shoppings;
    QVector<Transaction> excludeds;
    QVector<Transaction> transports;
    QVector<Transaction> outflows;
    QVector<Transaction> savings;

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

    *expenses <<
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

    *budget <<
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

    QBarSeries *series = new QBarSeries();
    series->append(expenses);
    series->append(budget);

    // Display vlaue inside bar
    series->setLabelsVisible(true);
    series->setLabelsFormat("@value");

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setAnimationOptions(QChart::SeriesAnimations);

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

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, max);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QWidget* iconContainer = new QWidget(chartView);
    QHBoxLayout* iconLayout = new QHBoxLayout(iconContainer);

    QVBoxLayout* mainLayout = qobject_cast<QVBoxLayout*>(layout());
    mainLayout->addWidget(chartView);
}

void AnalysisPage::headerGeneration()
{
    // Check if m_transactions is empty
    if (m_transactions.isEmpty())
        return;

    QVBoxLayout* mainLayout = qobject_cast<QVBoxLayout*>(layout());
    if (!mainLayout) {
        mainLayout = new QVBoxLayout(this);
        setLayout(mainLayout);
    }

    QWidget* monthBtnsContainer = new QWidget(this);

    QVector<QVector<Transaction>> months;
    QVector<Transaction> month;
    int currentMonth = m_transactions[0].getDate().month();

    // Group transactions by month
    for (Transaction& t : m_transactions)
    {
        if (t.getDate().month() == currentMonth) {
            month.append(t);
        } else {
            months.append(month);
            month.clear();
            month.append(t);
            currentMonth = t.getDate().month();
        }
    }
    if (!month.isEmpty())
        months.append(month);

    // Create btn for each month and place them in a grid
    QGridLayout* grid = new QGridLayout();
    monthBtnsContainer->setLayout(grid);

    for (int i = 0; i < months.count(); ++i) {
        QString name = QString::number(months[i].at(0).getDate().month()) + "/" +
                       QString::number(months[i].at(0).getDate().year());
        QPushButton* btn = new QPushButton(name, monthBtnsContainer);
        grid->addWidget(btn, i / 12, i % 12);
        connect(btn, &QPushButton::clicked, [this, months, i]() {
            graphGeneration(months[i]);
        });
    }

    mainLayout->addWidget(monthBtnsContainer);
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
