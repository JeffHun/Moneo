#include "mainwindow.h"
#include <QApplication>
#include "transaction.h"
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Moneo");
    w.show();
    return a.exec();
}
