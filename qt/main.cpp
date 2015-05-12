#include "mainwindow.h"
#include <QApplication>

#include <QLabel>
#include <QObject>
#include <iostream>

MainWindow *w;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    w = new MainWindow;
    w->setWindowTitle("LED Emulator ");
    w->setWindowFlags(Qt::WindowStaysOnTopHint);

    QObject::connect(&a, SIGNAL(lastWindowClosed()), w, SLOT(closing()));

    w->show();

    return a.exec();
}
