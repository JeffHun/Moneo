#include "budgetcategory.h"

BudgetCategory::BudgetCategory(QWidget* parent, const QString& imgPath, const QString& name, const QString& color, QPieSlice *slice, float value, QLabel* expenseExemple) : QWidget(parent){
    m_imgPath = imgPath;
    m_name = name;
    m_color = color;
    m_slice = slice;
    m_value = value;
    m_expenseExemple = expenseExemple;
    setMouseTracking(true);
    setup();
}

void BudgetCategory::setup()
{
    setFixedSize(120, 140);

    QVBoxLayout *budgetingCategoryLayout = new QVBoxLayout(this);

    QLabel *iconLabel = new QLabel(this);
    QPixmap categoryImg(m_imgPath);
    QPixmap coloredPixmap(categoryImg.size());
    coloredPixmap.fill(Qt::transparent);
    QPainter painter(&coloredPixmap);
    painter.drawPixmap(0, 0, categoryImg);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(coloredPixmap.rect(), m_color);
    painter.end();

    QSize size(30,30);
    categoryImg = categoryImg.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    iconLabel->setPixmap(coloredPixmap);
    iconLabel->setAlignment(Qt::AlignCenter);

    QLabel *categoryLabel = new QLabel(m_name, this);
    categoryLabel->setObjectName("categoryLabel");
    categoryLabel->setAlignment(Qt::AlignCenter);

    m_lineEdit = new QLineEdit(this);
    m_lineEdit->setObjectName("lineEdit");
    m_lineEdit->setText(QString::number(m_value));
    m_lineEdit->setAlignment(Qt::AlignCenter);

    m_slice->setValue(m_value);

    // Constrain inputs to float
    QDoubleValidator *doubleValidator = new QDoubleValidator(0.00, std::numeric_limits<double>::max(), 0, m_lineEdit);
    doubleValidator->setNotation(QDoubleValidator::StandardNotation);
    m_lineEdit->setValidator(doubleValidator);

    m_percentage = new QLabel("0%", this);
    m_percentage->setObjectName("percentage");
    m_percentage->setAlignment(Qt::AlignCenter);

    budgetingCategoryLayout->addWidget(iconLabel);
    budgetingCategoryLayout->addWidget(categoryLabel);
    budgetingCategoryLayout->addWidget(m_lineEdit);
    budgetingCategoryLayout->addWidget(m_percentage);

    //Update pie chart slice and percentage each time value is edit
    connect(m_lineEdit, &QLineEdit::editingFinished, [this]() {
        for(int i=0; i<m_budgetCategories.count(); i++)
        {
            m_budgetCategories[i]->updateSlice();
            m_budgetCategories[i]->updatePercentageLabel();
        }
        emit valueChanged(m_value);
    });
}

// Highlight pie chart slice when mouse hover
bool BudgetCategory::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::Enter:
        if(m_value>0)
            m_slice->setExploded(true);
        updateExpenseExemple();
        return false;

    case QEvent::Leave:
        m_slice->setExploded(false);
        m_expenseExemple->setText("");
        return false;
    default:
        return QWidget::event(event);
    }
}

void BudgetCategory::updateExpenseExemple()
{
    if(m_name=="Food\n")
        m_expenseExemple->setText("Supermarket, restaurant, bakery, ...");
    if(m_name=="Excluded\ntransaction")
        m_expenseExemple->setText("Internal transfer, ...");
    if(m_name=="Leisure &\nholidays")
        m_expenseExemple->setText("Bar, museum, sport");
    if(m_name=="Education &\nfamily")
        m_expenseExemple->setText("Childcare, academic support, training, ...");
    if(m_name=="Shopping &\nservices")
        m_expenseExemple->setText("Hairdresser, clothing, gift, ...");
    if(m_name=="Taxes &\nduties")
        m_expenseExemple->setText("Social contribution, income tax, property tax, ...");
    if(m_name=="Transport\n")
        m_expenseExemple->setText("Car insurance, toll, public transport, ...");
    if(m_name=="Saving\n")
        m_expenseExemple->setText("Financial investment, saving account, housing savings, ...");
    if(m_name=="Outflow of\nmoney")
        m_expenseExemple->setText("Transfer, cash withdrawal, ...");
    if(m_name=="Legal &\nadministrative")
        m_expenseExemple->setText("Lawyers, bailiffs' fees, alimony, ...");
    if(m_name=="Bank &\ninsurance")
        m_expenseExemple->setText("Credit, bank charges, securities, ...");
    if(m_name=="Housing\n")
        m_expenseExemple->setText("Rent, internet subscription, electricity bills, ...");
    if(m_name=="Health\n")
        m_expenseExemple->setText("Doctor, mutual insurance, pharmacy, ...");
}

void BudgetCategory::updateSlice()
{
    m_value = m_lineEdit->text().toFloat();
    m_slice->setValue(m_value);
}

void BudgetCategory::updatePercentageLabel()
{
    m_percentage->setText("~" + QString::number(round(m_slice->percentage()*1000)/10) + "%");
}

void BudgetCategory::setBudgetCategories(QList<BudgetCategory*>aBudgetingWidgets)
{
    m_budgetCategories = aBudgetingWidgets;
}

float BudgetCategory::getValue()
{
    return m_value;
}
