#include <vangers/core//palette/palette.h>
#include <vangers/core//binaryio.h>

#include <vox/vox.h>
#include <vox/voxbinarywriterext.h>


#include "vmapmeta.h"
#include "vmapvoxwriter.h"
#include "layer/heightimagelayer.h"
#include "layer/terrainimagelayer.h"
#include "layer/doublelevelimagelayer.h"

#include <qdir.h>
#include <QtMath>

using namespace vangers::plugins::vmap;
using namespace vangers::core::palette;
using namespace vangers::core::binario;
using namespace vox;

// TODO: move to common
template<typename TContainer>
typename TContainer::value_type& pushEmpty(TContainer& container){
	container.push_back(typename TContainer::value_type{});
	return container.back();
}


void getHeights(
		const Matrix<uint8_t>& height,
		const Matrix<uint8_t>& meta,
		int32_t x,
		int32_t y,
		uint8_t& downHeight){
	VmapMeta xMeta = VmapMeta::fromMeta(meta.getData(x, y));
	if(!xMeta.isDoubleLevel()){
		downHeight = height.getData(x, y);
	} else {
		uint32_t downX = x & ~1;
		uint32_t upX = y | 1;

//		VmapMeta downMeta = VmapMeta::fromMeta(meta.getData(downX, y));
//		VmapMeta upMeta = VmapMeta::fromMeta(meta.getData(upX, y));

//		uint8_t downC = downMeta.terrain() + 1;
//		uint8_t upC = upMeta.terrain() + 1;
		downHeight = height.getData(downX, y);
//		upHeight = height.getData(upX, y);

//		delta = (downMeta.delta() << 2) + upMeta.delta();
	}
}

VmapVoxWriterSettings VmapVoxWriterSettings::makeDefault()
{
	return {
		.hollow = true,
		.hollowSize = 32,
		.indexColorShift = 24,
		.compressChunks = false,
		.flipX = true,
		.voxSizeX = 2048,
		.voxSizeY = 2048,
		.chunkSizeX = 128,
		.chunkSizeY = 128,
	};
}


