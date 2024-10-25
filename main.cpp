#include "mainwindow.h"
#include <QApplication>
#include "transaction.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Moneo");
    w.showMaximized();
    return a.exec();
}
