#ifndef BUDGETPAGE_H
#define BUDGETPAGE_H

#include "budgetcategory.h"

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QSettings>

class BudgetPage: public QWidget{
    Q_OBJECT

public:
    explicit BudgetPage(QWidget* parent = nullptr);
    void setBudgetCategoryValues();
    QSettings* getSettings();

private:
    QList<int> m_budgetCategoriesValues;
    QSettings* m_settings;
    QList<BudgetCategory*> m_budgetCategories;
    QLabel* m_total;

    void getBudgetCategoryValues();
    void setupUI();
    void setupChart();
    void createCategories(QGridLayout* layout, QWidget* container, QLabel* expenseExampleLabel, QPieSeries* series);

public slots:
    void updateTotalValue();
};

#endif // BUDGETPAGE_H
