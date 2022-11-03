#include "vectorstringext.h"

namespace vangers::core::vector {
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

	QString toString(bool v){
		return v ? "true" : "false";
	}

	bool fromString(const QString& s, bool& d){
		if(s.toLower() == "true"){
			d = true;
		} else if(s.toLower() == "false") {
			d = false;
		} else {
			return false;
		}
		return true;
	}

	QString toString(int32_t v){
		return QString::number(v);
	}

	bool fromString(const QString& s, int32_t& d){
		bool isOk = false;
		d = s.toInt(&isOk);
		return isOk;;
	}

}

