#include "objreader.h"

#include <QSet>
using namespace vangers::model::obj;

const QRegExp ObjStreamReader::REGEXP_START_OBJECT(R"(o\s+(.+))");
const QRegExp ObjStreamReader::REGEXP_START_GROUP(R"(g\s+(.+))");
const QRegExp ObjStreamReader::REGEXP_FACE(R"(f\s+(.*))");
const QRegExp ObjStreamReader::REGEXP_FACE_INDEX(R"((\d*)/(\d*)/(\d*))");
const QRegExp ObjStreamReader::REGEXP_VERTEX(R"(v\s+([\-\d\.e\+]+)\s+([\-\d\.e\+]+)\s+([\-\d\.e\+]+))");
const QRegExp ObjStreamReader::REGEXP_NORMAL(R"(vn\s+([\-\d\.e\+]+)\s+([\-\d\.e\+]+)\s+([\-\d\.e\+]+))");
const QRegExp ObjStreamReader::REGEXP_TEXTURE_COORD(R"(vt\s+([\-\d\.]+)\s+([\-\d\.]+)\s+([\-\d\.]+))");
const QRegExp ObjStreamReader::REGEXP_USE_MATERIAL_LIB(R"(mtllib\s+(.*))");
const QRegExp ObjStreamReader::REGEXP_USE_MATERIAL(R"(usemtl\s+(.*))");

bool assignVertices(ObjectCollection& col, const QList<Vector3F64>& vertices, const QList<Vector3F64>& normals);

bool ObjReader::read(ObjectCollection& objCol, QIODevice& device)
{
	QTextStream stream(&device);
	ObjStreamReader reader(stream);
//	Object obj{};
//	FaceGroup group{};
//	Face face{};
	QList<Vector3F64> vertices;
	QList<Vector3F64> normals;

	LineType type;
	while((type = reader.readNext()) != LineType::End){
		switch(type){
		case LineType::UseMaterialLib:
			{
				QString mtl;
				if(!reader.readUseMaterialLib(mtl)){
					return false;
				}
				objCol.useMaterialLibraries.append(mtl);
			}

			break;

		case LineType::StartObject:
			{
				Object obj = {};
				if(!reader.readObject(obj.name)){
					return false;
				}
				objCol.objects.append(obj);
			}
			break;

		case LineType::Vertex:
			{
				Vector3F64 v;
				if(!reader.readVertex(v)){
					return false;
				}
				vertices.append(v);
			}
			break;

		case LineType::Normal:
			{
				Vector3F64 n;
				if(!reader.readNormal(n)){
					return false;
				}
				normals.append(n);
			}
			break;

		case LineType::StartGroup:
		{
			if(objCol.objects.size() == 0) return false;

			Group group = {};
			if(!reader.readGroup(group.name)){
				return false;
			}
			objCol.objects.last().groups.append(group);
		}
		break;

		case LineType::UseMaterial:
		{
			if(objCol.objects.size() == 0) return false;

			QString material;
			if(!reader.readUseMaterial(material)){
				return false;
			}

			Object& obj = objCol.objects.last();
			if(obj.groups.size() == 0 || !obj.groups.last().material.isNull()){
				obj.groups.append(Group{.name = material});
			}

			Group& group = obj.groups.last();
			group.material = material;

		}
		break;

		case LineType::Face:
		{
			if(objCol.objects.size() == 0) return false;

			Object& obj = objCol.objects.last();

			if(obj.groups.size() == 0){
				obj.groups.append(Group{.name = obj.name});
			}
			Group& group = obj.groups.last();

			Face face = {};
			if(!reader.readFace(face)){
				return false;
			}
			group.faces.append(face);
			break;
		}

		default:
			continue;
		}
	}

	if(!assignVertices(objCol, vertices, normals)){
		return false;
	}
	return true;
}


ObjStreamReader::ObjStreamReader(QTextStream& stream)
{
	while (!stream.atEnd())
	{
	   QString line = stream.readLine();
	   _contents.append(line.trimmed());
	}
	_currentLine = -1;
}

LineType ObjStreamReader::readNext()
{
	_currentLine += 1;
	if(_currentLine >= _contents.size()) return LineType::End;

	const QString& line = _contents[_currentLine];

	// TODO: use regexp
	if(line.startsWith("o ")) return LineType::StartObject;
	if(line.startsWith("g ")) return LineType::StartGroup;
	if(line.startsWith("v ")) return LineType::Vertex;
	if(line.startsWith("vn ")) return LineType::Normal;
	if(line.startsWith("vt ")) return LineType::TextureCoord;
	if(line.startsWith("f ")) return LineType::Face;
	if(line.startsWith("usemtl ")) return LineType::UseMaterial;
	if(line.startsWith("mtllib ")) return LineType::UseMaterialLib;

	return LineType::Unknown;
}

bool ObjStreamReader::readObject(QString& name)
{
	QRegExp r = REGEXP_START_OBJECT;
	if(r.indexIn(_contents[_currentLine]) < 0){
		return false;
	}

	name = r.cap(1);
	return true;
}

