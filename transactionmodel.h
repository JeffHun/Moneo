#ifndef TRANSACTIONMODEL_H
#define TRANSACTIONMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include "transaction.h"

class TransactionModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit TransactionModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    void addTransaction(const Transaction& transaction);
    void removeTransaction(int row);
    Transaction getTransaction(int row) const;
    QVector<Transaction> getTransactions() const;

    void resetTransactions();

    void clear();

private:
    QVector<Transaction> m_transactions;
};

#endif // TRANSACTIONMODEL_H
