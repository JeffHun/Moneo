#include "budgetPage.h"

#include <QtCharts>
#include "budgetcategory.h"

BudgetPage::BudgetPage(QWidget* parent) : QWidget(parent) {
    m_settings = new QSettings("JeffHun", "Moneo");

    getBudgetCategoryValues();

    QLabel* title = new QLabel("Budget", this);
    title->setObjectName("bigTitle");
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(title);
    m_total = new QLabel("?", this);
    layout->addWidget(m_total);
    m_total->setObjectName("text");
    m_total->setAlignment(Qt::AlignCenter);
    QWidget* budgetContainer = new QWidget(this);
    QHBoxLayout* budgetLayout = new QHBoxLayout(budgetContainer);
    layout->addWidget(budgetContainer);
    title->setAlignment(Qt::AlignCenter);
    QLabel* expenseExempleLabel = new QLabel(this);
    expenseExempleLabel->setObjectName("text");
    QWidget *budgetingCategoriesContainer = new QWidget(this);
    QGridLayout *budgetingCategoriesLayout = new QGridLayout(budgetingCategoriesContainer);


    QPieSeries *series = new QPieSeries();
    series->append("", 0)->setBrush(QColor(179, 86, 82));
    series->append("", 0)->setBrush(QColor(179, 82, 124));
    series->append("", 0)->setBrush(QColor(179, 82, 174));
    series->append("", 0)->setBrush(QColor(123, 82, 179));
    series->append("", 0)->setBrush(QColor(82, 84, 179));
    series->append("", 0)->setBrush(QColor(82, 125, 179));
    series->append("", 0)->setBrush(QColor(82, 166, 179));
    series->append("", 0)->setBrush(QColor(82, 179, 147));
    series->append("", 0)->setBrush(QColor(82, 179, 99));
    series->append("", 0)->setBrush(QColor(113, 179, 82));
    series->append("", 0)->setBrush(QColor(158, 179, 82));
    series->append("", 0)->setBrush(QColor(179, 167, 82));
    series->append("", 0)->setBrush(QColor(179, 137, 82));
    series->setLabelsVisible(false);
    series->setHoleSize(0.5);
    series->setPieSize(0.87);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setMargins(QMargins(0, 0, 0, 0));
    chart->legend()->hide();
    chart->setBackgroundBrush(Qt::NoBrush);

    QChartView *chartview = new QChartView(chart);
    chartview->setRenderHint(QPainter::Antialiasing);
    chartview->setObjectName("budgetChartView");

    createCategories(budgetingCategoriesLayout, budgetingCategoriesContainer, expenseExempleLabel, series);

    budgetLayout->addWidget(budgetingCategoriesContainer);
    budgetLayout->addWidget(chartview);
    layout->addWidget(expenseExempleLabel, 0, Qt::AlignCenter);

    for(int i=0; i<m_budgetCategories.count(); i++)
    {
        m_budgetCategories[i]->updateSlice();
        m_budgetCategories[i]->updatePercentageLabel();
    }

    updateTotalValue();
}

void BudgetPage::updateTotalValue() {
    float totalValue = 0;
    for (BudgetCategory* category : m_budgetCategories) {
        totalValue += category->getValue();
    }
    m_total->setText(QString("Total: %1").arg(totalValue, 0, 'f', 2));
}

void BudgetPage::createCategories(QGridLayout* layout, QWidget* container, QLabel* expenseExampleLabel, QPieSeries* series) {
    QStringList categoryNames = {"Food\n",
                                 "Excluded\ntransaction",
                                 "Leisure &\nholidays",
                                 "Education &\nfamily",
                                 "Shopping &\nservices",
                                 "Taxes &\nduties",
                                 "Transport\n",
                                 "Saving\n",
                                 "Outflow of\nmoney",
                                 "Legal &\nadministrative",
                                 "Bank &\ninsurance",
                                 "Housing\n",
                                 "Health\n"};
    QStringList icons = {":/resources/style/img/food.png",
                         ":/resources/style/img/excluded.png",
                         ":/resources/style/img/leisure.png",
                         ":/resources/style/img/education.png",
                         ":/resources/style/img/shopping.png",
                         ":/resources/style/img/taxes.png",
                         ":/resources/style/img/transport.png",
                         ":/resources/style/img/saving.png",
                         ":/resources/style/img/outflow.png",
                         ":/resources/style/img/legal.png",
                         ":/resources/style/img/bank.png",
                         ":/resources/style/img/housing.png",
                         ":/resources/style/img/health.png"};
    QStringList colors = {"#B35652", "#B3527C", "#B352AE", "#7B52B3", "#5254B3", "#527DB3", "#52A6B3", "#52B393",
                          "#52B363", "#71B352", "#9EB352", "#B3A752", "#B38952"};

    // Create BudgetCategory and place it
    for (int i = 0; i < categoryNames.size(); ++i) {
        BudgetCategory *category = new BudgetCategory(container, icons[i], categoryNames[i], colors[i], series->slices().at(i), m_budgetCategoriesValues[i], expenseExampleLabel);
        m_budgetCategories.append(category);
        layout->addWidget(category, i / 5, i % 5);
    }

    // Share BudgetCategory list to each budgetCategory
    for(BudgetCategory* category: m_budgetCategories)
    {
        category->setBudgetCategories(m_budgetCategories);
    }

    // Connect each budgetCategory signal to budgetPage slot
    for (BudgetCategory* category : m_budgetCategories) {
        connect(category, &BudgetCategory::valueChanged, this, &BudgetPage::updateTotalValue);
    }
}

QSettings* BudgetPage::getSettings()
{
    return m_settings;
}

void BudgetPage::getBudgetCategoryValues()
{
    QStringList categories = {"food", "excluded", "leisure", "education", "shopping", "taxes", "transport", "saving", "outflow", "legal", "bank", "housing", "health"};
    foreach (const QString &category, categories) {
        if(m_settings->contains(category))
            m_budgetCategoriesValues.append(m_settings->value(category).toFloat());
        else {
            m_settings->setValue(category, "0");
            m_budgetCategoriesValues.append(0.0f);
        }
    }
}

void BudgetPage::setBudgetCategoryValues()
{
    QStringList categories = {"food", "excluded", "leisure", "education", "shopping", "taxes", "transport",
                              "saving", "outflow", "legal", "bank", "housing", "health"};

    for(int i = 0; i < m_budgetCategories.count(); i++)
        m_settings->setValue(categories[i], m_budgetCategories[i]->getValue());
}
