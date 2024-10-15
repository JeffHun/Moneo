#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <QDate>
#include <QFile>

class Transaction
{
public:
    Transaction(const QDate& aDate = QDate(), const QString& aDesc = "", double aAmount = 0.0, const QString& aCategory = "");

    QDate getDate() const;
    void setDate(const QDate& aDate);

    QString getDescription() const;
    void setDescription(const QString& aDesc);

    double getAmount() const;
    void setAmount(const double aAmount);

    QString getCategory() const;
    void setCategory(const QString& aCategory);

    static QStringList getAllowedCategories();

    static QString convertFRtoENCategories(const QString& FRcategory);

private:
    QDate m_date;
    QString m_description;
    double m_amount;
    QString m_category;
    static QStringList m_allowedCategories;
};

QList<Transaction> transactionGenerator(const QString& filePath);

#endif // TRANSACTION_H
