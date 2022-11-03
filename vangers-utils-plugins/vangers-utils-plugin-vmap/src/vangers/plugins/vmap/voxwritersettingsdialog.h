#ifndef VOXWRITERSETTINGSDIALOG_H
#define VOXWRITERSETTINGSDIALOG_H

#include <QDialog>
#include <vangers/plugins/vmap/vmapvoxwriter.h>

namespace Ui {
	class VoxWriterSettingsDialog;
}

class VoxWriterSettingsDialog : public QDialog
{
		Q_OBJECT

	public:
		explicit VoxWriterSettingsDialog(QWidget *parent = nullptr);
		~VoxWriterSettingsDialog();
		vangers::plugins::vmap::VmapVoxWriterSettings getSettings();

	private:
		Ui::VoxWriterSettingsDialog *ui;
};

#endif // VOXWRITERSETTINGSDIALOG_H
