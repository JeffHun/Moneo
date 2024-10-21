#include "buttonutility.h"

void ButtonUtility::setActiveProperty(QPushButton* button, bool active)
{
    button->setProperty("active", active);
    button->style()->unpolish(button);
    button->style()->polish(button);
    button->update();
}

void ButtonUtility::connectButton(QPushButton* button)
{
    QObject::connect(button, &QPushButton::pressed, [button]() {
        setActiveProperty(button, true);
    });

    QObject::connect(button, &QPushButton::released, [button]() {
        setActiveProperty(button, false);
    });
}

void ButtonUtility::connectToggleActiveProperty(QPushButton* button)
{
    QObject::connect(button, &QPushButton::clicked, [button]() {
        bool isActive = button->property("active").toBool();
        button->setProperty("active", !isActive);
        button->style()->unpolish(button);
        button->style()->polish(button);
        button->update();
    });
}
