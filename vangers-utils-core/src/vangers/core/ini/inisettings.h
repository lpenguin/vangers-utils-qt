#ifndef INISETTINGS_H
#define INISETTINGS_H

#include <QMap>
#include <QVariant>


namespace vangers::core::ini {
	class IniSettings
	{
	public:
		QString value(QString group, QString name) const;
		QString value(QString name) const;
		void setValue(QString group, QString name, QString value);
		void setValue(QString name, QString value);

		QStringList groups() const;
		QStringList keys(QString group) const;
	private:
		QMap<QString, QMap<QString, QString>> _values;
	};
}


#endif // INISETTINGS_H
