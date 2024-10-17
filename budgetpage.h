#ifndef BUDGETPAGE_H
#define BUDGETPAGE_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class BudgetPage: public QWidget{
    Q_OBJECT

public:
    explicit BudgetPage(QWidget* parent = nullptr);
};

#endif // BUDGETPAGE_H
