#include "mainwindow.h"

#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
	QApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
    QApplication a(argc, argv);
	a.setStyle(QStyleFactory::create("fusion"));
    MainWindow w;
    w.show();
    return a.exec();
}
