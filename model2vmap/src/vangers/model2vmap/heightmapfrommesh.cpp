#include "heightmapfrommesh.h"
#include <QFile>
#include <QDebug>
#include <iterator> // back_inserter
#include <vector>   // vector
#include <algorithm>
#include <span>
#include <float.h>
#include <functional>
#include <QTime>
#include <QImage>
#include <QFileInfo>
#include <QDir>

#include <obj/objreader.h>
#include <vangers/core/vector/vector.h>
#include <vangers/plugins/vmap/vmapvmpwriter.h>
#include <vangers/plugins/vmap/vmapmeta.h>
#include <vangers/plugins/vmap/vprwriter.h>

using namespace obj;
using namespace vangers::core::vector;
using namespace vangers::core::error;
using namespace vangers::plugins::vmap;

using Vec3f = Vector3<float>;
using Vec2f = Vector2<float>;
using Vec2i = Vector2<int32_t>;

struct level_t {
	std::vector<uint8_t> minLevel;
	std::vector<uint8_t> midLevel;
	std::vector<uint8_t> maxLevel;
	std::vector<uint8_t> minMaterial;
	std::vector<uint8_t> maxMaterial;
};

Vec3f barycentric(Vec3f A, Vec3f B, Vec3f C, Vec3f P) {
	Vec3f s[2];
	for (int i = 2; i--; ) {
		s[i].x = C[i]-A[i];
		s[i].y = B[i]-A[i];
		s[i].z = A[i]-P[i];
	}
	Vec3f u = s[0].cross(s[1]);

	if (std::abs(u.z) > 1e-2) // dont forget that u[2] is integer. If it is zero then triangle ABC is degenerate
		return Vec3f{
			1.f - (u.x + u.y) / u.z,
			u.y / u.z,
			u.x / u.z
		};
	return Vec3f{-1, 1, 1}; // in this case generate negative coordinates, it will be thrown away by the rasterizator
}



void triangle(const Vec3f pts[3], Vec2i screenSize, const std::function<void (const Vec2i&, const Vec3f&)>& shader) {
	Vec2f bboxmin{std::numeric_limits<float>::max(),  std::numeric_limits<float>::max()};
	Vec2f bboxmax{-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max()};

	Vec2f clamp{ screenSize.x - 1.0f, screenSize.y - 1.0f};

	for (int i=0; i<3; i++) {
		for (int j=0; j<2; j++) {
			bboxmin[j] = std::max(0.f,      std::min(bboxmin[j], pts[i][j]));
			bboxmax[j] = std::min(clamp[j], std::max(bboxmax[j], pts[i][j]));
		}
	}

	Vec2i screenPos;

	for (screenPos.x = bboxmin.x; screenPos.x <= bboxmax.x; screenPos.x++) {
		for (screenPos.y = bboxmin.y; screenPos.y <= bboxmax.y; screenPos.y ++) {
			Vec3f bc_screen = barycentric(pts[0], pts[1], pts[2], {(float)screenPos.x, (float)screenPos.y, 0});

			if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) continue;

//			if(bboxmax.y >= 8191){
//				int a = 0;
//			}

			Vec3f worldPos {0, 0, 0};
			for (int i=0; i < 3; i++) {
				worldPos.x += pts[i].x * bc_screen[i];
				worldPos.y += pts[i].y * bc_screen[i];
				worldPos.z += pts[i].z * bc_screen[i];
			}
			shader(screenPos, worldPos);
		}
	}
}

