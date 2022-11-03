#include "inistreamwriter.h"

using namespace vangers::core::ini;

IniStreamWriter::IniStreamWriter(QTextStream* textStream)
	: _textStream(textStream)
	, _first(true)
{

}

void IniStreamWriter::writeBeginSection(QString section)
{
	if(!_first) {
		*_textStream << "\n";
	} else {
		_first = false;
	}

	*_textStream << QString("[%1]\n").arg(section);
}

//void IniStreamWriter::writeProperty(QString name, QString type, const QVariant& value)
//{
//	*_textStream << QString("%1:%2 = %3\n").arg(name, type, value.toString());
//}

void IniStreamWriter::writeProperty(QString name, const QVariant& value)
{
	*_textStream << QString("%1 = %2\n").arg(name, value.toString());
}
