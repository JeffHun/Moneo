#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "transactionmodel.h"
#include "transactiondelegate.h"
#include <QVBoxLayout>
#include <QScreen>
#include <QGuiApplication>

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
    QTableView* m_tableView;
    TransactionModel* m_model;
    TransactionDelegate* m_delegate;
    QList<QPushButton*> m_menuButtons;
    QPushButton* m_menuActiveButton;
    QVBoxLayout* m_contentLayout;
    QWidget* m_content;

    void loadContent(QPushButton*);
    void setButtonState(QPushButton*, bool);

    void loadWelcomeContent();
    void loadTransactionsContent();
    void loadBudgetContent();
    void loadAnalysisContent();
    void loadSettingsContent();

    void createMenuButton(QWidget*,QVBoxLayout*, const QString&, const QString& path, void (MainWindow::*slot)());
    void moveToPrimaryScreen();
    void uiSetUp();
    void deleteContentChildren();
};

void setupInteractiveButton(QPushButton*);
void setupToggleButton(QPushButton*);

#endif // MAINWINDOW_H
