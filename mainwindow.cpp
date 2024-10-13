#include "mainwindow.h"
#include "transaction.h"

#include <QTableView>
#include <QVBoxLayout>
#include <QWidget>
#include <QHeaderView>
#include <QDir>
#include <Iostream>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), tableView(new QTableView(this)), model(new TransactionModel(this)), delegate(new TransactionDelegate(this))
{
    // UI configuration
    QWidget* central = new QWidget(this);
    setCentralWidget(central);
    central->setObjectName("central");
    QVBoxLayout* layout = new QVBoxLayout(central);
    layout->addWidget(tableView);

    // UI style
    QFile styleFile("../Moneo/style/style.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(styleFile.readAll());
        this->setStyleSheet(styleSheet);
    }


    // View configuration
    tableView->setModel(model);
    tableView->setItemDelegate(delegate);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);

    // Delete signal connexion
    connect(delegate, &TransactionDelegate::deleteClicked, this, &MainWindow::handleDelete);

    // Generate transactions from file
    QString filePath = QDir::current().filePath("../Moneo/data/Test.csv");
    QList<Transaction> transactions = transactionGenerator(filePath);

    // Add all transactions to model
    for (Transaction& t : transactions)
    {
        model->addTransaction(Transaction(t));
    }

    // Test to verify the modification
    QPushButton* btnTest = new QPushButton(central);
    btnTest->setText("Test");
    connect(btnTest, &QPushButton::clicked, [this]() {
        QVector<Transaction> currentTransactions = model->getTransactions();
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
        button->setText(isActive ? "Button" : "Active button");
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
    model->removeTransaction(row);
}
