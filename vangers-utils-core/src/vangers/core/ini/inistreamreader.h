#ifndef INISTREAMREADER_H
#define INISTREAMREADER_H

#include <QTextStream>



namespace vangers::core::ini {
	enum class TokenType {
		Section,
		Property,
//			TypedProperty,
		Unknown,
		End
	};

	class IniStreamReader
	{
	public:
		IniStreamReader(QTextStream& textStream);
		TokenType readNext();
		bool readSection(QString& section);
		bool readProperty(QString& name, QString& value);

	private:
		QStringList _lines;
		int _currentLine;
	};
}


#endif // INISTREAMREADER_H
//template<typename T>
//bool tryConvert(const QString& valueString, T& value){
//	QVariant valueVariant(valueString);

//	if(!valueVariant.canConvert<T>()) return false;

//	value = valueVariant.value<T>();
//	return true;
//}
