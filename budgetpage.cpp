#include "budgetPage.h"

BudgetPage::BudgetPage(QWidget* parent) : QWidget(parent) {
    QLabel* title = new QLabel("Budget", this);
    title->setObjectName("title");
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(title);
}
