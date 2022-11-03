#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>

#include "heightmapfrommesh.h"

using namespace vangers::model2vmap;
using namespace vangers::core::error;

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	settings("lpenguin", "model2vmap")
{
	ui->setupUi(this);
	this->setWindowTitle("model2vmap");

	ui->modelPathText->setText(settings.value("lastModelFile").toString());
	QObject::connect(ui->modelBrowseButton, &QPushButton::clicked, [&](){
		QString lastFolder = settings.value("lastModelFolder").toString();
		QString filename = QFileDialog::getOpenFileName(
			this,
			tr("Open model file"),
			lastFolder,
			tr("Wavefront OBJ (*.obj)")
		);

		if(filename.isEmpty() ||
		   filename.isNull()){
			return;
		}
		ui->modelPathText->setText(filename);
		settings.setValue("lastModelFile", filename);
		settings.setValue("lastModelFolder", QFileInfo(filename).absoluteDir().path());
	});

	ui->outputText->setText(settings.value("lastOutputFolder").toString());
	QObject::connect(ui->outputBrowserButton, &QPushButton::clicked, [&](){
		QString lastFolder = settings.value("lastOutputFolder").toString();
		QString outputFolder = QFileDialog::getExistingDirectory(
			this,
			tr("Select output folder"),
			lastFolder
		);

		if(outputFolder.isEmpty() ||
		   outputFolder.isNull()){
			return;
		}
		ui->outputText->setText(outputFolder);
		settings.setValue("lastOutputFolder", outputFolder);
	});

	if(settings.value("doubleLevelStrategy").toString() == "determine-mid"){
		ui->algorithmComboBox->setCurrentIndex(0);
	} else {
		ui->algorithmComboBox->setCurrentIndex(1);
	}

	QObject::connect(ui->convertButton, &QPushButton::clicked, [&](){
		DoubleLevelStrategy doubleLevelStrategy;
		int rIndex = ui->algorithmComboBox->currentIndex();
		if(rIndex == 0){
			settings.setValue("doubleLevelStrategy", "determine-mid");
			doubleLevelStrategy = DoubleLevelStrategy::DetermineMid;
		} else {
			settings.setValue("doubleLevelStrategy", "max");
			doubleLevelStrategy = DoubleLevelStrategy::TakeMax;
		}

		QString powerXText = ui->mapPowerXCombo->currentText();
		QString powerYText = ui->mapPowerYCombo->currentText();

		int powerX = powerXText.split(" ")[0].toInt();
		int powerY = powerYText.split(" ")[0].toInt();
		int32_t geoNetPower = ui->geoNetPowerCombo->currentText().toInt();
		int32_t sectionSizePower = ui->sectionSizePowerCombo->currentText().toInt();
		int32_t minSquarePower = ui->minSquarePowerCombo->currentText().toInt();
		bool writeVpr = ui->writeVprCheck->isChecked();

		RasterSettings rasterSettings {
			.worldSize = vangers::core::vector::Vector3I32{
			    .x = (int)std::pow(2, powerX),
			    .y = (int)std::pow(2, powerY),
				.z = 256,
			},
			.writeVpr = writeVpr,
			.globalParams = {
			    .mapPowerX = powerX,
			    .mapPowerY = powerY,
			    .geoNetPower = geoNetPower,
			    .sectionSizePower = sectionSizePower,
			    .minSquarePower = minSquarePower,
			},
			.hThr = -1,
			.doubleLevelStrategy = doubleLevelStrategy,
			.minUpDownLevelDist = 30,
			.minDoubleLevelThikness = 30,
			        .shadowAspect = 2.0f/3.0f
		};

		settings.setValue("lastModelFile", ui->modelPathText->text());
		settings.setValue("lastOutputFolder", ui->outputText->text());
		ui->logText->setPlainText(QString("Processing, it might take several minutes..."));
		qApp->processEvents();
		Result result = heightmapFromMesh(ui->modelPathText->text(),
						  ui->outputText->text(),
						  rasterSettings
						  );


		QString resultStr;
		if(result){
			resultStr = "Converted";
		} else {
			resultStr = "Conversion error: " + result.error().message;
		}
		ui->logText->setPlainText(ui->logText->toPlainText() + "\n" + resultStr);
	});
}

MainWindow::~MainWindow()
{
	delete ui;
}
