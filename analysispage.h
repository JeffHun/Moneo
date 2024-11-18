#ifndef ANALYSISPAGE_H
#define ANALYSISPAGE_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSettings>
#include <QMap>
#include <QtCore>
#include <QtGui>
#include <QtCharts>

#include "transaction.h"

class AnalysisPage: public QWidget{
    Q_OBJECT

public:
    explicit AnalysisPage(QWidget* parent = nullptr);
    void setTransactions(QVector<Transaction> transactions);
    void setBudgets(QSettings* settings);
    void headerGeneration();

private :
    QVector<Transaction> m_transactions;
    QMap<QString, float> m_budgets;

    void graphGeneration(QVector<Transaction> transactions);
};

#endif // ANALYSISPAGE_H
