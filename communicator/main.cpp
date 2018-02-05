#include "mainwindow.h"
#include <QApplication>
#include <QPushButton>
#include <conversation.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.Show();
    w.show();

    return a.exec();
}
