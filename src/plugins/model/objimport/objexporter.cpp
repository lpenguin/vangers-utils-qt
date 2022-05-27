#include "objexporter.h"
#include <plugins/model/m3d/colortable.h>
#include <image/palette.h>
#include <core/vector/vector.h>

#include <QColor>
#include <QDebug>

using namespace vangers::model::m3d;
using namespace vangers::model::objimport;
using namespace vangers::core::vector;

void exportC3D(Object& res, const C3D& c3d){
	for(const Vertex& v: c3d.vectices){
		res.vertices.append(Vector3F64::fromVector(v.tf));
	}

	for(const Normal& n: c3d.normals){		
		Vector3F64 normal = Vector3F64::fromVector(n.normal).normalized();
		if(qIsNaN(normal.x)){
			qDebug() << "nan!";
			normal = {};
		}
		res.normals.append(normal);
	}

	for(uint32_t colorId = 0; colorId < (uint32_t)ColorId::MAX_VALUE; colorId++){
		QString material = ColorNames[colorId];
		Group group {
			.name = material,
			.material = material
		};

		for(const Polygon& p: c3d.polygons){
			if(p.colorId != colorId) continue;

			group.faces.append(Face{});
			Face& face = group.faces.last();
			for(const PolygonIndex& pi: p.indices){
				face.indices.prepend({
					.vertexIndex = pi.vertInd,
					.normalIndex = pi.normInd,
				});
			}
		}
		if(group.faces.size() > 0){
			res.groups.append(group);
		}
	}
}

void ObjExporter::export_(const M3D& m3d,
		ObjectCollection& objCol,
		ObjImportSettings& settings)
{
	objCol = {};
	objCol.objects.append(Object{.name = "Body"});
	Object& body = objCol.objects.last();
	exportC3D(body, m3d.body);
	settings.objectMapping["Body"] = ObjectType::Body;

	objCol.objects.append(Object{.name = "Bound"});
	Object& bound = objCol.objects.last();
	exportC3D(bound, m3d.bound);
	settings.objectMapping["Bound"] = ObjectType::BodyBound;

	for(int i = 0; i < m3d.debris.size(); i++){
		const C3D& debree = m3d.debris[i];

		QString name = QString("Debree_%1").arg(i + 1);
		objCol.objects.append(Object{.name = name});
		Object& debreeObject = objCol.objects.last();
		exportC3D(debreeObject, debree);
		settings.objectMapping[name] = ObjectType::Debree;
	}

	for(int i = 0; i < m3d.boundDebris.size(); i++){
		const C3D& debreeBound = m3d.boundDebris[i];

		QString name = QString("DebreeBound_%1").arg(i + 1);

		objCol.objects.append(Object{.name = name});
		Object& debreeBoundObject = objCol.objects.last();
		exportC3D(debreeBoundObject, debreeBound);
		settings.objectMapping[name] = ObjectType::DebreeBound;
	}

	for(int i = 0; i < m3d.wheels.size(); i++){
		const Wheel& wheel = m3d.wheels[i];
		if(wheel.steer == 1){
			QString name = QString("Wheel_%1").arg(i + 1);
			objCol.objects.append(Object{.name = name});
			Object& wheelObject = objCol.objects.last();

			exportC3D(wheelObject, wheel.model[0]);
			wheelObject.move(wheel.f);
			settings.objectMapping[name] = ObjectType::Wheel;
		} else {
			QString name = QString("WheelStatic_%1").arg(i + 1);
			objCol.objects.append(Object{.name = name});
			Object& wheelObject = objCol.objects.last();
			Object::makeCube(wheelObject, {16, 16, 16}, wheel.f);
			settings.objectMapping[name] = ObjectType::WheelStatic;
		}
	}

	for(int i = 0; i < 3; i++){
		if(!((1 << i) & m3d.slotsExistence)){
			continue;
		}
		const Slot& slot = m3d.rSlots[i];
		QString name = QString("Slot_%1").arg(i + 1);
		objCol.objects.append(Object{.name = name});
		Object& slotObject = objCol.objects.last();

		Object::makeCube(slotObject, {4, 4, 4}, Vector3F64::fromVector(slot.rSlots));
		settings.objectMapping[name] = i == 0
									   ? ObjectType::Slot1
									   : i == 1
										 ? ObjectType::Slot2
										 : ObjectType::Slot3;
	}

	vangers::Palette pal = vangers::Palette::read("objects.pal");
	QMap<int32_t, QString> colorNames;
	for(int32_t colorId = 0; colorId < (int32_t)ColorId::MAX_VALUE; colorId++){
		uint8_t colorIndex = get_color_index(colorId);
		QString materialName = QString(ColorNames[colorId]);
		colorNames[colorId] = materialName;


		QColor c = pal[colorIndex];
		objCol.materials.append({
									   .name = materialName,
									   .ambientColor = Vector3F32{(float)c.redF(), (float)c.greenF(), (float)c.blueF()},
									   .diffuseColor = Vector3F32{(float)c.redF(), (float)c.greenF(), (float)c.blueF()},
									   .specularColor = {0, 0, 0},
								   });
		settings.materialsMapping[materialName] = (ColorId)colorId;
	}

	settings.customScale = 1.0;
	settings.customVolume = m3d.body.volume;
	settings.customRcm = m3d.body.rcm;
	settings.customJ = m3d.body.J;
}
