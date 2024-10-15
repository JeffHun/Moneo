#include "mainwindow.h"
#include "transaction.h"

#include <QTableView>
#include <QVBoxLayout>
#include <QWidget>
#include <QHeaderView>
#include <QDir>
#include <Iostream>
#include <QLabel>

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

    m_content = new QWidget(central);
    layout->addWidget(m_content);
    m_contentLayout = new QVBoxLayout(m_content);
}

void MainWindow::createMenuButton(QWidget* parent,QVBoxLayout* layout, const QString& text, const QString& path,void (MainWindow::*slot)())
{
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

void MainWindow::deleteContentChildren() {
    for (QWidget *child : m_content->findChildren<QWidget*>()) {
        qDebug() << "Deleting:" << child->objectName();
        delete child;
    }
}

void MainWindow::loadWelcomeContent()
{
    deleteContentChildren();
    QLabel* test = new QLabel("Welcome", m_content);
    test->setObjectName("bigTitle");
    m_contentLayout->addWidget(test);
}

void MainWindow::loadTransactionsContent()
{
    deleteContentChildren();
    QLabel* test = new QLabel("Transaction", m_content);
    test->setObjectName("bigTitle");
    m_contentLayout->addWidget(test);
}

void MainWindow::loadBudgetContent()
{
    deleteContentChildren();
    QLabel* test = new QLabel("Budget", m_content);
    test->setObjectName("bigTitle");
    m_contentLayout->addWidget(test);
}

void MainWindow::loadAnalysisContent()
{
    deleteContentChildren();
    QLabel* test = new QLabel("Analysis", m_content);
    test->setObjectName("bigTitle");
    m_contentLayout->addWidget(test);
}

void MainWindow::loadSettingsContent()
{
    deleteContentChildren();
    QLabel* test = new QLabel("Settings", m_content);
    test->setObjectName("bigTitle");
    m_contentLayout->addWidget(test);
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

/*
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), m_tableView(new QTableView(this)), m_model(new TransactionModel(this)), m_delegate(new TransactionDelegate(this))
{
    // UI configuration
    QWidget* central = new QWidget(this);
    setCentralWidget(central);
    central->setObjectName("central");
    QVBoxLayout* layout = new QVBoxLayout(central);
    layout->addWidget(m_tableView);

    // UI style
    QFile styleFile("../Moneo/style/style.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(styleFile.readAll());
        this->setStyleSheet(styleSheet);
    }


    // View configuration
    m_tableView->setModel(m_model);
    m_tableView->setItemDelegate(m_delegate);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);

    // Delete signal connexion
    connect(m_delegate, &TransactionDelegate::deleteClicked, this, &MainWindow::handleDelete);

    // Generate transactions from file
    QString filePath = QDir::current().filePath("../Moneo/data/Test.csv");
    QList<Transaction> transactions = transactionGenerator(filePath);

    // Add all transactions to model
    for (Transaction& t : transactions)
    {
        m_model->addTransaction(Transaction(t));
    }

    // Test to verify the modification
    QPushButton* btnTest = new QPushButton(central);
    btnTest->setText("Test");
    connect(btnTest, &QPushButton::clicked, [this]() {
        QVector<Transaction> currentTransactions = m_model->getTransactions();
        for (const Transaction& t : currentTransactions) {
            qDebug() << "Date:" << t.getDate().day() << "/" << t.getDate().month() << "/" << t.getDate().year() << " | "
                     << "Description:" << t.getDescription() << " | "
                     << "Amount:" << t.getAmount() << " | "
                     << "Catégory:" << t.getCategory() << "\n";
        }
    });

    // Typography test
    QLabel* bigTitle = new QLabel("Big title",central);
    bigTitle->setObjectName("bigTitle");
    QLabel* title = new QLabel("Title",central);
    title->setObjectName("title");
    QLabel* text = new QLabel("Text",central);
    text->setObjectName("text");
    QLabel* smallText = new QLabel("Small text",central);
    smallText->setObjectName("smallText");
    layout->addWidget(bigTitle);
    layout->addWidget(title);
    layout->addWidget(text);
    layout->addWidget(smallText);

    // Button and interaction test
    QPushButton* inactiveBtn = new QPushButton("Inactive Button", central);
    inactiveBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    inactiveBtn->setObjectName("inactiveBtn");
    layout->addWidget(inactiveBtn);
    QPushButton* pushBtn = new QPushButton("Button", central);
    pushBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    layout->addWidget(pushBtn);
    QPushButton* pushBtn2 = new QPushButton("Button", central);
    pushBtn2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    layout->addWidget(pushBtn2);
    setupInteractiveButton(pushBtn);
    setupToggleButton(pushBtn2);

    // Iconography test
    QLabel *categoryIcon = new QLabel(central);
    QPixmap categoryImg(":/resources/style/img/food.png");
    QSize size(20,20);
    categoryImg = categoryImg.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    categoryIcon->setPixmap(categoryImg);
    categoryIcon->setAlignment(Qt::AlignCenter);
    layout->addWidget(categoryIcon);

    // #add font to qrs

}
*/

void setupInteractiveButton(QPushButton* button) {
    MainWindow::connect(button, &QPushButton::pressed, [button]() {
        button->setProperty("active", true);
        button->style()->unpolish(button);
        button->style()->polish(button);
        button->update();
    });

    MainWindow::connect(button, &QPushButton::released, [button]() {
        button->setProperty("active", false);
        button->style()->unpolish(button);
        button->style()->polish(button);
        button->update();
    });
}

void setupToggleButton(QPushButton* button) {
    MainWindow::connect(button, &QPushButton::clicked, [button]() {
        bool isActive = button->property("active").toBool();
        button->setProperty("active", !isActive);
        button->style()->unpolish(button);
        button->style()->polish(button);
        button->update();
    });
}

MainWindow::~MainWindow()
{
}

void MainWindow::handleDelete(int row)
{
    m_model->removeTransaction(row);
}
