#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "transactionmodel.h"
#include "transactiondelegate.h"

class QTableView;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleDelete(int row);

private:
    QTableView* tableView;
    TransactionModel* model;
    TransactionDelegate* delegate;
};

#endif // MAINWINDOW_H
