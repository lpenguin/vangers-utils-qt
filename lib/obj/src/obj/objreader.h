#ifndef OBJREADER_H
#define OBJREADER_H

#include <QTextStream>
#include <vangers/core/error/error.h>

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
		vangers::core::error::Result readObject(QString& name);
		vangers::core::error::Result readGroup(QString& name);
		vangers::core::error::Result readUseMaterial(QString& name);
		vangers::core::error::Result readUseMaterialLib(QString& name);
		vangers::core::error::Result readVertex(Vector3F64& v);
		vangers::core::error::Result readNormal(Vector3F64& v);
		vangers::core::error::Result readFace(Face& face);
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
		vangers::core::error::Result read(ObjectCollection& objCol, QIODevice& device);
	};



}


#endif // OBJREADER_H
