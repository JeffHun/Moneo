#ifndef TRANSACTIONSWINDOW_H
#define TRANSACTIONSWINDOW_H

#include "transaction.h"

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QScrollArea>

class TransactionsWindow : public QWidget{
    Q_OBJECT
public:
    explicit TransactionsWindow(QWidget *parent = nullptr, QList<Transaction> transactions = QList<Transaction>());

private :
};

#endif // TRANSACTIONSWINDOW_H
