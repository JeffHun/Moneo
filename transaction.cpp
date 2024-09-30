#include "transaction.h"

Transaction::Transaction(const QDate& aDate, const QString& aDesc, double aAmount, const QString& aCategory)
    : date(aDate), description(aDesc), amount(aAmount), category(aCategory) {}


const QString DATE_COLUMN = "Date de comptabilisation";
const QString DESCRIPTION_COLUMN = "Libelle operation";
const QString CATEGORY_COLUMN = "Categorie";
const QString DEBIT_COLUMN = "Debit";
const QString CREDIT_COLUMN = "Credit";

QList<Transaction> transactionGenerator(const QString& filePath) {
    QList<Transaction> transactions;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Impossible to open the file :" << filePath;
        return transactions;
    }

    QTextStream in(&file);

    // Strore each headers values separated by ';'
    QStringList headers  = in.readLine().split(';');

    // Initialize a map to store column indices
    QMap<QString, int> columnIndices;

    // Determine the slot of the values
    for (int i = 0; i < headers.count(); i++) {
        columnIndices[headers[i]] = i;
    }

    // Validate required columns
    if (!columnIndices.contains(DATE_COLUMN) ||
        !columnIndices.contains(DESCRIPTION_COLUMN) ||
        !columnIndices.contains(CATEGORY_COLUMN) ||
        !columnIndices.contains(DEBIT_COLUMN) ||
        !columnIndices.contains(CREDIT_COLUMN)) {
        qWarning() << "Required columns are missing in the file:" << filePath;
        return transactions;
    }

    int lineNbr = 0;

    // Read line by line
    while (!in.atEnd()) {

        lineNbr++;

        QStringList items = in.readLine().split(';');

        if (items.size() < headers.size()) {
            qWarning() << "Line" << lineNbr << "has an incorrect number of columns.";
            continue;
        }

        // Parsing items values

        QDate date = QDate::fromString(items[columnIndices[DATE_COLUMN]], "dd/MM/yyyy");
        // Check if date is a valid value
        if (!date.isValid()) {
            qWarning() << "Invalid date in the file : " << filePath << " on the line : " << lineNbr;
            continue;
        }

        QString description = items[columnIndices[DESCRIPTION_COLUMN]];
        // Check if description is a valid value
        if (description.isEmpty()) {
            qWarning() << "Empty description in the file : " << filePath << " on the line : " << lineNbr;
            continue;
        }

        // Check if amount is a valid value
        double amount = items[columnIndices[DEBIT_COLUMN]].isEmpty() ?
                            items[columnIndices[CREDIT_COLUMN]].replace(",", ".").toDouble() :
                            items[columnIndices[DEBIT_COLUMN]].replace(",", ".").toDouble();
        if(amount == 0)
        {
            qWarning() << "Invalid amount in the file : " << filePath << " on the line : " << lineNbr;
            continue;
        }

        QString category = items[columnIndices[CATEGORY_COLUMN]];
        // Check if category is a valid value
        if (category.isEmpty()) {
            qWarning() << "Empty category in file : " << filePath << " on the line : " << lineNbr;
            continue;
        }

        // Create new Transaction and add it to transactions list
        transactions.append(Transaction(date, description, amount, category));
    }

    file.close();
    return transactions;
}

QDate Transaction::getDate() const
{
    return date;
}

QString Transaction::getDescription() const
{
    return description;
}

double Transaction::getAmount() const
{
    return amount;
}

QString Transaction::getCategory() const
{
    return category;
}
