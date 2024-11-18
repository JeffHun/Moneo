#include "mainwindow.h"
#include "transaction.h"

#include "welcomePage.h"
#include "transactionsPage.h"
#include "budgetPage.h"
#include "analysispage.h"
#include "settingsPage.h"

#include <QTableView>
#include <QVBoxLayout>
#include <QWidget>
#include <QHeaderView>
#include <Iostream>
#include <QLabel>
#include <QObject>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), m_menuActiveButton(nullptr)
{
    moveToPrimaryScreen();

    uiSetUp();
    loadWelcomeContent();
}

void MainWindow::uiSetUp()
{
    // UI style
    QFile styleFile(":/resources/style/style.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(styleFile.readAll());
        this->setStyleSheet(styleSheet);
    } else {
        qCritical() << "Failed to open stylesheet:" << styleFile.errorString();
    }

    // Setup central widget and layout
    QWidget* central = new QWidget(this);
    setCentralWidget(central);
    central->setObjectName("central");
    QHBoxLayout* layout = new QHBoxLayout(central);
    layout->setContentsMargins(0, 0, 0, 0);

    // Setup menu
    QWidget* menu = new QWidget(central);
    menu->setObjectName("menu");
    layout->addWidget(menu);
    QVBoxLayout* menuLayout = new QVBoxLayout(menu);
    menuLayout->setContentsMargins(0,0,0,0);

    // Add logo
    QLabel *logo = new QLabel(menu);
    logo->setObjectName("logo");
    QPixmap logoPix(":/resources/style/img/logo.png");
    QSize size(70,70);
    logoPix = logoPix.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    logo->setPixmap(logoPix);
    logo->setAlignment(Qt::AlignCenter);
    menuLayout->addWidget(logo);
    menuLayout->addStretch();

    // Setup buttons container
    QWidget* btnContainer = new QWidget(menu);
    menuLayout->addWidget(btnContainer);
    QVBoxLayout* btnLayout = new QVBoxLayout(btnContainer);
    btnLayout->setContentsMargins(0, 0, 0, 0);
    btnLayout->setAlignment(Qt::AlignRight);

    // Create menu buttons
    createMenuButton(menu, btnLayout, "Transactions", ":/resources/style/img/transaction.png", &MainWindow::loadTransactionsContent);
    btnLayout->addStretch();
    createMenuButton(menu, btnLayout, "Budget", ":/resources/style/img/budget.png", &MainWindow::loadBudgetContent);
    btnLayout->addStretch();
    createMenuButton(menu, btnLayout, "Analysis", ":/resources/style/img/analysis.png", &MainWindow::loadAnalysisContent);
    btnLayout->addStretch();
    createMenuButton(menu, btnLayout, "Settings", ":/resources/style/img/setting.png", &MainWindow::loadSettingsContent);
    menuLayout->addStretch();

    // Add version label
    QLabel* versionLbl = new QLabel("V 0.1", menu);
    versionLbl->setObjectName("smallText");
    menuLayout->addWidget(versionLbl);

    // Page creation and stacking
    QWidget* content = new QWidget(central);
    layout->addWidget(content);

    m_stack = new QStackedWidget(content);
    m_stack->addWidget(new WelcomePage(m_stack));
    m_stack->addWidget(new TransactionsPage(m_stack));
    m_stack->addWidget(new BudgetPage(m_stack));
    m_stack->addWidget(new AnalysisPage(m_stack));
    m_stack->addWidget(new SettingsPage(m_stack));

    QVBoxLayout* contentLayout = new QVBoxLayout(content);
    contentLayout->addWidget(m_stack);

    // Load welcome page by default
    m_stack->setCurrentIndex(0);
}

void MainWindow::createMenuButton(QWidget* parent,QVBoxLayout* layout, const QString& text, const QString& path,void (MainWindow::*slot)())
{
    // Btn components and properties creation
    auto* button = new QPushButton(text, parent);
    QIcon buttonIcn(path);
    button->setIcon(buttonIcn);
    button->setIconSize(QSize(15,15));
    button->setObjectName("menuBtn");
    button->setFixedWidth(125);
    layout->addWidget(button);
    connect(button, &QPushButton::clicked, this, slot);

    // Active button management
    connect(button, &QPushButton::clicked, this, [this, button]() {
        if (m_menuActiveButton) {
            setButtonState(m_menuActiveButton, false);
        }

        m_menuActiveButton = button;
        setButtonState(m_menuActiveButton, true);
    });
}

void MainWindow::loadWelcomeContent()
{
    m_stack->setCurrentIndex(0);
}

void MainWindow::loadTransactionsContent()
{
    m_stack->setCurrentIndex(1);
}

void MainWindow::loadBudgetContent()
{
    m_stack->setCurrentIndex(2);
}

void MainWindow::loadAnalysisContent()
{
    m_stack->setCurrentIndex(3);
    BudgetPage *budgetPage = qobject_cast<BudgetPage*>(m_stack->widget(2));
    if (budgetPage) {
        budgetPage->setBudgetCategoryValues();
    }

    AnalysisPage *analysisPage = qobject_cast<AnalysisPage*>(m_stack->widget(3));
    TransactionsPage *transactionsPage = qobject_cast<TransactionsPage*>(m_stack->widget(1));
    analysisPage->setTransactions(transactionsPage->getTransactions());
    analysisPage->setBudgets(budgetPage->getSettings());
    analysisPage->headerGeneration();
}

void MainWindow::loadSettingsContent()
{
    m_stack->setCurrentIndex(4);
}

void MainWindow::setButtonState(QPushButton* button, bool state) {
    button->setProperty("active", state);
    button->style()->unpolish(button);
    button->style()->polish(button);
    button->update();
}

void MainWindow::moveToPrimaryScreen() {
    QScreen *primaryScreen = QGuiApplication::primaryScreen();
    if (primaryScreen) {
        QRect screenGeometry = primaryScreen->geometry();
        move(screenGeometry.topLeft());
    }
}

MainWindow::~MainWindow()
{
    delete m_menuActiveButton;
}
