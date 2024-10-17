#include "analysispage.h"

AnalysisPage::AnalysisPage(QWidget* parent) : QWidget(parent) {
    QLabel* title = new QLabel("Analysis", this);
    title->setObjectName("title");
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(title);
}
