#ifndef BUDGETCATEGORY_H
#define BUDGETCATEGORY_H

#include <QString>
#include <QWidget>
#include <QtCharts>

class BudgetCategory : public QWidget
{
public:
    explicit BudgetCategory(QWidget* parent, const QString& imgPath, const QString& name, const QString& color, QPieSlice* slice, float value, QLabel* expenseExemple);
    void updateSlice();
    void updatePercentageLabel();
    void setBudgetCategories(QList<BudgetCategory*>);
    float getValue();

protected:
    bool event(QEvent*) override;

private:
    QString m_imgPath;
    QString m_name;
    QString m_color;
    QPieSlice* m_slice;
    QLabel* m_percentage;
    QLineEdit* m_lineEdit;
    float m_value;
    QList<BudgetCategory*> m_budgetCategories;
    QLabel* m_expenseExemple;

    void setup();
    void updateExpenseExemple();
};

#endif // BUDGETCATEGORY_H
