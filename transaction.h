#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <QDate>
#include <QFile>

class Transaction
{
public:
    Transaction(const QDate& aDate, const QString& aDesc, double aAmount, const QString& aCategory);

    QDate getDate() const;
    QString getDescription() const;
    double getAmount() const;
    QString getCategory() const;

private:
    QDate date;
    QString description;
    double amount;
    QString category;
};

QList<Transaction> transactionGenerator(const QString& filePath);

#endif // TRANSACTION_H
