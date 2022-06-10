#include "mainwindow.h"

#include <QApplication>
#include <QStyleFactory>
#include "heightmapfrommesh.h"

int main(int argc, char *argv[])
{
//	QString path ="/home/nikita/Sources/notebooks/vangers/data/proizvol_dec8.obj";
//	vangers::heightmapFromMesh(path);
//	return 0;

	QApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
    QApplication a(argc, argv);
	a.setStyle(QStyleFactory::create("fusion"));
    MainWindow w;
    w.show();
    return a.exec();
}
