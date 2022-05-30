#include "voxbinaryreaderext.h"
#include <riff/riff.h>
#include <riff/riffbinaryreaderext.h>
#include <QBuffer>

using namespace vangers;
using namespace vangers::vox;
using namespace vangers::riff;

template<>
bool BinaryReader::tryRead<>(QString & str){
	int32_t length;
	if(!tryRead(length)) return false;
	str = read(length);
	return true;
}

template<>
bool BinaryReader::tryRead<>(Dict & dict){
	int32_t numPairs;
	if(!tryRead(numPairs)) return false;

	for(int  i =0; i < numPairs; i++){
		QString key;
		QString value;

		if(!tryRead(key)) {
			return false;
		}
		if(!tryRead(value)) {
			return false;
		}
		dict.append({key, value});
	}

	return true;
}

template<>
bool BinaryReader::tryRead<>(Voxel& voxel){
	if(!tryRead(voxel.data[0])) return false;
	if(!tryRead(voxel.data[1])) return false;
	if(!tryRead(voxel.data[2])) return false;
	if(!tryRead(voxel.data[3])) return false;
	return true;
}

template<>
bool BinaryReader::tryRead<>(ShapeNodeModel& model){
	if(!tryRead(model.modelId)) {
		return false;
	}
	if(!tryRead(model.modelAttributes)) {
		return false;
	}
	return true;
}

bool decodeChunkContents(TransformNode& tr, BinaryReader& reader){
	if(!reader.tryRead(tr.nodeId)) return false;
	if(!reader.tryRead(tr.attributes)) return false;

	if(!reader.tryRead(tr.childNodeId)) return false;
	if(!reader.tryRead(tr.reservedNodeId)) return false;
	if(!reader.tryRead(tr.layerId)) return false;
	int32_t numberOfFrames;
	if(!reader.tryRead(numberOfFrames)) return false;

	if(!reader.tryReadArray(tr.frameAttributes, numberOfFrames)) return false;

	return true;
}

bool decodeChunkContents(GroupNode& group, BinaryReader& reader){
	if(!reader.tryRead(group.nodeId)) return false;
	if(!reader.tryRead(group.attributes)) return false;
	int32_t numChildrenNodes;
	if(!reader.tryRead(numChildrenNodes)) return false;
	if(!reader.tryReadArray(group.childNodeIds, numChildrenNodes)) return false;

	return true;
}

bool decodeChunkContents(ShapeNode& shape, BinaryReader& reader){
	if(!reader.tryRead(shape.nodeId)) {
		return false;
	}

	if(!reader.tryRead(shape.attributes)) {
		return false;
	}
	int32_t numModel;
	if(!reader.tryRead(numModel)) {
		return false;
	}
	if(!reader.tryReadArray(shape.models, numModel)) {
		return false;
	};

	return true;
}

bool decodeChunkContents(Pack& pack, BinaryReader& reader){
	if(!reader.tryRead(pack.numModels)) return false;
	return true;
}

bool decodeChunkContents(Model& model, BinaryReader& reader){
	if(!reader.tryRead(model.sizeX)) return false;
	if(!reader.tryRead(model.sizeY)) return false;
	if(!reader.tryRead(model.sizeZ)) return false;
	return true;
}

bool decodeChunkContents(ModelVoxels& model, BinaryReader& reader){
	int32_t numVoxels;
	if(!reader.tryRead(numVoxels)) return false;
	if(!reader.tryReadArray(model.voxels, numVoxels)) return false;
	return true;
}

bool decodeChunkContents(Material& material, BinaryReader& reader){
	if(!reader.tryRead(material.id)) return false;
	if(!reader.tryRead(material.attributes)) return false;
	return true;
}

bool decodeChunkContents(Layer& layer, BinaryReader& reader){
	if(!reader.tryRead(layer.id)) return false;
	if(!reader.tryRead(layer.attributes)) return false;
	if(!reader.tryRead(layer.reservedId)) return false;
	return true;
}

bool decodeChunkContents(RenderObjects& ro, BinaryReader& reader){
	if(!reader.tryRead(ro.attributes)) return false;
	return true;
}

bool decodeChunkContents(RenderCamera& rc, BinaryReader& reader){
	if(!reader.tryRead(rc.id)) return false;
	if(!reader.tryRead(rc.attributes)) return false;
	return true;
}

