#ifndef FINANCECONTROLLER_H
#define FINANCECONTROLLER_H

#include <QObject>
#include "financemodel.h"

class FinanceController : public QObject
{
    Q_OBJECT

public:
    FinanceController(FinanceModel* model, QObject* parent = nullptr);

public slots:
    void addTransaction(const Transaction& transaction);
    void removeTransaction(int row);
    void updateTransaction(int row, const Transaction& transaction);

private:
    FinanceModel* m_model;
};

#endif // FINANCECONTROLLER_H
