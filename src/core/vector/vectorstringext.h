#ifndef VECTOREXT_H
#define VECTOREXT_H

#include <QRegExp>
#include <QString>
#include <QStringList>
#include <core/vector/vector.h>

namespace vangers::core::vector {
	template<typename T>
	QString toString(const Vector3<T>& v){
		return QString("%1 %2 %3").arg(v.x, 0, 'f').arg(v.y, 0, 'f').arg(v.z, 0, 'f');
	}

	template<typename T>
	bool fromString(const QString& str, Vector3<T>& v){
		QStringList strs = str.split(QRegExp(R"(\s+)"));
		if(strs.size() != 3) return false;
		bool ok = false;

		v.x = (T)strs[0].toDouble(&ok);
		if(!ok) return false;

		v.y = (T)strs[1].toDouble(&ok);
		if(!ok) return false;

		v.z = (T)strs[2].toDouble(&ok);
		if(!ok) return false;

		return true;
	}

	template<typename T>
	QString toString(const Matrix3x3<T>& v){
		QStringList strs;
		for(int i = 0; i < 9; i++){
			strs.append(QString::number(v.values[i], 'f'));
		}
		return strs.join(" ");
	}

	template<typename T>
	bool fromString(const QString& str, Matrix3x3<T>& v){
		QStringList strs = str.split(QRegExp(R"(\s+)"));
		if(strs.size() != 9) return false;
		bool ok = false;
		for(int i = 0; i < 9; i++){
			v.values[i] = (T)strs[i].toDouble(&ok);
			if(!ok) return false;
		}

		return true;
	}

	bool fromString(const QString& s, float& d){
		bool ok;
		d = s.toFloat(&ok);
		return ok;
	}

	bool fromString(const QString& s, double& d){
		bool ok;
		d = s.toDouble(&ok);
		return ok;
	}
}
#endif // VECTOREXT_H
