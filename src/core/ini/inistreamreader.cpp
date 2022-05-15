#include "inistreamreader.h"
#include <QRegularExpression>

using namespace vangers::core::ini;

const QRegularExpression SECTION_REGEXP(R"(\[([^\]]+)\])");
const QRegularExpression PROPERTY_REGEXP(R"(([^\=]+)\=([^\=]+))");

IniStreamReader::IniStreamReader(QTextStream& textStream)
	: _lines()
	,  _currentLine(-1)
{
	QString s;
	while(!(s = textStream.readLine()).isNull()){
		_lines.append(s);
	}
}

TokenType IniStreamReader::readNext()
{
	_currentLine += 1;

	if(_currentLine >= _lines.size()){
		return TokenType::End;
	}

	QString line = _lines[_currentLine];
	QRegularExpressionMatch match;

	if((match = SECTION_REGEXP.match(line)).hasMatch()){
		return TokenType::Section;
	} else if((match = PROPERTY_REGEXP.match(line)).hasMatch()){
		return TokenType::Property;
	}
	return TokenType::Unknown;
}

bool IniStreamReader::readSection(QString& section)
{
	if(_currentLine < 0 || _currentLine >= _lines.size()){
		return false;
	}

	QString line = _lines[_currentLine];
	QRegularExpressionMatch match = SECTION_REGEXP.match(line);

	if(!match.hasMatch()){
		return false;
	}

	section = match.captured(1).trimmed();
	return true;
}

bool IniStreamReader::readProperty(QString& name, QString& value)
{
	if(_currentLine < 0 || _currentLine >= _lines.size()){
		return false;
	}

	QString line = _lines[_currentLine];
	QRegularExpressionMatch match = PROPERTY_REGEXP.match(line);

	if(!match.hasMatch()){
		return false;
	}

	name = match.captured(1).trimmed();
	value = match.captured(2).trimmed();
	return true;
}
