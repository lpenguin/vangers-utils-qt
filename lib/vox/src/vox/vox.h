#ifndef VOX_H
#define VOX_H
#include <vector>
#include <stdint.h>
#include <QString>
#include <QMap>

namespace vox {
	typedef QList<QPair<QString, QString>> Dict;

	struct Voxel{
		int8_t data[4];
	};

	struct ModelVoxels {
		QVector<Voxel> voxels;
	};

	struct Model {
		int32_t sizeX;
		int32_t sizeY;
		int32_t sizeZ;
		ModelVoxels voxelsCollection;
	};

	struct Palette {
		int32_t colors[256];
	};

	struct Pack {
		uint32_t numModels;
	};

	struct Node {
		int32_t nodeId;
		Dict attributes;
	};

	struct TransformNode : public Node {
		int32_t childNodeId;
		int32_t reservedNodeId;
		int32_t layerId;
		QList<Dict> frameAttributes;
	};

	struct GroupNode : public Node {
		QList<int32_t> childNodeIds;
	};

	struct ShapeNodeModel {
		int32_t modelId;
		Dict modelAttributes;
	};

	struct ShapeNode : public Node {
		QList<ShapeNodeModel> models;
	};

	struct Material {
		int32_t id;
		Dict attributes;
	};

	struct Layer {
		int32_t id;
		Dict attributes;
		int32_t reservedId;
	};

	struct RenderObjects {
		Dict attributes;
	};

	struct RenderCamera {
		int32_t id;
		Dict attributes;
	};

	struct PaletteNote {
		QList<QString> colorNames;
	};

	struct IndexMap {
		int32_t paletteIndices[256];
	};

	struct Vox {
		Pack pack;
		QList<Model> models;
		QList<TransformNode> transformNodes;
		QList<GroupNode> groupNodes;
		QList<ShapeNode> shapeNodes;
		QList<Material> materials;
		QList<Layer> layers;
		QList<RenderObjects> renderObjects;
		QList<RenderCamera> renderCameras;
		QList<PaletteNote> paletteNotes;
		QList<IndexMap> indexMaps;

		QList<Palette> paletteOptional;

	};
}
#endif // VOX_H
