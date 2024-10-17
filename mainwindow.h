#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QScreen>
#include <QGuiApplication>
#include <QStackedWidget>

class QTableView;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QPushButton* m_menuActiveButton;
    QStackedWidget* m_stack;

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
