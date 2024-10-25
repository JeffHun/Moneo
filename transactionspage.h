#ifndef TRANSACTIONSPAGE_H
#define TRANSACTIONSPAGE_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QTableView>
#include <QtCore>
#include <QtGui>
#include <QtCharts>
#include <vector>

#include "transactionmodel.h"
#include "transactiondelegate.h"
#include "fileDropZone.h"


class TransactionsPage: public QWidget{
    Q_OBJECT

private slots:
    void handleDelete(int row);
    void onDeleteClicked(int row);

public:
    explicit TransactionsPage(QWidget* parent = nullptr);

private:
    QTableView* m_transactionsView;
    TransactionModel* m_transactionsModel;
    TransactionDelegate* m_transactionsDelegate;
    QHBoxLayout* m_tableGraphLayout;
    QChartView* m_chartView;

    void loadTransactionsFromFilesAndSetupModel(QList<QFile*> files);
    void resetModel();

    void setupUI();
    void setupTableView();
    void setupLineView();
    void createGraphLine(QMap<QDate, int> balance);
    void configureAxis(QValueAxis *axis, const QString &title);
    void loadTransactionsFromFiles(QList<QFile*> files, QList<Transaction>& transactions);
    void showError(const QString& message);
    void updateBalanceAndModel(QList<Transaction>& transactions);
    QColor generateRandomColor();
};

#endif // TRANSACTIONSPAGE_H