bool decodeChunkContents(PaletteNote& paletteNote, BinaryReader& reader){
	int32_t numColorNames;

	if(!reader.tryRead(numColorNames)) return false;
	if(!reader.tryReadArray(paletteNote.colorNames, numColorNames)) return false;
	return true;
}

bool decodeChunkContents(IndexMap& indexMap, BinaryReader& reader){
	for(int i = 0; i < 256; i++){
		if(!reader.tryRead(indexMap.paletteIndices[i])) return false;
	}

	return true;
}

bool decodeChunkContents(Palette& palette, BinaryReader& reader){
	for(int i = 0; i < 256; i++){
		if(!reader.tryRead(palette.colors[i])) return false;
	}

	return true;
}

template<typename T>
bool decodeChunk(T& model, Chunk& chunk){
	QBuffer buffer(&chunk.content);
	buffer.open(QBuffer::ReadOnly);
	BinaryReader reader(&buffer);
	bool res = decodeChunkContents(model, reader);
	buffer.close();
	return res;
}

template<typename TContainer>
typename TContainer::value_type& pushEmpty(TContainer& container){
	container.push_back(typename TContainer::value_type{});
	return container.back();
}

template<>
bool BinaryReader::tryRead<>(Vox& vox)
{
	Riff riff;
	tryRead(riff);
	if(riff.chunks.size() != 1){
		return false;
	}

	Chunk& mainChunk = riff.chunks[0];

	if(!mainChunk.hasId("MAIN")) return false;

	int currentChunkIndex = 0;
	while(currentChunkIndex < mainChunk.children.size()){
		Chunk& chunk = mainChunk.children[currentChunkIndex++];
		QString id(chunk.id);
		id.resize(4);
		qDebug() << id;

		if(chunk.hasId("PACK")){
			if(!decodeChunk(vox.pack, chunk)) {
				return false;
			}
		} else if(chunk.hasId("SIZE")) {
			Chunk& xyzIChunk = mainChunk.children[currentChunkIndex++];
			Model& model = pushEmpty(vox.models);

			if(!decodeChunk(model, chunk)) {
				return false;
			}
			if(!decodeChunk(model.voxelsCollection, xyzIChunk)) {
				return false;
			}
		} else if(chunk.hasId("nTRN")) {
			if(!decodeChunk(pushEmpty(vox.transformNodes), chunk)) {
				return false;
			}
			qDebug() << "TransformNode" << vox.transformNodes.last().nodeId << "child" << vox.transformNodes.last().childNodeId;
		} else if(chunk.hasId("nGRP")) {
			if(!decodeChunk(pushEmpty(vox.groupNodes), chunk)) {
				return false;
			}
			qDebug() << "GroupNode" << vox.groupNodes.last().nodeId << "children" << vox.groupNodes.last().childNodeIds;
		} else if(chunk.hasId("nSHP")) {
			if(!decodeChunk(pushEmpty(vox.shapeNodes), chunk)) {
				return false;
			}
			qDebug() << "ShapeNode" << vox.shapeNodes.last().nodeId;
		} else if(chunk.hasId("MATL")) {
			if(!decodeChunk(pushEmpty(vox.materials), chunk)) {
				return false;
			}
		} else if(chunk.hasId("LAYR")) {
			if(!decodeChunk(pushEmpty(vox.layers), chunk)) {
				return false;
			}
		} else if(chunk.hasId("rCAM")) {
			if(!decodeChunk(pushEmpty(vox.renderCameras), chunk)) {
				return false;
			}
		} else if(chunk.hasId("rOBJ")) {
			if(!decodeChunk(pushEmpty(vox.renderObjects), chunk)) {
				return false;
			}
		} else if(chunk.hasId("NOTE")) {
			if(!decodeChunk(pushEmpty(vox.paletteNotes), chunk)) {
				return false;
			}
		} else if(chunk.hasId("IMAP")) {
			if(!decodeChunk(pushEmpty(vox.indexMaps), chunk)) {
				return false;
			}
		} else if(chunk.hasId("RGBA")) {
			if(!decodeChunk(pushEmpty(vox.paletteOptional), chunk)) {
				return false;
			}
		} else {
			qDebug() << "Unknown chunk" << id;
		}
	}


	return true;
}
