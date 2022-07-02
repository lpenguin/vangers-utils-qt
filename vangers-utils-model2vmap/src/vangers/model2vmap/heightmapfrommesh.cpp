#include "heightmapfrommesh.h"
#include <QFile>
#include <QDebug>
#include <iterator> // back_inserter
#include <vector>   // vector
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

using namespace obj;
using namespace vangers::core::vector;
using namespace vangers::plugins::vmap;

using Vec3f = Vector3<float>;
using Vec2f = Vector2<float>;
using Vec2i = Vector2<int32_t>;

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

void calculateShadow(uint8_t* line, bool* isShadowed, size_t size, float shadowSlope){
	std::fill(isShadowed, isShadowed + size, false);

	for(int iPos = size - 1; iPos >= 0; iPos --){
		int32_t h = (int32_t)line[iPos];
		if(h == 0){
			continue;
		}
		for(int iShadowPos = iPos - 1; iShadowPos >= 0; iShadowPos--){
			if(isShadowed[iShadowPos]){
				break;
			}

			int32_t shadowH = line[iShadowPos];
			int32_t hDelta = h - shadowH;
			int32_t lDelta = iPos - iShadowPos;
			int32_t targetShadowH = ((h * shadowSlope) - lDelta)/shadowSlope;
			int32_t targetHDelta = h - targetShadowH;

			if(targetHDelta > hDelta) {
				break;
			}

			isShadowed[iShadowPos] = true;
		}
	}
}

