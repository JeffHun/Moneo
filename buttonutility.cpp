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

void ButtonUtility::connectUniqueToggleActiveProperty(QList<QPushButton*> btns, int activeIndex) {
    for (int j = 0; j < btns.count(); ++j) {
        bool isActive = (j == activeIndex);
        btns[j]->setProperty("active", isActive);
        btns[j]->style()->unpolish(btns[j]);
        btns[j]->style()->polish(btns[j]);
        btns[j]->update();
    }
}
