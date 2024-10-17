#ifndef WELCOMEPAGE_H
#define WELCOMEPAGE_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class WelcomePage : public QWidget{
    Q_OBJECT

public:
    explicit WelcomePage(QWidget* parent = nullptr);

private:
    QWidget* createImageContainer(const QString& imagePath, const QString& labelText, QSize size, QWidget* parent);
};

#endif // WELCOMEPAGE_H
