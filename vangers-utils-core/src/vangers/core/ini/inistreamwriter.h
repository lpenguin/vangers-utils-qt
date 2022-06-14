#ifndef INISTREAMWRITER_H
#define INISTREAMWRITER_H

#include <QTextStream>


namespace vangers::core::ini {
	class IniStreamWriter
	{
	public:
		IniStreamWriter(QTextStream* textStream);
		void writeBeginSection(QString section);
//		void writeProperty(QString name, QString type, const QVariant& value);
		void writeProperty(QString name, const QVariant& value);
	private:
		QTextStream* _textStream;
		bool _first;
	};
}


#endif // INISTREAMWRITER_H
