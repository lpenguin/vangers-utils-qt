#include "guiconverter.h"

#include <QApplication>
#include <QStyleFactory>

#include "mainwindow.h"

int vangers::model2vmap::gui_convert(int argc, char *argv[])
{
	QApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
	QApplication a(argc, argv);
	a.setStyle(QStyleFactory::create("fusion"));
	MainWindow w;
	w.show();
	return a.exec();
}
