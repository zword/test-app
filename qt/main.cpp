#include "mainwindow.h"
#include <QApplication>

#include <QLabel>
#include <QObject>
#include <iostream>

MainWindow *w;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#if 1
    w = new MainWindow;

    w->setWindowTitle("LED Emulator");
    w->setWindowFlags(Qt::WindowStaysOnTopHint);

    QObject::connect(&a, SIGNAL(lastWindowClosed()), w, SLOT(closing()));

    w->show();
#else

    QImage myImage;
    if ( !myImage.load("Red_Wine_Glass.jpg") )
        std::cout<<"sdsd\n";

    QLabel myLabel;
    myLabel.setPixmap(QPixmap::fromImage(myImage));

    myLabel.show();
#endif
    return a.exec();
}
