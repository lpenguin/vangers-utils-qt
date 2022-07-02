#include <riff/riff.h>
#include <riff/riffencoding.h>
#include <riff/riffbinarywriterext.h>
#include <riff/riffbinaryreaderext.h>

#include "voxbinarywriterext.h"
#include <cstring>

#include <QBuffer>
using namespace vangers::core::binario;
using namespace riff;
using namespace vox;


template<>
void BinaryWriter::write<>(const QString& str){
	QByteArray encoded = str.toLocal8Bit();
	write<int32_t>(encoded.size());

	for(int i = 0; i < encoded.size(); i++){
		write<uint8_t>(encoded[i]);
	}
}

template<>
void BinaryWriter::write<>(const Dict& dict){
	write<int32_t>(dict.size());
	for(const auto& kv: dict){
		write(kv.first);
		write(kv.second);
	}
}

template<>
void BinaryWriter::write<>(const ShapeNodeModel& nodeModel){
	write<int32_t>(nodeModel.modelId);
	write(nodeModel.modelAttributes);
}

template<>
void riff::encodeChunkContents<>(const TransformNode& node, BinaryWriter& writer, Chunk& chunk){
	chunk.setId("nTRN");
	writer.write(node.nodeId);
	writer.write(node.attributes);
	writer.write(node.childNodeId);
	writer.write(node.reservedNodeId);
	writer.write(node.layerId);
	writer.write<int32_t>(node.frameAttributes.size());
	writer.writeArray(node.frameAttributes);
}

template<>
void riff::encodeChunkContents<>(const ShapeNode& node, BinaryWriter& writer, Chunk& chunk){
	chunk.setId("nSHP");
	writer.write(node.nodeId);
	writer.write(node.attributes);
	writer.write<int32_t>(node.models.size());
	writer.writeArray(node.models);
}

template<>
void riff::encodeChunkContents<>(const Layer& layer, BinaryWriter& writer, Chunk& chunk){
	chunk.setId("LAYR");
	writer.write(layer.id);
	writer.write(layer.attributes);
	writer.write(layer.reservedId);
}

template<>
void riff::encodeChunkContents<>(const GroupNode& node, BinaryWriter& writer, Chunk& chunk){
	chunk.setId("nGRP");
	writer.write(node.nodeId);
	writer.write(node.attributes);
	writer.write<int32_t>(node.childNodeIds.size());
	writer.writeArray(node.childNodeIds);
}


template<>
void riff::encodeChunkContents<Pack>(const Pack& pack, BinaryWriter& writer, Chunk& chunk){
	chunk.setId("PACK");
	writer.write(pack.numModels);
}

template<>
void riff::encodeChunkContents(const Model& model, BinaryWriter& writer, Chunk& chunk){
	chunk.setId("SIZE");

	writer.write(model.sizeX);
	writer.write(model.sizeY);
	writer.write(model.sizeZ);
}

template<>
void riff::encodeChunkContents(const ModelVoxels& modelVoxels, BinaryWriter& writer, Chunk& chunk){
	chunk.setId("XYZI");
	writer.write(modelVoxels.voxels.size());

	for(const Voxel& voxel: modelVoxels.voxels){
		writer.write(voxel.data[0]);
		writer.write(voxel.data[1]);
		writer.write(voxel.data[2]);
		writer.write(voxel.data[3]);
	}
}

template<>
void riff::encodeChunkContents(const Material& material, BinaryWriter& writer, Chunk& chunk){
	chunk.setId("MATL");
	writer.write(material.id);
	writer.write(material.attributes);
}

template<>
void riff::encodeChunkContents(const RenderObjects& ro, BinaryWriter& writer, Chunk& chunk){
	chunk.setId("rOBJ");
	writer.write(ro.attributes);
}

template<>
void riff::encodeChunkContents(const RenderCamera& rc, BinaryWriter& writer, Chunk& chunk){
	chunk.setId("rCAM");
	writer.write(rc.id);
	writer.write(rc.attributes);
}

template<>
void riff::encodeChunkContents(const PaletteNote& note, BinaryWriter& writer, Chunk& chunk){
	chunk.setId("NOTE");
	writer.write<int32_t>(note.colorNames.size());
	writer.writeArray(note.colorNames);
}

