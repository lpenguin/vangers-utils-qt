#include "inisettings.h"

using namespace vangers::core::ini;

QString IniSettings::value(QString group, QString name) const
{
	if(!_values.contains(group)) return QString();
	if(!_values[group].contains(name)) return QString();

	return _values[group][name];
}

QString IniSettings::value(QString name) const
{
	return value(QString(), name);
}

void IniSettings::setValue(QString group, QString name, QString value)
{
	_values[group][name] = value;
}

void IniSettings::setValue(QString name, QString value)
{
	setValue(QString(), name, value);
}

QStringList IniSettings::groups() const
{
	return _values.keys();
}

QStringList IniSettings::keys(QString group) const
{
	if(!_values.contains(group)) return QStringList();

	return _values[group].keys();
}