void calculateShadowLine(const std::span<uint8_t>& minLevel,
                     const std::span<uint8_t>& midLevel,
                     const std::span<uint8_t>& maxLevel,
                     const std::span<uint8_t>& isShadowed,
                     float shadowSlope){
	std::fill(isShadowed.begin(), isShadowed .end(), false);

	int size = minLevel.size();

	for(int iPos = size - 1; iPos >= 0; iPos --){
		int32_t hmin = minLevel[iPos];
		int32_t hmid = midLevel[iPos];
		int32_t hmax = maxLevel[iPos];

		int32_t hlow;
		int32_t hhigh;
		if(hmin == hmax) {
			hlow = 0;
			hhigh = hmax;
		} else {
			hlow = hmid;
			hhigh = hmax;
		}

		if(hhigh == 0){
			continue;
		}

		int32_t endPos = iPos - hhigh * shadowSlope;

		for(int iShadowPos = iPos - 1; iShadowPos >= endPos; iShadowPos--){
			int iShadowPosCycled = iShadowPos;
			if(iShadowPosCycled < 0){
				iShadowPosCycled = size + iShadowPosCycled % size;
			}

//			if(isShadowed[iShadowPosCycled]){
//				break;
//			}

			int32_t shadowH = maxLevel[iShadowPosCycled];
			int32_t deltaX = iPos - iShadowPos;
			int32_t targetShadowHigh = std::ceil((hhigh * shadowSlope) - deltaX)/shadowSlope;
			int32_t targetShadowLow = std::floor((hlow * shadowSlope) - deltaX)/shadowSlope;

			if(shadowH < targetShadowHigh &&
			   shadowH > targetShadowLow) {
				isShadowed[iShadowPosCycled] = true;
			}

		}
	}
}

void calculateShadow(
    level_t& level,
    std::vector<uint8_t>& isShadowed,
    vangers::model2vmap::RasterSettings settings){
	isShadowed.resize(level.maxLevel.size());
	auto worldSize = settings.worldSize;

	for(int iLine = 0; iLine < worldSize.y; iLine++){
		auto minLevel = std::span<uint8_t>(level.minLevel.begin() + iLine * worldSize.x,
		                                   worldSize.x);

		auto midLevel = std::span<uint8_t>(level.midLevel.begin() + iLine * worldSize.x,
		                                   worldSize.x);

		auto maxLevel = std::span<uint8_t>(level.maxLevel.begin() + iLine * worldSize.x,
		                                   worldSize.x);

		std::span<uint8_t> isShadowedLine = std::span<uint8_t>(isShadowed.begin() + iLine * worldSize.x,
		                                      isShadowed.begin() + (iLine + 1)* worldSize.x);
		calculateShadowLine(
		            minLevel,
		            midLevel,
		            maxLevel,
		            isShadowedLine,
		            settings.shadowAspect);
	}
}


void fixLevels(
        level_t& level,
        vangers::model2vmap::RasterSettings settings){
	auto worldSize = settings.worldSize;

	for(int i = 0; i < level.minLevel.size(); i++){
		int32_t x = i % worldSize.x;
		int32_t y = i / worldSize.x;


		uint8_t& hmin = level.minLevel[i];
		uint8_t& hmid = level.midLevel[i];
		uint8_t hmax = level.maxLevel[i];
		uint8_t& matMin = level.minMaterial[i];
		uint8_t& matMax = level.maxMaterial[i];

		if(hmax == hmid && hmid == hmin) {
			continue;
		}

		{
			if((int32_t)hmax - (int32_t)hmid < settings.minDoubleLevelThikness){
				if((int32_t)hmax - settings.minDoubleLevelThikness <= 0){
					hmin = hmid = hmax;
					matMin = matMax;
					continue;
				}

				hmid = hmax - settings.minDoubleLevelThikness;
			}
		}

		if((int32_t)hmax - (int32_t)hmin < settings.minUpDownLevelDist ||
		   (int32_t)hmid - (int32_t)hmin < settings.minUpDownLevelDist){

			hmin = hmid = hmax;
			matMin = matMax;
			continue;
		}
	}

}

void save_level(
        const std::vector<uint8_t>& level,
        vangers::model2vmap::RasterSettings settings,
        QString imgPath){
	uint8_t min = 255;
	uint8_t max = 0;

//		const auto& heights = heightsLevels[0];
	for(uint8_t h: level){
		min = std::min(min, h);
		max = std::max(max, h);
	}
	qDebug() << QTime::currentTime() << min << max << "writing image" << imgPath;
	QImage img(level.data(),
	            settings.worldSize.x,
	            settings.worldSize.y,
	            settings.worldSize.x,
	            QImage::Format_Grayscale8);
	img.save(imgPath);
}

