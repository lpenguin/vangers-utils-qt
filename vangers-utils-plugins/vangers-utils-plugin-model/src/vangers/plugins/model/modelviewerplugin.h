#ifndef MODELVIEWERPLUGIN_H
#define MODELVIEWERPLUGIN_H

#include <vangers/core/plugin/resourceviewer.h>


namespace vangers::plugins::model {
	class ModelViewerPlugin : public vangers::core::plugin::ResourceViewerPlugin
	{
	public:
		ModelViewerPlugin() = default;
		virtual ~ModelViewerPlugin() = default;

		QList<vangers::core::plugin::ResourceType> supportedImportTypes() const;
		QList<vangers::core::plugin::ResourceType> supportedExportTypes() const;
		vangers::core::plugin::ResourceViewer *makeResourceViewer(QWidget *parent);

		QString name() const {
			return "Model";
		};

		static const vangers::core::plugin::ResourceType M3D;
		static const vangers::core::plugin::ResourceType A3D;
		static const vangers::core::plugin::ResourceType Json;
		static const vangers::core::plugin::ResourceType Obj;
	};

}


#endif // MODELVIEWERPLUGIN_H
