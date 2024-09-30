#include "mainwindow.h"
#include <QApplication>
#include "transaction.h"
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    QString filePath = QDir::current().filePath("../Moneo/data/Test.csv");
    QList<Transaction> transactions = transactionGenerator(filePath);

    for (Transaction& t : transactions) {
        qDebug() << "Date:" << t.getDate().day() << "/" << t.getDate().month() << "/" << t.getDate().year() << " | "
                 << "Description:" << t.getDescription() << " | "
                 << "Amount:" << t.getAmount() << " | "
                 << "Catégory:" << t.getCategory() << "\n";
    }

    return a.exec();
}