void save_vpr(const QString& filename, const VmapGlobalParams& params){
	VprWriter writer;
	QFile f(filename);
	f.open(QFile::WriteOnly);
	writer.writeVpr(params, f);
	f.close();

}

Result vangers::model2vmap::heightmapFromMesh(QString objPath,
		QString outputDirPath, const RasterSettings& settings)
{
//	objPath ="/home/nikita/Sources/notebooks/vangers/data/cube.obj";

//	const Vector3I32 worldSize = {
//		2048,
//		2048,
//		256
//	};

	QDir outputDir(outputDirPath);
	outputDir.mkpath(".");

	if(settings.writeVpr){
		save_vpr(outputDir.filePath(QString("output.vpr")), settings.globalParams);
	}

	const Vector3I32& worldSize = settings.worldSize;
	const int32_t hThr = settings.hThr;

	QFileInfo objPathInfo(objPath);
	if(!objPathInfo.exists()){
		qWarning() << "file doens't exists" << objPath;
		return Error{QString("file doens't exists: ") + objPath};
	}

	QString objPathBaseName =  objPathInfo.baseName();

	qDebug() << QTime::currentTime() << "reading OBJ" << objPath ;
	ObjReader reader;
	QFile file(objPath);
	if(!file.open(QFile::ReadOnly)){
		return Error{"Cannot open the file: " + objPath};
	}

	ObjectCollection objCol;
	Result res = reader.read(objCol, file);
	if(!res) return res;


//	for(Vector3F64& vert: mergedObj.vertices){
//		if(vert.z < 0){
//			vert.z = 0;
//		}
//	}
//	mergedObj.triangulate();
	auto bounds = objCol.bounds();
	objCol.move(bounds.min * -1.0);
	bounds = objCol.bounds();

	Vector3F64 objSize = bounds.max;
	Vector3F64 worldScale = Vector3F64::fromVector(worldSize + Vector3I32{1, 1, 1}) / objSize;

	qDebug() << QTime::currentTime() << "rasterizing";

	level_t level{};
	level.minLevel.resize(worldSize.x * worldSize.y, 0);
	level.midLevel.resize(worldSize.x * worldSize.y, 0);
	level.maxLevel.resize(worldSize.x * worldSize.y, 0);
	level.minMaterial.resize(worldSize.x * worldSize.y, 0);
	level.maxMaterial.resize(worldSize.x * worldSize.y, 0);

	Vec2i worldSize2D = {worldSize.x, worldSize.y};

	QList<QString> materialNames;
	QList<const Object*> objects;
	for(const Object& obj: objCol.objects){
		objects.append(&obj);
	}

	std::sort(objects.begin(), objects.end(), [](const Object* a, const Object* b){
		auto downPredicate = [](const Group& group)  {
			return group.material.contains("down");
		};

		bool aIsDown = a->name.contains("down") ||
		               std::any_of(a->groups.constBegin(), a->groups.constEnd(), downPredicate);

		bool bIsDown = b->name.contains("down") ||
		               std::any_of(b->groups.constBegin(), b->groups.constEnd(), downPredicate);

		return aIsDown > bIsDown;
	});
	for(const Object* obj: objects){
		qDebug() << "Processing object" << obj->name
		         << ", vertices" << obj->vertices.size()
		            << ", normals" << obj->normals.size();

		for(const Group& group: obj->groups){
			qDebug() << "Processing group" << group.name
			         << ", faces" << group.faces.size();


			int32_t materialIndex = -1;
			if(group.material.toLower().replace(" ", "").startsWith("terrain")){
				materialIndex = group.material
								.mid(7, 1)
								.toInt();
			} else {
				materialIndex = materialNames.indexOf(group.material);
			}

			if(materialIndex < 0){
				materialNames.append(group.material);
				materialIndex = materialNames.size();
				if(materialIndex > 7){
					qDebug() << "wrong num of materials:" << materialNames.size();
					materialIndex = 7;
				}

			}

			bool isDownObj = obj->name.contains("down") ||
			                 group.material.contains("down");

			qDebug() << "Material" << group.material << "assigned index: " << materialIndex;
			qDebug() << "Is Down" << isDownObj;

			for(const Face& face: group.faces){
				assert(face.indices.size() == 3);

				Vec3f verts[3];
				Vec3f normals[3];
				for(int i = 0; i < 3; i++){
					const Vector3F64& v = obj->vertices[face.indices[i].vertexIndex];
					verts[i] = Vec3f::fromVector(v * worldScale);
					verts[i].x -= 1;
					verts[i].y -= 1;
					normals[i] = Vec3f::fromVector(obj->normals[face.indices[i].normalIndex].normalized());
				}

				auto side1 = (verts[1] - verts[0]).normalized();
				auto side2 = (verts[2] - verts[0]).normalized();
				auto cross = side1.cross(side2);

				auto normal = (normals[0] + normals[1] + normals[2]) / 3.0;
				float upFactor = cross.dot({0, 0, 1.0f});

				bool isUpFace = normal.z > 0;

				triangle(verts, worldSize2D, [&](const Vec2i& screenPos, const Vec3f& worldPos){
					if(screenPos.x < 0 ||
					   screenPos.y < 0 ||
					   screenPos.x >= worldSize.x
					   || screenPos.y >= worldSize.y) {
						return;
					}

					float heightF = worldPos.z;

					if(heightF > 255){
	//					qDebug() << "invalid height" << heightF;
						heightF = 255;
					}

					uint8_t height = (uint8_t)heightF;

					if((int32_t)height <= hThr) {
						return;
					}

					int32_t index = (screenPos.x) + worldSize2D.x * (screenPos.y);

					uint8_t& hmin = level.minLevel[index];
					uint8_t& hmid = level.midLevel[index];
					uint8_t& hmax = level.maxLevel[index];
					uint8_t& matMin = level.minMaterial[index];
					uint8_t& matMax = level.maxMaterial[index];

					if(settings.doubleLevelStrategy == DoubleLevelStrategy::TakeMax){
						if(hmax < height){
							hmin = hmid = hmax = height;
							matMin = matMax = (uint8_t)materialIndex;
						}
						return;
					}

					if(isDownObj){
						if(hmax < height){
							hmin = hmid = hmax = height;
							matMin = matMax = (uint8_t)materialIndex;
						}

						return;
					}

					if(isUpFace){
						if(hmax < height){
							hmax = height;
							matMax = (uint8_t)materialIndex;
						}
					} else {
						if(hmid < height){
							hmid = height;
						}
					}

				});
			}
		}
	}

	fixLevels(level, settings);

	std::vector<uint8_t> isShadowed;

	calculateShadow(level, isShadowed, settings);


	save_level(level.minLevel,
	           settings,
	           outputDir.filePath(QString("%1.height.min.png").arg(objPathBaseName)));

	save_level(level.midLevel,
	           settings,
	           outputDir.filePath(QString("%1.height.mid.png").arg(objPathBaseName)));

	save_level(level.maxLevel,
	           settings,
	           outputDir.filePath(QString("%1.height.max.png").arg(objPathBaseName)));

	save_level(level.minMaterial,
	           settings,
	           outputDir.filePath(QString("%1.material.min.png").arg(objPathBaseName)));

	save_level(level.maxMaterial,
	           settings,
	           outputDir.filePath(QString("%1.material.max.png").arg(objPathBaseName)));


	QVector<QRgb> colorTable;
	colorTable.resize(256);
	std::fill(colorTable.begin(), colorTable.end(), 0);

	for(int iColor = 0; iColor < materialNames.size(); iColor++){
		const QString& matName = materialNames[iColor];
		colorTable[iColor] = qRgb(qrand() % 255, qrand() % 255, qrand() % 255);
//			for(int iMat = 0; iMat < objCol.materials.size(); iMat++){
//				const auto& mat = objCol.materials[iMat];
//				if(mat.name == matName){
//					const auto& color = mat.diffuseColor;
//					colorTable[iColor] = qRgb(color.x * 255, color.y * 255, color.z * 255);
//					break;
//				}
//			}
	}

//	for(int i = 0; i < 2; i++){
//		QString imgFileName = QString("%1.material.%2.png").arg(objPathBaseName).arg(i);;
//		QString imgPath = outputDir.filePath(imgFileName);

//		uint8_t min = 255;
//		uint8_t max = 0;
//		auto& materials = materialLevels[1];
//		for(uint8_t m: materials){
//			min = std::min(min, m);
//			max = std::max(max, m);
//		}
//		qDebug() << QTime::currentTime() << min << max << "writing image" << imgPath;
//		QImage img(materials.data(),
//					worldSize2D.x,
//					worldSize2D.y,
//					worldSize2D.x,
//					QImage::Format_Indexed8);

//		img.setColorTable(colorTable);
//		img.save(imgPath);
//	}

//	{
//		QVector<uint32_t> height3;
//		height3.resize(heightsLevels[0].size());

//		for(int i = 0; i < height3.size(); i++){
//			uint8_t hmin = heightsLevels[0][i];
//			uint8_t hmid = heightsLevels[1][i];
//			uint8_t hmax = heightsLevels[2][i];
//			QRgb col = qRgb(hmin, hmax, hmid - hmin);
////			QRgb col = qRgb(hmax, hmax, 0);
//			height3[i] = col;
//		}

//		QString imgFileName = QString("%1.height3.png").arg(objPathBaseName);
//		QString imgPath = outputDir.filePath(imgFileName);

//		qDebug() << QTime::currentTime() << "writing image" << imgPath;
//		QImage img((uint8_t*)(height3.data()),
//					worldSize2D.x,
//					worldSize2D.y,
//					worldSize2D.x * sizeof(uint32_t),
//					QImage::Format_RGB32);
//		img.save(imgPath);
//	}
	{
		Vmap vmap;
		auto& meta = vmap.meta();
		auto& height = vmap.height();
		vmap.setSize({worldSize.x, worldSize.y});

		meta.resize(worldSize.x, worldSize.y);
		height.resize(worldSize.x, worldSize.y);

		for(size_t i = 0; i < meta.size(); i++){
			size_t evenI = (i & ~1);
			size_t oddI = (i | 1);

			uint8_t hmin = level.minLevel[i];
			uint8_t hmid = level.midLevel[i];
			uint8_t hmax = level.maxLevel[i];
			uint8_t matMin = level.minMaterial[i];
			uint8_t matMax = level.maxMaterial[i];

			bool shadowFlag = isShadowed[i];

			// TODO:
			shadowFlag = false;

			if(hmin == hmax){
				// No double level
				height[i] = hmax;
				meta[i] = VmapMeta(0, false, matMax, false, shadowFlag).toMeta();
			} else if(i == oddI){
				height[i] = hmin;
				meta[i] = VmapMeta(0, false, matMin, false, shadowFlag).toMeta();
			} else {
				i++;
				height[evenI] = hmin;
				height[oddI] = hmax;
				int32_t deltaI32 = (hmid - hmin) >> 3;
				uint8_t delta = deltaI32;
				if(deltaI32 > 15 || deltaI32 < 0) {
					delta = 15;
				}
				uint8_t evenDelta = (delta >> 2) & 0b11;
				uint8_t oddDelta = delta & 0b11;

				uint8_t evenMaterial = matMin;
				uint8_t oddMaterial = matMax;

				meta[evenI] = VmapMeta(evenDelta, false, evenMaterial, true, shadowFlag).toMeta();
				meta[oddI] = VmapMeta(oddDelta, false, oddMaterial, true, shadowFlag).toMeta();
			}
		}

		QString vmpFileName = QString("output.vmp");
		QString vmpPath = outputDir.filePath(vmpFileName);

		qDebug() << QTime::currentTime() << "writing vmp" << vmpPath;
		QFile vmpFile(vmpPath);
		vmpFile.open(QFile::WriteOnly);

		VmapVmpWriter writer;
		writer.write(vmap, vmpFile);
		vmpFile.close();
	}


	return {};
}



