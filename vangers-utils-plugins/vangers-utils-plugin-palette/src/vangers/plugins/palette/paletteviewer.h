#ifndef PALETTEVIEWER_H
#define PALETTEVIEWER_H

#include <QMap>

#include <vangers/core//plugin/resourceviewer.h>
#include <vangers/core//plugin/resourceaccess.h>
#include <vangers/core//palette/palette.h>

#include "microsoftpalaccess.h"
#include "vangerspaletteaccess.h"
#include "pngpaletteaccess.h"

using namespace vangers;
namespace Ui {
    class PaletteViewer;
}

namespace vangers::plugins::palette {
	using namespace vangers::core::plugin;
	using namespace vangers::core::palette;

	class PaletteViewer : public ResourceViewer
	{
		Q_OBJECT

	public:
		~PaletteViewer();
	public:
		explicit PaletteViewer(ResourceViewerPlugin* plugin, QWidget *parent = nullptr);

		bool importResource(const QString& filePath, const ResourceType& resourceType) override;
		void exportResource(const QString& filePath, const ResourceType& resourceType) override;
		QString currentFile() const override;

	public slots:
		void importButton_click();
	private:
		QString _currentFile;
		static const QMap<QString, QSharedPointer<AbstractResourceAccess<Palette>>> accesses;
	private:
		Ui::PaletteViewer *ui;
	};

	class PaletteViewerPlugin: public ResourceViewerPlugin {


		// ResourceViewerPlugin interface
	public:
		static const ResourceType PalType;
		static const ResourceType PngType;
		static const ResourceType BmpType;
		static const ResourceType MicrosoftPalType;

		explicit PaletteViewerPlugin(QObject* parent = nullptr)
			: ResourceViewerPlugin(parent) {}

		QList<ResourceType> supportedImportTypes() const override;
		QList<ResourceType> supportedExportTypes() const override;
		ResourceViewer* makeResourceViewer(QWidget* parent) override;
		QString name() const override {
			return "Palette";
		}
	};

	}

#endif // PALETTEVIEWER_H
