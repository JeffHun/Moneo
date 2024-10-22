#include "transaction.h"
#include <QMessageBox>

Transaction::Transaction(const QDate& aDate, const QString& aDesc, double aAmount, const QString& aCategory)
    : m_date(aDate), m_description(aDesc), m_amount(aAmount), m_category(aCategory) {}

const QString DATE_COLUMN = "Date de comptabilisation";
const QString DESCRIPTION_COLUMN = "Libelle operation";
const QString CATEGORY_COLUMN = "Categorie";
const QString DEBIT_COLUMN = "Debit";
const QString CREDIT_COLUMN = "Credit";

QStringList Transaction::m_allowedCategories = {"Food",
                                              "Bank and insurance",
                                              "Education and Family",
                                              "Taxes and duties",
                                              "Legal and Administrative",
                                              "Housing and home",
                                              "Leisure and vacations",
                                              "Health",
                                              "Shopping and services",
                                              "Excluded and transaction",
                                              "Transports",
                                              "Outflow of money",
                                              "Saving",
                                              "Income and earnings",
                                              "Inflow of money",
                                              "Refund",
                                              "Other"};

QList<Transaction> transactionGenerator(const QString& filePath) {
    QMessageBox messageBox;
    messageBox.setFixedSize(500,200);

    QList<Transaction> transactions;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        messageBox.critical(0,"Error", "Impossible to open the file : " + filePath);
        return transactions;
    }

    QTextStream in(&file);

    // Store each headers values separated by ';'
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

        messageBox.critical(0,"Error", "Required columns are missing in the file: " + filePath);
        return transactions;
    }

    int lineNbr = 0;

    // Read line by line
    while (!in.atEnd()) {

        lineNbr++;

        QStringList items = in.readLine().split(';');

        if (items.size() < headers.size()) {
            messageBox.critical(0,"Error", "File " + filePath + " line " + QString::number(lineNbr) + " has an incrorrect number of columns");
            continue;
        }

        // Parsing items values

        QDate date = QDate::fromString(items[columnIndices[DATE_COLUMN]], "dd/MM/yyyy");
        // Check if date is a valid value
        if (!date.isValid()) {
            messageBox.critical(0,"Error", "Invalid date in the file : " + filePath + " on the line : " + QString::number(lineNbr));
            continue;
        }

        QString description = items[columnIndices[DESCRIPTION_COLUMN]];
        // Check if description is a valid value
        if (description.isEmpty()) {
            messageBox.critical(0,"Error", "Empty description in the file : " + filePath + " on the line : " + QString::number(lineNbr));
            continue;
        }

        // Check if amount is a valid value
        double amount = items[columnIndices[DEBIT_COLUMN]].isEmpty() ?
                            items[columnIndices[CREDIT_COLUMN]].replace(",", ".").toDouble() :
                            items[columnIndices[DEBIT_COLUMN]].replace(",", ".").toDouble();
        if(amount == 0)
        {
            messageBox.critical(0,"Error", "Invalid amount in the file : " + filePath + " on the line : " + QString::number(lineNbr));
            continue;
        }

        QString category = items[columnIndices[CATEGORY_COLUMN]];
        category = Transaction::convertFRtoENCategories(category);

        // Check if category is a valid value
        if (category.isEmpty()) {
            messageBox.critical(0,"Error", "Empty category in file : " + filePath + " on the line : " + QString::number(lineNbr));
            continue;
        }

        // Create new Transaction and add it to transactions list
        transactions.append(Transaction(date, description, amount, category));
    }

    file.close();
    return transactions;
}

QString Transaction::convertFRtoENCategories(const QString& FRcategory)
{
    static const QMap<QString, QString> categoryMap = {
        {"Alimentation", "Food"},
        {"Banque et assurances", "Bank and insurance"},
        {"Education et famille", "Education and family"},
        {"Epargne", "Saving"},
        {"Impots et taxes", "Taxes and duties"},
        {"Juridique et administratif", "Legal and administrative"},
        {"Logement - maison", "Housing and home"},
        {"Loisirs et vacances", "Leisure and vacations"},
        {"Revenus et rentrees d'argent", "Income and earnings"},
        {"Sante", "Health"},
        {"Shopping et services", "Shopping and services"},
        {"Transaction exclue", "Excluded and transaction"},
        {"Transports", "Transports"},
        {"A categoriser - sortie d'argent", "Outflow of money"},
        {"A categoriser - rentree d'argent", "Inflow of money"}
    };

    return categoryMap.value(FRcategory, "Other");
}

QDate Transaction::getDate() const {return m_date;}
void Transaction::setDate(const QDate& aDate) {m_date = aDate;}

QString Transaction::getDescription() const {return m_description;}
void Transaction::setDescription(const QString& aDesc) {m_description = aDesc;}

double Transaction::getAmount() const {return m_amount;}
void Transaction::setAmount(double aAmount) {m_amount = aAmount;}

QString Transaction::getCategory() const {return m_category;}
void Transaction::setCategory(const QString& aCategory) {m_category = aCategory;}

QStringList Transaction::getAllowedCategories() {return m_allowedCategories;}