bool ObjStreamReader::readGroup(QString& name)
{
	QRegExp r = REGEXP_START_GROUP;
	if(r.indexIn(_contents[_currentLine]) < 0){
		return false;
	}

	name = r.cap(1);
	return true;
}

bool ObjStreamReader::readUseMaterial(QString& name)
{
	QRegExp r = REGEXP_USE_MATERIAL;
	if(r.indexIn(_contents[_currentLine]) < 0){
		return false;
	}

	name = r.cap(1);
	return true;
}

bool ObjStreamReader::readUseMaterialLib(QString& name)
{
	QRegExp r = REGEXP_USE_MATERIAL_LIB;
	if(r.indexIn(_contents[_currentLine]) < 0){
		return false;
	}

	name = r.cap(1);
	return true;
}

bool ObjStreamReader::readVertex(Vector3F64& v)
{
	const QString& line = _contents[_currentLine];
	QRegExp r = REGEXP_VERTEX;
	if(r.indexIn(line) < 0){
		return false;
	}

	int cc = r.captureCount();

	bool ok = true;

	v.x = r.cap(1).toDouble(&ok);
	if(!ok)
		return false;

	v.y = r.cap(2).toDouble(&ok);
	if(!ok)
		return false;

	v.z = r.cap(3).toDouble(&ok);
	if(!ok)
		return false;

	return true;
}

bool ObjStreamReader::readNormal(Vector3F64& v)
{
	QRegExp r = REGEXP_NORMAL;
	if(r.indexIn(_contents[_currentLine]) < 0){
		return false;
	}

	int cc = r.captureCount();

	bool ok = true;

	v.x = r.cap(1).toDouble(&ok);
	if(!ok) return false;

	v.y = r.cap(2).toDouble(&ok);
	if(!ok) return false;

	v.z = r.cap(3).toDouble(&ok);
	if(!ok) return false;

	return true;
}

bool ObjStreamReader::readFace(Face& face)
{
	face.indices.clear();

	QRegExp r = REGEXP_FACE;
	if(r.indexIn(_contents[_currentLine]) < 0){
		return false;
	}

	QString faceStr = r.cap(1);
	QStringList faceStrs = faceStr.split(QRegExp(R"(\s+)"));
	for(const QString& faceIndStr: faceStrs){
		QRegExp rf = REGEXP_FACE_INDEX;
		if(rf.indexIn(faceIndStr) < 0) return false;

		FaceIndex index {};

		if(!rf.cap(1).isEmpty()){
			index.vertexIndex = rf.cap(1).toInt() - 1;
		}

		if(!rf.cap(2).isEmpty()){
			index.textureCoordIndex = rf.cap(2).toInt() - 1;
		}

		if(!rf.cap(3).isEmpty()){
			index.normalIndex = rf.cap(3).toInt() - 1;
		}
		face.indices.append(index);
	}

	return true;
}

bool assignVertices(ObjectCollection& col, const QList<Vector3F64>& vertices, const QList<Vector3F64>& normals){
	for(Object& obj: col.objects){
		if(obj.groups.size() == 0) continue;

		QVector<int32_t> verticesSubset;
		QVector<int32_t> normalsSubset;

		for(const Group& g: obj.groups){
			for(const Face& f: g.faces){
				for(const FaceIndex& fi: f.indices){
					verticesSubset.append(fi.vertexIndex);
					normalsSubset.append(fi.normalIndex);
				}
			}
		}

		std::sort(verticesSubset.begin(), verticesSubset.end());
		std::sort(normalsSubset.begin(), normalsSubset.end());

		QMap<int32_t, int32_t> vertexIndexMap;
		QMap<int32_t, int32_t> normalIndexMap;
		vertexIndexMap[verticesSubset[0]] = 0;
		obj.vertices.append(vertices[verticesSubset[0]]);
		for(int i = 1; i < verticesSubset.size(); i++){
			if(verticesSubset[i] != verticesSubset[i - 1]){
				vertexIndexMap[verticesSubset[i]] = vertexIndexMap.size();
				obj.vertices.append(vertices[verticesSubset[i]]);
			}
		}

		normalIndexMap[normalsSubset[0]] = 0;
		obj.normals.append(normals[normalsSubset[0]]);
		for(int i = 1; i < normalsSubset.size(); i++){
			if(normalsSubset[i] != normalsSubset[i - 1]){
				normalIndexMap[normalsSubset[i]] = normalIndexMap.size();
				obj.normals.append(normals[normalsSubset[i]]);
			}
		}

		for(Group& g: obj.groups){
			for(Face& f: g.faces){
				for(FaceIndex& fi: f.indices){
					fi.vertexIndex = vertexIndexMap[fi.vertexIndex];
					fi.normalIndex = normalIndexMap[fi.normalIndex];
				}
			}
		}
	}

	return true;
}

