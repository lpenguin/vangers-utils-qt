#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDebug>
#include <iostream>

#include "cliconverter.h"
#include "guiconverter.h"

using namespace vangers::model2vmap;

int main(int argc, char *argv[]){
	if(argc > 1){
		return cli_convert(argc, argv);
	} else {
		return gui_convert(argc, argv);
	}
	return 0;
}
