#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QWidget>
#include <QSharedPointer>
#include <QSettings>
#include "image.h"
#include <vangers/core//palette/palette.h>
#include <vangers/core//plugin/resourceviewer.h>

namespace Ui {
class ImageViewer;
}

namespace vangers::plugins::image {
	namespace  {
		using namespace vangers::core::plugin;
		using namespace vangers::core::palette;
	}

	class ImageViewerPlugin: public ResourceViewerPlugin {
	public:
		static const ResourceType PngType;
		static const ResourceType BmpType;
		static const ResourceType XbmType;
		static const ResourceType vBmpType;
		static const ResourceType TiffType;
		static const ResourceType SvsType;
		static const ResourceType MssType;

		explicit ImageViewerPlugin(QObject *parent = nullptr)
			: ResourceViewerPlugin(parent) {}

		QList<ResourceType> supportedImportTypes() const override;
		QList<ResourceType> supportedExportTypes() const override;
		ResourceViewer* makeResourceViewer(QWidget* parent = nullptr) override;

		QString name() const override {
			return "Image";
		}
	};

	class ImageViewer : public ResourceViewer
	{
		Q_OBJECT

	public:
		explicit ImageViewer(ImageViewerPlugin* plugin, QWidget *parent = nullptr);
		~ImageViewer();


		bool importResource(const QString& filename, const ResourceType& resourceType) override;
		void exportResource(const QString& filename, const ResourceType& resourceType) override;
		QString currentFile() const override;
	public slots:
		void handlePaletteChanged(QString paletteName);
	private slots:
		void palette_useTransparentColor(bool use, int colorIndex);
	private:
		Ui::ImageViewer *ui;
		QSharedPointer<Image> _image;
		Palette _palette;
		QList<QWidget*> _metaFields;
		QMap<QString, QSharedPointer<AbstractImageAccess>> _accesses;

		bool tryRead(Image& image, const QString& fileName, const ResourceType& resourceType);
		void setPalette(const Palette palette);
		void updateImage();
		bool useTransparentColor;
		int transparentColor;
		QString _filename;
		QSettings _settings;
	};
	}


#endif // IMAGEVIEWER_H
