#include "financecontroller.h"

FinanceController::FinanceController(FinanceModel* model, QObject* parent)
    : QObject(parent), m_model(model){}

void FinanceController::addTransaction(const Transaction& transaction) {
    m_model->addTransaction(transaction);
}

void FinanceController::removeTransaction(int row) {
    m_model->removeTransaction(row);
}

void FinanceController::updateTransaction(int row, const Transaction& transaction) {
    m_model->updateTransaction(row, transaction);
}
