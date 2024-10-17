#include "welcomepage.h"

#include <QPushButton>

WelcomePage::WelcomePage(QWidget* parent) : QWidget(parent) {
    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* title = new QLabel("Welcome to MONEO", this);
    title->setObjectName("bigTitle");
    title->setAlignment(Qt::AlignCenter);

    QLabel* description = new QLabel("Facilitate managing your finances by automating the visualization of your expenses. Understand where your money is being spent to track your budget.", this);
    description->setObjectName("text");
    description->setWordWrap(true);
    description->setAlignment(Qt::AlignCenter);

    QWidget* imgsContainer = new QWidget(this);
    QHBoxLayout* imgsContainerLyt = new QHBoxLayout(imgsContainer);

    QPixmap pixArrow(":/resources/style/img/arrow.png");
    QLabel* arrow = new QLabel(imgsContainer);
    QSize smallSize(25,25);
    QTransform transform;
    transform.rotate(90);
    QPixmap rotatedArrow = pixArrow.transformed(QTransform().rotate(90), Qt::SmoothTransformation);
    rotatedArrow = rotatedArrow.scaled(smallSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    arrow->setPixmap(rotatedArrow);
    arrow->setAlignment(Qt::AlignCenter);

    QLabel* arrow2 = new QLabel(imgsContainer);
    arrow2->setPixmap(rotatedArrow);
    arrow2->setAlignment(Qt::AlignCenter);

    QSize size(100,100);
    imgsContainerLyt->addStretch();
    imgsContainerLyt->addWidget(createImageContainer(":/resources/style/img/transactionIllustration.png", "Upload your transactions using a CSV file.", size, imgsContainer));
    imgsContainerLyt->addWidget(arrow);
    imgsContainerLyt->addWidget(createImageContainer(":/resources/style/img/budgetIllustration.png", "Set your budget for each category.", size, imgsContainer));
    imgsContainerLyt->addWidget(arrow2);
    imgsContainerLyt->addWidget(createImageContainer(":/resources/style/img/analyticsIllustration.png", "View the analytics and identify optional expenses.", size, imgsContainer));
    imgsContainerLyt->addStretch();

    QPushButton* btn = new QPushButton("Upload file",this);
    btn->setFixedWidth(150);

    layout->addStretch();
    layout->addStretch();
    layout->addWidget(title);
    layout->addWidget(description);
    layout->addStretch();
    layout->addWidget(imgsContainer);
    layout->addStretch();
    layout->addWidget(btn, 0, Qt::AlignCenter);
    layout->addStretch();
    layout->addStretch();
}

QWidget* WelcomePage::createImageContainer(const QString& imagePath, const QString& labelText, QSize size, QWidget* parent) {
    QWidget* container = new QWidget(parent);
    QVBoxLayout* layout = new QVBoxLayout(container);

    QPixmap pix(imagePath);
    QLabel* imgLabel = new QLabel(container);
    pix = pix.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    imgLabel->setPixmap(pix);
    imgLabel->setAlignment(Qt::AlignCenter);

    QLabel* textLabel = new QLabel(labelText, container);
    textLabel->setObjectName("smallText");
    textLabel->setAlignment(Qt::AlignCenter);
    textLabel->setWordWrap(true);

    layout->addWidget(imgLabel);
    layout->addWidget(textLabel);

    return container;
}
