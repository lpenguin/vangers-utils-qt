#ifndef OBJIMPORTSETTINGSWIDGET_H
#define OBJIMPORTSETTINGSWIDGET_H

#include <QTreeWidgetItem>
#include <QWidget>

#include <plugins/model/obj/obj.h>
#include <plugins/model/objimport/objimportsettings.h>

namespace Ui {
	class ObjImportSettingsWidget;
}

namespace vangers::model::objimport {
	using namespace vangers::model::obj;

	class ObjImportSettingsWidget : public QWidget
	{
		Q_OBJECT

	public:
		explicit ObjImportSettingsWidget(QWidget *parent = nullptr);
		~ObjImportSettingsWidget();

		void setImportSettings(const ObjImportSettings& importSettings);
		void makeImportSettings(ObjImportSettings& importSettings);
	signals:
		void importSettingsChanged();

	private slots:
		void handleReimportClicked();

	private:
		void setupTables(const ObjImportSettings& importSettings);
		void setupObjetcs(const ObjImportSettings& importSettings);
		void setupMaterials(const ObjImportSettings& importSettings);
		void setupParameters(const ObjImportSettings& importSettings);

		void makeObjects(ObjImportSettings& importSettings);
		void makeMaterials(ObjImportSettings& importSettings);
		void makeParameters(ObjImportSettings& importSettings);
		Ui::ObjImportSettingsWidget *ui;
	};

}

#endif // OBJIMPORTSETTINGSWIDGET_H
