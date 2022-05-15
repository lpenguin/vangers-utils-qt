#include "iniaccess.h"

#include <core/ini/inistreamreader.h>
#include <core/ini/inistreamwriter.h>

using namespace vangers::core::ini;

void IniAccess::write(const IniSettings& settings, QIODevice& device)
{
	QTextStream stream(&device);
	IniStreamWriter writer(&stream);

	for(const QString& group: settings.groups()){
		if(!group.isNull()) {
			writer.writeBeginSection(group);
		}

		for(const QString& key: settings.keys(group)){
			writer.writeProperty(key, settings.value(group, key));
		}
	}
}

bool IniAccess::read(IniSettings& settings, QIODevice& device)
{
	QTextStream stream(&device);
	IniStreamReader reader(stream);

	TokenType type;

	QString group;
	while((type = reader.readNext()) != TokenType::End){
		switch (type) {
		case TokenType::Section: {
				if(!reader.readSection(group)) return false;
			}
			break;
		case TokenType::Property: {
				QString value;
				QString name;

				if(!reader.readProperty(name, value)) return false;
				settings.setValue(group, name, value);
			}
			break;
		default:
			continue;
		}
	}

	return true;
}