bool vangers::model2vmap::heightmapFromMesh(QString objPath,
		QString outputDirPath, const RasterSettings& settings)
{
//	objPath ="/home/nikita/Sources/notebooks/vangers/data/cube.obj";

//	const Vector3I32 worldSize = {
//		2048,
//		2048,
//		256
//	};

	const Vector3I32& worldSize = settings.worldSize;
	const int32_t hThr = settings.hThr;

	QFileInfo objPathInfo(objPath);
	if(!objPathInfo.exists()){
		qWarning() << "file doens't exists" << objPath;
		return false;
	}

	QString objPathBaseName =  objPathInfo.baseName();

	qDebug() << QTime::currentTime() << "reading OBJ" << objPath ;
	ObjReader reader;
	QFile file(objPath);
	if(!file.open(QFile::ReadOnly)){
		return false;
	}

	ObjectCollection objCol;
	if(!reader.read(objCol, file))
		return false;


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

	QVector<uint8_t> heightsLevels[3];
	for(int i =0; i < 3; i++){
		QVector<uint8_t>& heights = heightsLevels[i];
		heights.resize(worldSize.x * worldSize.y);
		std::fill(heights.begin(), heights.end(), 0);
	}

	QVector<uint8_t> materialLevels[2];
	for(int i =0; i < 2; i++){
		QVector<uint8_t>& materials = materialLevels[i];
		materials.resize(worldSize.x * worldSize.y);
		std::fill(materials.begin(), materials.end(), 0);
	}

	Vec2i worldSize2D = {worldSize.x, worldSize.y};

	QList<QString> materialNames;
	QList<const Object*> objects;
	for(const Object& obj: objCol.objects){
		objects.append(&obj);
	}

	std::sort(objects.begin(), objects.end(), [](const Object* a, const Object* b){
		bool aIsDown = a->name.contains("down");
		bool bIsDown = b->name.contains("down");
		return aIsDown > bIsDown;
	});
	for(const Object* obj: objects){
		qDebug() << obj->name;
		bool isDownObj = obj->name.contains("down");

		for(const Group& group: obj->groups){
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

			qDebug() << group.material << materialIndex;

			for(const Face& face: group.faces){
				assert(face.indices.size() == 3);

				Vec3f verts[3];
				for(int i = 0; i < 3; i++){
					const Vector3F64& v = obj->vertices[face.indices[i].vertexIndex];
					verts[i] = Vec3f::fromVector(v * worldScale);
					verts[i].x -= 1;
					verts[i].y -= 1;
				}

				auto side1 = (verts[1] - verts[0]).normalized();
				auto side2 = (verts[2] - verts[0]).normalized();
				auto cross = side1.cross(side2);

				float upFactor = cross.dot({0, 0, 1.0f});

				bool isUpFace = upFactor >= 0;

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

					uint8_t& hmin = heightsLevels[0][index];
					uint8_t& hmid = heightsLevels[1][index];
					uint8_t& hmax = heightsLevels[2][index];
					uint8_t& matMin = materialLevels[0][index];
					uint8_t& matMax = materialLevels[1][index];

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

//					if(hmin == 0){
//						hmin = height;
//						hmid = height;
//						hmax = height;
//						matMin = (uint8_t)materialIndex;
//						matMax = (uint8_t)materialIndex;
//						return;
//					}

//					if(height < hmin){
//	//					hmid = std::min(hmin, hmid);
//						hmin = height;
//						matMin = (uint8_t)materialIndex;
//					}else if(height < hmid){
//						hmid = height;
//					} else if(height > hmax){
//						if(hmid == hmin){
//							hmid = height;
//						}
//						hmax = height;
//						matMax = (uint8_t)materialIndex;
//					}
				});
			}
		}
	}

	QVector<bool> isShadowed;
	isShadowed.resize(heightsLevels[2].size());
	for(int iLine = 0; iLine < worldSize.y; iLine++){
		uint8_t* hLine = heightsLevels[2].data() + iLine * worldSize.x;
		bool* isShadowedLine = isShadowed.data() + iLine * worldSize.x;

		calculateShadow(hLine, isShadowedLine, worldSize.x, 3.0f/2.0f);
	}

	QDir outputDir(outputDirPath);
	outputDir.mkpath(".");

	for(int i = 0; i < 3; i++){
		QString imgFileName = QString("%1.height.%2.png").arg(objPathBaseName).arg(i);;
		QString imgPath = outputDir.filePath(imgFileName);

		uint8_t min = 255;
		uint8_t max = 0;

//		const auto& heights = heightsLevels[0];
		for(uint8_t h: heightsLevels[i]){
			min = std::min(min, h);
			max = std::max(max, h);
		}
		qDebug() << QTime::currentTime() << min << max << "writing image" << imgPath;
		QImage img(heightsLevels[i].data(),
					worldSize2D.x,
					worldSize2D.y,
					worldSize2D.x,
					QImage::Format_Grayscale8);
		img.save(imgPath);
	}

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

	for(int i = 0; i < 2; i++){
		QString imgFileName = QString("%1.material.%2.png").arg(objPathBaseName).arg(i);;
		QString imgPath = outputDir.filePath(imgFileName);

		uint8_t min = 255;
		uint8_t max = 0;
		auto& materials = materialLevels[1];
		for(uint8_t m: materials){
			min = std::min(min, m);
			max = std::max(max, m);
		}
		qDebug() << QTime::currentTime() << min << max << "writing image" << imgPath;
		QImage img(materials.data(),
					worldSize2D.x,
					worldSize2D.y,
					worldSize2D.x,
					QImage::Format_Indexed8);

		img.setColorTable(colorTable);
		img.save(imgPath);
	}

	{
		QVector<uint32_t> height3;
		height3.resize(heightsLevels[0].size());

		for(int i = 0; i < height3.size(); i++){
			uint8_t hmin = heightsLevels[0][i];
			uint8_t hmid = heightsLevels[1][i];
			uint8_t hmax = heightsLevels[2][i];
			QRgb col = qRgb(hmin, hmax, hmid - hmin);
//			QRgb col = qRgb(hmax, hmax, 0);
			height3[i] = col;
		}

		QString imgFileName = QString("%1.height3.png").arg(objPathBaseName);
		QString imgPath = outputDir.filePath(imgFileName);

		qDebug() << QTime::currentTime() << "writing image" << imgPath;
		QImage img((uint8_t*)(height3.data()),
					worldSize2D.x,
					worldSize2D.y,
					worldSize2D.x * sizeof(uint32_t),
					QImage::Format_RGB32);
		img.save(imgPath);
	}
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

			uint8_t hmin = heightsLevels[0][i];
			uint8_t hmid = heightsLevels[1][i];
			uint8_t hmax = heightsLevels[2][i];
			uint8_t matMin = materialLevels[0][i];
			uint8_t matMax = materialLevels[1][i];

			bool shadowFlag = isShadowed[i];
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

		QString vmpFileName = QString("%1.vmp").arg(objPathBaseName);
		QString vmpPath = outputDir.filePath(vmpFileName);

		qDebug() << QTime::currentTime() << "writing vmp" << vmpPath;
		QFile vmpFile(vmpPath);
		vmpFile.open(QFile::WriteOnly);

		VmapVmpWriter writer;
		writer.write(vmap, vmpFile);
		vmpFile.close();
	}


	return true;
}



