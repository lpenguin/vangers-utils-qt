#include "cliconverter.h"

#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDebug>

#include "heightmapfrommesh.h"

using namespace vangers::model2vmap;

int vangers::model2vmap::cli_convert(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);

	QCoreApplication::setApplicationName("model2vmap");
	QCoreApplication::setApplicationVersion("0.1");

	QCommandLineParser parser;
	parser.setApplicationDescription("OBJ to vmap converter");
	parser.addHelpOption();

	parser.addVersionOption();

	parser.addPositionalArgument("input", "input .obj file");
	parser.addPositionalArgument("output-dir", "output directory");
	parser.addOptions({
						  {"width", "World width", "width", "2048"},
						  {"height", "World height", "height", "2048"},
						  {"htr", "height threshold", "htr", "10"},
						  {"determine-mid", "Try to determine middle level"},
					  });

	QStringList args;
	for(int i = 0; i < argc; i++){
		args << argv[i];
	}
	if(!parser.parse(args)){
		qWarning() << parser.errorText();
		return 1;
	}

	parser.process(app);

	QStringList posArgs = parser.positionalArguments();
	if(posArgs.size() != 2){
		qWarning().noquote() << parser.helpText();
		return 1;
	}

	QString inputPath = posArgs[0];
	QString outputDirPath = posArgs[1];

	QString widthStr = parser.value("width");
	QString heightStr = parser.value("height");
	QString htrStr = parser.value("htr");

	int32_t width = widthStr.toInt();
	int32_t height = heightStr.toInt();
	int32_t htr = htrStr.toInt();

	RasterSettings settings = {
		.worldSize = {width, height, 256},
		.hThr = htr,
		.doubleLevelStrategy = parser.isSet("determine-mid") ?
				DoubleLevelStrategy::DetermineMid :
				DoubleLevelStrategy::TakeMax,
	};

	auto res = heightmapFromMesh(inputPath, outputDirPath, settings);
	if(!res) {
		qWarning() << "Conversion error";
	}

	return 0;
}
