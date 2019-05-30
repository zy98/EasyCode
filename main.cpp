#include "MainWindow.h"
#include <QApplication>
#include <QFileInfo>
#include <ctime>
#include <QDebug>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    MainWindow w;

    if(argc > 1)
    {
        QFileInfo file(argv[1]);
        if(file.isFile())
            w.OpenFile(file.absoluteFilePath());
    }

    w.show();
    return a.exec();
}
