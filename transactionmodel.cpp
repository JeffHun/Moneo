#include "transactionmodel.h"

TransactionModel::TransactionModel(QObject* parent)
    : QAbstractTableModel(parent)
{
}

int TransactionModel::rowCount(const QModelIndex & /*parent*/) const
{
    return m_transactions.size();
}

int TransactionModel::columnCount(const QModelIndex & /*parent*/) const
{
    // There is 4 column for 4 attributs + 1 column for delete btn
    return 5;
}

QVariant TransactionModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_transactions.size() || index.row() < 0)
        return QVariant();

    const Transaction &transaction = m_transactions.at(index.row());

    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        switch (index.column())
        {
        case 0:
            return transaction.getDate();
        case 1:
            return transaction.getDescription();
        case 2:
            return transaction.getAmount();
        case 3:
            return transaction.getCategory();
        case 4:
            return QString("Delete");
        default:
            return QVariant();
        }
    }

    return QVariant();
}

QVariant TransactionModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
    {
        switch (section)
        {
        case 0:
            return tr("Date");
        case 1:
            return tr("Description");
        case 2:
            return tr("Amount");
        case 3:
            return tr("Category");
        case 4:
            return tr("Action");
        default:
            return QVariant();
        }
    }
    return QVariant();
}

bool TransactionModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole)
    {
        Transaction &transaction = m_transactions[index.row()];

        switch (index.column())
        {
        case 0:
            if (value.canConvert<QDate>())
                transaction.setDate(value.toDate());
            else
                return false;
            break;
        case 1:
            transaction.setDescription(value.toString());
            break;
        case 2:
            if (value.canConvert<double>())
                transaction.setAmount(value.toDouble());
            else
                return false;
            break;
        case 3:
            // Authorized category
            if (Transaction::getAllowedCategories().contains(value.toString()))
                transaction.setCategory(value.toString());
            else
                return false;
            break;
        default:
            return false;
        }

        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}

Qt::ItemFlags TransactionModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    if (index.column() == 4)
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    return Qt::ItemFlags(QAbstractTableModel::flags(index) | Qt::ItemIsEditable);
}

void TransactionModel::addTransaction(const Transaction& transaction)
{
    beginInsertRows(QModelIndex(), m_transactions.size(), m_transactions.size());
    m_transactions.append(transaction);
    endInsertRows();
}

void TransactionModel::removeTransaction(int row)
{
    if (row < 0 || row >= m_transactions.size())
        return;

    beginRemoveRows(QModelIndex(), row, row);
    m_transactions.removeAt(row);
    endRemoveRows();
}

Transaction TransactionModel::getTransaction(int row) const
{
    if (row < 0 || row >= m_transactions.size())
        return Transaction();

    return m_transactions.at(row);
}

QVector<Transaction> TransactionModel::getTransactions() const {
    return m_transactions;
}

void TransactionModel::clear()
{
    m_transactions.clear();
}

void TransactionModel::resetTransactions()
{
    beginResetModel();
    m_transactions.clear();
    endResetModel();
}
