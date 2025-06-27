#include "mainwindow.h"
#include <QIcon>
#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    QCoreApplication::setApplicationName("DeskSort");

    a.setWindowIcon(QIcon(":/icon.ico"));

    MainWindow w;
    w.show();
    return a.exec();
}
