#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "financemodel.h"
#include "financecontroller.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    FinanceModel* model;
    FinanceController* controller;
};
#endif // MAINWINDOW_H
