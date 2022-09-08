#include "voxwritersettingsdialog.h"
#include "ui_voxwritersettingsdialog.h"

VoxWriterSettingsDialog::VoxWriterSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VoxWriterSettingsDialog)
{
	ui->setupUi(this);
}

VoxWriterSettingsDialog::~VoxWriterSettingsDialog()
{
	delete ui;
}

vangers::plugins::vmap::VmapVoxWriterSettings VoxWriterSettingsDialog::getSettings()
{
	QStringList indexColorMappingStrings = ui->indexColorMappingText->text().split(QRegExp("\\s+"));
	std::map<int32_t, int32_t> indexColorMap {};
	for(const QString& indexColorMappingRecord: indexColorMappingStrings){
		QStringList tokens = indexColorMappingRecord.split('=');
		if(tokens.length() != 2){
			continue;
		}

		bool ok;
		int32_t from = tokens[0].toInt(&ok);
		if(!ok) continue;
		int32_t to = tokens[1].toInt(&ok);
		if(!ok) continue;
		indexColorMap[from - 1] = to - 1;
	}
	QString hsStr = ui->heightScaleSpinBox->text();
	QString indexColorStr = ui->indexColorShiftText->text();
	return {
		.hollow = ui->hollowCb->isChecked(),
		.hollowSize = ui->hollowSizeText->text().toInt(),
		.indexColorShift = ui->indexColorShiftText->text().toInt(),
		.compressChunks = ui->compressChunksCb->isChecked(),
		.flipX = ui->flipXCb->isChecked(),
		.voxSizeX = ui->voxSizeXText->text().toInt(),
		.voxSizeY = ui->voxSizeXText->text().toInt(),
		.chunkSizeX = ui->chunkSizeXText->text().toInt(),
		.chunkSizeY = ui->chunkSizeYText->text().toInt(),
		.heightScale = ui->heightScaleSpinBox->text().toFloat(),
		.indexColorMapping = indexColorMap,
	};
}
