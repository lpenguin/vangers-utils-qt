#ifndef OBJREADER_H
#define OBJREADER_H

#include <QTextStream>

#include "obj.h"

namespace obj {

	enum class LineType{
		StartObject,
		StartGroup,
		Face,
		Vertex,
		Normal,
		TextureCoord,
		UseMaterialLib,
		UseMaterial,
		Unknown,
		End
	};

	class ObjStreamReader{
	public:
		ObjStreamReader(QTextStream& stream);
		LineType readNext();
		bool readObject(QString& name);
		bool readGroup(QString& name);
		bool readUseMaterial(QString& name);
		bool readUseMaterialLib(QString& name);
		bool readVertex(Vector3F64& v);
		bool readNormal(Vector3F64& v);
		bool readFace(Face& face);
	private:
		QStringList _contents;
		int32_t _currentLine;

		static const QRegExp REGEXP_START_OBJECT;
		static const QRegExp REGEXP_START_GROUP;
		static const QRegExp REGEXP_FACE;
		static const QRegExp REGEXP_FACE_INDEX;
		static const QRegExp REGEXP_VERTEX;
		static const QRegExp REGEXP_NORMAL;
		static const QRegExp REGEXP_TEXTURE_COORD;
		static const QRegExp REGEXP_USE_MATERIAL_LIB;
		static const QRegExp REGEXP_USE_MATERIAL;
	};

	class ObjReader
	{
	public:
		bool read(ObjectCollection& objCol, QIODevice& device);
	};



}


#endif // OBJREADER_H
