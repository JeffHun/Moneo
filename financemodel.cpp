#include "financemodel.h"

FinanceModel::FinanceModel(QObject* parent) :
    QAbstractTableModel(parent){}

int FinanceModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return static_cast<int>(transactions.size());
}

int FinanceModel::columnCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return 4; // There is 4 attribut by Transaction (date, description, amount, category)
}

QVariant FinanceModel::data(const QModelIndex& index, int role) const
{
    if(!index.isValid() || index.row() >= static_cast<int>(transactions.size()) || role != Qt::DisplayRole)
        return QVariant();

    const Transaction& transaction = transactions.at(index.row());

    switch(index.column())
    {
    case 0:
        return transaction.getDate().toString("dd-MM-yyyy");
    case 1:
        return transaction.getDescription();
    case 2:
        return transaction.getAmount();
    case 3:
        return transaction.getCategory();
    default:
        return QVariant();
    }
}

QVariant FinanceModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant();

    if(orientation == Qt::Horizontal)
    {
        switch (section){
        case 0:
            return QString("Date");
        case 1:
            return QString("Description");
        case 2:
            return QString("Amount");
        case 3:
            return QString("Category");
        default:
            return QVariant();
        }
    }
    return QVariant();
}

void FinanceModel::addTransaction(const Transaction& transaction)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    transactions.push_back(transaction);
    endInsertRows();
}

void FinanceModel::removeTransaction(int row)
{
    if(row < 0 || row >= rowCount())
        return;

    beginRemoveRows(QModelIndex(), row, row);
    transactions.erase(transactions.begin() + row);
    endRemoveRows();
}


Transaction FinanceModel::getTransaction(int row) const
{
    if(row < 0 || row >= rowCount())
        return Transaction();

    return transactions.at(row);
}

void FinanceModel::updateTransaction(int row, const Transaction& transaction)
{
    if(row < 0 || row >= rowCount())
        return;

    transactions[row] = transaction;
    emit dataChanged(index(row, 0), index(row, columnCount() - 1));
}
























