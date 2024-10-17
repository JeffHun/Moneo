#include "settingspage.h"

SettingsPage::SettingsPage(QWidget* parent) : QWidget(parent) {
    QLabel* title = new QLabel("Settings", this);
    title->setObjectName("title");
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(title);
}
