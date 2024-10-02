#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <QDate>
#include <QFile>

class Transaction
{
public:
    Transaction(const QDate& aDate = QDate(), const QString& aDesc = "", double aAmount = 0.0, const QString& aCategory = "");

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
