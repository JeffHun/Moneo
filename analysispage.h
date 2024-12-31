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
    QWidget* m_monthBtnsContainer;
    QBarSet* m_expenses;
    QBarSet* m_budget;
    QHorizontalBarSeries* m_series;
    QChart *m_chart = nullptr;
    QValueAxis* m_axisX;
    QBarCategoryAxis* m_axisY;
    QChartView* m_chartView;
    QVector<QVector<Transaction>> m_months;
    QList<QPushButton*> m_btns;
    QHBoxLayout *m_graphLayout;
    QBarSet* m_globalExpenses;
    QBarSet* m_globalBudget;
    QStackedBarSeries * m_globalSeries;
    QChart* m_globalChart = nullptr;
    QBarCategoryAxis* m_globalAxisX;
    QValueAxis* m_globalAxisY;
    QChartView* m_globalChartView;
    QLabel* m_leftOverLabel;
    QWidget *m_leftOverWidget;
    int m_monthIdx;

    void globalGraphGeneration();
    void budgetGraph();
    void globalGraph();
    void categoryBtnsGeneration();
    void createCategoryButton(QWidget*,QVBoxLayout*, const QString&, const QString&, const QString&);
    void showTransactionsCategory(const QString&);
};

#endif // ANALYSISPAGE_H
