#include "heightmapfrommesh.h"
#include <plugins/model/obj/objreader.h>
#include <QFile>
#include <QDebug>
#include <iterator> // back_inserter
#include <vector>   // vector
#include <iostream> // clog
#include <float.h>
#include <functional>
#include <QTime>
#include <QImage>

using namespace vangers::model::obj;
using namespace vangers::core::vector;
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

bool vangers::heightmapFromMesh(QString objPath)
{
//	objPath ="/home/nikita/Sources/notebooks/vangers/data/cube.obj";

//	const Vector3I32 worldSize = {
//		2048,
//		2048,
//		256
//	};

	const uint8_t hThr = 10;

	const Vector3I32 worldSize = {
		2048,
		2048 * 8,
		256
	};

	qDebug() << QTime::currentTime() << "reading OBJ" << objPath ;
	ObjReader reader;
	QFile file(objPath);
	if(!file.open(QFile::ReadOnly)){
		return false;
	}

	ObjectCollection objCol;
	if(!reader.read(objCol, file))
		return false;

	Object mergedObj;
	QList<const Object*> toMerge;
	for(const Object& obj: objCol.objects){
		toMerge.append(&obj);
	}

	qDebug() << QTime::currentTime() << "merging OBJ";
	Object::merge(mergedObj, toMerge);

//	for(Vector3F64& vert: mergedObj.vertices){
//		if(vert.z < 0){
//			vert.z = 0;
//		}
//	}
//	mergedObj.triangulate();
	auto bounds = mergedObj.bounds();	
	mergedObj.move(bounds.min * -1.0);
	bounds = mergedObj.bounds();

	Vector3F64 objSize = bounds.max;
	Vector3F64 worldScale = Vector3F64::fromVector(worldSize - Vector3I32{1, 1, 1}) / objSize;

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

	for(const Group& group: mergedObj.groups){
		int32_t materialIndex = materialNames.indexOf(group.material);
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
				const Vector3F64& v = mergedObj.vertices[face.indices[i].vertexIndex];
				verts[i] = Vec3f::fromVector(v * worldScale);
			}
			triangle(verts, worldSize2D, [&](const Vec2i& screenPos, const Vec3f& worldPos){
				float heightF = worldPos.z;

				if(heightF > 255){
//					qDebug() << "invalid height" << heightF;
					heightF = 255;
				}

				uint8_t height = (uint8_t)heightF;

				if(height <= hThr) {
					return;
				}

				int32_t index = screenPos.x + worldSize2D.x * screenPos.y;

				uint8_t& hmin = heightsLevels[0][index];
				uint8_t& hmid = heightsLevels[1][index];
				uint8_t& hmax = heightsLevels[2][index];
				uint8_t& matMin = materialLevels[0][index];
				uint8_t& matMax = materialLevels[1][index];
				if(hmin == 0){
					hmin = height;
					hmid = height;
					hmax = height;
					matMin = (uint8_t)materialIndex;
					matMax = (uint8_t)materialIndex;
					return;
				}

				if(height < hmin){
//					hmid = std::min(hmin, hmid);
					hmin = height;
					matMin = (uint8_t)materialIndex;
				}else if(height < hmid){
					hmid = height;
				} else if(height > hmax){
					if(hmid == hmin){
						hmid = height;
					}
					hmax = height;
					matMax = (uint8_t)materialIndex;
				}
			});
		}
	}

	for(int i = 0; i < 3; i++){
		QString imgPath = QString("%1.height.%2.png").arg(objPath).arg(i);

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
		QString imgPath = QString("%1.material.%2.png").arg(objPath).arg(i);

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
//			uint8_t* h3 = (uint8_t*)(&height3[i]);
			uint8_t hmin = heightsLevels[0][i];
			uint8_t hmid = heightsLevels[1][i];
			uint8_t hmax = heightsLevels[2][i];
			QRgb col = qRgb(hmin, hmax, hmid - hmin);
//			QRgb col = qRgb(hmax, hmax, 0);
			height3[i] = col;
//			h3[0] = 0xFF;
//			h3[3] = heightsLevels[0][i];
//			h3[2] = heightsLevels[2][i];
//			h3[1] = heightsLevels[1][i] - heightsLevels[0][i];

		}
		QString imgPath = QString("%1.heights3.png").arg(objPath);

		qDebug() << QTime::currentTime() << "writing image" << imgPath;
		QImage img((uint8_t*)(height3.data()),
					worldSize2D.x,
					worldSize2D.y,
					worldSize2D.x * sizeof(uint32_t),
					QImage::Format_RGB32);
		img.save(imgPath);
	}


	return true;
}