void VmapVoxWriter::write(const Vmap& vmap, QString filename, const VmapVoxWriterSettings& settings)
{
	const QSet<uint8_t> excludedTerrainIndices = {};
	const bool hollow = settings.hollow; //true
	const int8_t hollowSize = settings.hollowSize; // 32
	const uint8_t colorIndexShift = settings.indexColorShift; // 24;
	const bool compressChunks = settings.compressChunks;// false;
	const bool flipX = settings.flipX; // true;

	int sizeX = vmap.size().width();
	int sizeY = vmap.size().height();
	const int sizeZ = 256;

	const int32_t voxSizeX = qMin(settings.voxSizeX/*2048*/, sizeX);
	const int32_t voxSizeY = qMin(settings.voxSizeY/*2048*/, sizeY);

	const int32_t chunkSizeX = settings.chunkSizeX; //128;
	const int32_t chunkSizeY = settings.chunkSizeY; //128;


	auto palette = vangers::core::palette::Palette::grayscale();
	TerrainImageLayer terrainLayer;
	const Matrix<uint8_t>& height = vmap.heightConst();
	const Matrix<uint8_t>& meta = vmap.metaConst();

	auto terrainPalette = terrainLayer.getPalette(vmap);

	int32_t numVoxX = sizeX / voxSizeX;
	int32_t numVoxY = sizeY / voxSizeY;

	for(int iVoxY = 0; iVoxY < numVoxY; iVoxY++){
		for(int iVoxX = 0; iVoxX < numVoxX; iVoxX++){
			int32_t numChunksX = voxSizeX / chunkSizeX;
			int32_t numChunksY = voxSizeY / chunkSizeX;
			QFileInfo fInfo(filename);
			QString voxFileName;
			if(numVoxX == 1 && numVoxY == 1){
				voxFileName = fInfo.absoluteDir().filePath(QString("%1.vox")
																	.arg(fInfo.baseName()))
																;

			} else {
				voxFileName = fInfo.absoluteDir().filePath(QString("%1.%2.%3.vox")
																	.arg(fInfo.baseName())
																	.arg(iVoxX)
																	.arg(iVoxY));
			}

			qDebug() << "Saving vox" << voxFileName;

			Vox vox;

			int iNode = 0;
			int iModel = 0;

			TransformNode& rootTransform = pushEmpty(vox.transformNodes);
			rootTransform.nodeId = iNode++;
			GroupNode& rootGroup = pushEmpty(vox.groupNodes);
			rootGroup.nodeId = iNode++;

			rootTransform.attributes = {};
			rootTransform.childNodeId = rootGroup.nodeId;
			rootTransform.layerId = -1;
			rootTransform.frameAttributes = {{}};
			rootTransform.reservedNodeId = -1;

			rootGroup.attributes = {};

			for(int iChunkY = 0; iChunkY < numChunksY; iChunkY++){
				for(int iChunkX = 0; iChunkX < numChunksX; iChunkX++){
					int32_t maxZ = 0;
					int32_t minZ = 256;

					qDebug() << "Saving model" << iModel << "of" << numChunksX * numChunksY << ", chunk:" << iChunkX << iChunkY;

					Model& model = pushEmpty(vox.models);

					for(int iX = 0; iX < chunkSizeX; iX++){
						for(int iY = 0; iY < chunkSizeY; iY++){
							int32_t realIX = iX + chunkSizeX * (iChunkX + iVoxX * numChunksX);
							int32_t realIY = iY + chunkSizeY * (iChunkY + iVoxY * numChunksY);

							if(flipX){
								realIX =  sizeX - 1 - realIX;
							}

							uint8_t heighbourDownHeights[4];

							getHeights(height, meta,
									   qMax(0, realIX - 1),
									   realIY,
									   heighbourDownHeights[0]);

							getHeights(height, meta,
									   qMin(sizeX - 1, realIX + 1),
									   realIY,
									   heighbourDownHeights[1]);

							getHeights(height, meta,
									   realIX,
									   qMax(0, realIY - 1),
									   heighbourDownHeights[2]);

							getHeights(height, meta,
									   realIX,
									   qMin(sizeY - 1, realIY + 1),
									   heighbourDownHeights[3]);

							uint8_t heighbourDownHeightMin = 255;
							for(int i = 0; i < 4; i++){
								heighbourDownHeightMin = qMin(
															 heighbourDownHeightMin,
															 (uint8_t)qMax(0, (int32_t)heighbourDownHeights[i] - (int32_t)hollowSize));
							}


							VmapMeta xMeta = VmapMeta::fromMeta(meta.getData(realIX, realIY));
							if(!xMeta.isDoubleLevel()){
								if(excludedTerrainIndices.contains(xMeta.terrain())) {
									continue;
								}
								uint8_t h = height.getData(realIX, realIY);
								uint8_t c = xMeta.terrain() + 1 + colorIndexShift;

								heighbourDownHeightMin = qMin(
															 heighbourDownHeightMin,
															 (uint8_t)qMax(
																 (int32_t)heighbourDownHeightMin,
																 (int32_t)h - (int32_t)hollowSize));

								uint8_t startIndex = hollow
													 ? heighbourDownHeightMin
													 : 0;
								minZ = qMin(minZ, (int32_t)startIndex);
								maxZ = qMax(maxZ, (int32_t)h);

								for(int32_t iZ = startIndex; iZ < h; iZ++){
									minZ = qMin(minZ, iZ);
									maxZ = qMax(maxZ, iZ);
									model.voxelsCollection.voxels.push_back(Voxel {.data{
															   (int8_t)iX,
															   (int8_t)iY,
															   (int8_t)iZ,
															   (int8_t)c,
														   }});
								}
							} else {
								uint32_t downX = realIX & ~1;
								uint32_t upX = realIX | 1;

								VmapMeta downMeta = VmapMeta::fromMeta(meta.getData(downX, realIY));
								VmapMeta upMeta = VmapMeta::fromMeta(meta.getData(upX, realIY));

								uint8_t downC = downMeta.terrain() + 1 + colorIndexShift;
								uint8_t upC = upMeta.terrain() + 1 + colorIndexShift;
								uint8_t downHeight = height.getData(downX, realIY);
								uint8_t upHeight = height.getData(upX, realIY);


								int32_t delta = (downMeta.delta() << 2) + upMeta.delta();

								heighbourDownHeightMin = qMin(
															 heighbourDownHeightMin,
															 (uint8_t)qMax(
																 (int32_t)heighbourDownHeightMin,
																 (int32_t)downHeight - (int32_t)hollowSize));

								uint8_t startIndex = hollow
													 ? heighbourDownHeightMin
													 : 0;
								minZ = qMin(minZ, (int32_t)startIndex);
								maxZ = qMax(maxZ, (int32_t)downHeight);

								for(int iZ = startIndex; iZ < downHeight; iZ++){
									minZ = qMin(minZ, iZ);
									maxZ = qMax(maxZ, iZ);
									model.voxelsCollection.voxels.push_back(Voxel {.data{
															   (int8_t)iX,
															   (int8_t)iY,
															   (int8_t)iZ,
															   (int8_t)downC,
														   }});
								}

								minZ = qMin(minZ, upHeight - delta);
								maxZ = qMax(maxZ, (int32_t)upHeight);
								for(int iZ = upHeight - delta; iZ < upHeight; iZ++){

									model.voxelsCollection.voxels.push_back(Voxel {.data{
															   (int8_t)iX,
															   (int8_t)iY,
															   (int8_t)iZ,
															   (int8_t)upC,
														   }});
								}
							}
						}
					}

					int32_t modelX = iChunkX * chunkSizeX - voxSizeX/2 + chunkSizeX/2;
					int32_t modelY = iChunkY * chunkSizeY - voxSizeY/2 + chunkSizeY/2;
					int32_t modelZ = 0;

					if(!compressChunks){
						model.sizeX = chunkSizeX;
						model.sizeY = chunkSizeY;
						model.sizeZ = sizeX;
					} else {
						int32_t compSizeZ = (maxZ - minZ + 1);

						for(Voxel& v: model.voxelsCollection.voxels){
							v.data[2] -= minZ;
						}

						model.sizeX = chunkSizeX;
						model.sizeY = chunkSizeY;
						model.sizeZ = compSizeZ;

						modelZ = (maxZ + minZ)/2;
					}


					TransformNode& transform = pushEmpty(vox.transformNodes);
					transform.nodeId = iNode++;

					ShapeNode& shape = pushEmpty(vox.shapeNodes);
					shape.nodeId = iNode++;
					shape.attributes = {};
					shape.models = {
						{
							.modelId = iModel,
							.modelAttributes = {}
						}
					};


					QString transformStr = QString("%1 %2 %3")
										   .arg(modelX)
										   .arg(modelY)
										   .arg(modelZ);

					transform.attributes = {};
					transform.childNodeId = shape.nodeId;
					transform.reservedNodeId = -1;
					transform.layerId = 0;
					transform.frameAttributes = {
						{{"_t", transformStr}}
					};

					rootGroup.childNodeIds.append(transform.nodeId);



					iModel ++;
				}
			}

//			for(int i = 0; i < 32; i++){
//				Layer& layer = pushEmpty(vox.layers);
//				layer.id = i;
//				layer.attributes = {{"_color", "150 150 150"}};
//				layer.reservedId = -1;
//			}

			vox.paletteOptional.push_back(vox::Palette{});
			vox::Palette& voxPallette = vox.paletteOptional[0];

			for(int i = 0; i < terrainPalette.size() && i < 255 - colorIndexShift; i++){
				QRgb color = terrainPalette[i];
				uint8_t* voxPaletteColor = (uint8_t*)(&voxPallette.colors[i + colorIndexShift]);
				voxPaletteColor[0] = qRed(color);
				voxPaletteColor[1] = qGreen(color);
				voxPaletteColor[2] = qBlue(color);
				voxPaletteColor[3] = qAlpha(color);
			}

			vox.pack.numModels = vox.models.size();

			QFile file(voxFileName);

			if(!file.open(QFile::WriteOnly)){
				return;
			}

			qDebug() << "Writing VOX...";
			BinaryWriter writer(&file);
			writer.write(vox);
			qDebug() << "Done writing VOX";
		}
	}
	qDebug() << "Done writing world";

}
