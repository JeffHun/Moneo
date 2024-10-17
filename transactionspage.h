#ifndef TRANSACTIONSPAGE_H
#define TRANSACTIONSPAGE_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QTableView>

#include "transactionmodel.h"
#include "transactiondelegate.h"


class TransactionsPage: public QWidget{
    Q_OBJECT

private slots:
    void handleDelete(int row);

public:
    explicit TransactionsPage(QWidget* parent = nullptr);

private:
    QTableView* m_tableView;
    TransactionModel* m_model;
    TransactionDelegate* m_delegate;
};

#endif // TRANSACTIONSPAGE_H
