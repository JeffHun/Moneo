#ifndef FINANCEMODEL_H
#define FINANCEMODEL_H

#include <QAbstractTableModel>
#include "transaction.h"
#include <vector>

class FinanceModel: public QAbstractTableModel
{
    Q_OBJECT

public:
    FinanceModel(QObject* parent = nullptr);

    // QAbstractTableModoel method
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    void addTransaction(const Transaction& transaction);
    void removeTransaction(int row);
    Transaction getTransaction(int row) const;
    void updateTransaction(int row, const Transaction& transaction);

private:
    std::vector<Transaction> transactions;
};

#endif // FINANCEMODEL_H