enum class NodeType {
	Transform,
	Shape,
	Group
};

struct TreeNode {
	NodeType type;
	const Node* node;
};

void makeNodeTree(const Vox& vox, QMap<int32_t, TreeNode>& tree){
	for(const TransformNode& n: vox.transformNodes){
		tree[n.nodeId] = TreeNode {
			.type = NodeType::Transform,
			.node = &n,
		};
	}

	for(const ShapeNode& n: vox.shapeNodes){
		tree[n.nodeId] = TreeNode {
			.type = NodeType::Shape,
			.node = &n,
		};
	}

	for(const GroupNode& n: vox.groupNodes){
		tree[n.nodeId] = TreeNode {
			.type = NodeType::Group,
			.node = &n,
		};
	}
}

void traverseNodeTree(const QMap<int32_t, TreeNode>& tree, int id, Chunk& rootChunk){
	const TreeNode& node= tree[id];

	switch(node.type){
	case NodeType::Transform:
	{

		const TransformNode* tr = (TransformNode*)node.node;
		encodeChildChunk(*tr, rootChunk);
		traverseNodeTree(tree, tr->childNodeId, rootChunk);
	}
		break;
	case NodeType::Shape:
	{
		const ShapeNode* shape = (ShapeNode*)node.node;
		encodeChildChunk(*shape, rootChunk);
	}
		break;
	case NodeType::Group:
	{
		const GroupNode* group = (GroupNode*)node.node;
		encodeChildChunk(*group, rootChunk);
		for(int i = group->childNodeIds.size() - 1; i >= 0; i--){
			int32_t childId = group->childNodeIds[i];
			traverseNodeTree(tree, childId, rootChunk);
		}
	}
		break;
	}
}
template<>
void riff::encodeChunkContents(const Vox& vox, BinaryWriter& , Chunk& chunk){
	chunk.setId("MAIN");

//	encodeChildChunk(vox.pack, chunk);
	for(const Model& model: vox.models){
		encodeChildChunk(model, chunk);
		encodeChildChunk(model.voxelsCollection, chunk);
	}

	QMap<int32_t, TreeNode> nodeTree;
	makeNodeTree(vox, nodeTree);
	traverseNodeTree(nodeTree, 0, chunk);

//	for(const TransformNode& n: vox.transformNodes){
//		encodeChildChunk(n, chunk);
//	}

//	for(const ShapeNode& n: vox.shapeNodes){
//		encodeChildChunk(n, chunk);
//	}

//	for(const GroupNode& n: vox.groupNodes){
//		encodeChildChunk(n, chunk);
//	}
//	encodeChildChunk(vox.transformNodes[0], chunk);
//	encodeChildChunk(vox.groupNodes[0], chunk);
//	encodeChildChunk(vox.transformNodes[1], chunk);
//	encodeChildChunk(vox.shapeNodes[0], chunk);

	for(const Layer& layer: vox.layers){
		encodeChildChunk(layer, chunk);
	}

	for(const Palette& pal: vox.paletteOptional){
		encodeChildChunk(pal, chunk);
	}

	for(const Material& mat: vox.materials){
		encodeChildChunk(mat, chunk);
	}

	for(const RenderObjects& robj: vox.renderObjects){
		encodeChildChunk(robj, chunk);
	}

	for(const RenderCamera& rcam: vox.renderCameras){
		encodeChildChunk(rcam, chunk);
	}

	for(const PaletteNote& note: vox.paletteNotes){
		encodeChildChunk(note, chunk);
	}
}

template<>
void riff::encodeChunkContents(const Palette& palette, BinaryWriter& writer, Chunk& chunk){
	chunk.setId("RGBA");

	for(int i = 0; i < 256; i++){
		writer.write(palette.colors[i]);
	}
}

template<>
void BinaryWriter::write<Vox>(const Vox& vox)
{
	Riff riff;
	riff.setId("VOX ");
	riff.version = 150;
	riff.chunks.append(Chunk{});

	Chunk& mainChunk = riff.chunks.last();
	encodeChunk(vox, mainChunk);
	write(riff);
}
