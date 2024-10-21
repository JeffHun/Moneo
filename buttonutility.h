#ifndef BUTTONUTILITY_H
#define BUTTONUTILITY_H

#include <QPushButton>
#include <QStyle>

class ButtonUtility
{
public:
    static void setActiveProperty(QPushButton* button, bool active);
    static void connectButton(QPushButton* button);
    static void connectToggleActiveProperty(QPushButton* button);
};

#endif // BUTTONUTILITY_H
