#ifndef RESOURCEACCESS_H
#define RESOURCEACCESS_H

#include <QFile>
#include <QIODevice>


namespace vangers {

template <typename TResource>
class AbstractResourceReader {
public:
	virtual bool read(TResource& resource, QIODevice& device) = 0;
	virtual ~AbstractResourceReader() {}

	bool readFromFile(TResource &resource, const QString& filename){
		QFile device(filename);
		device.open(QFile::ReadOnly);
		bool res = read(resource, (QIODevice&)device);
		device.close();
		return res;
	}
};

template <typename TResource>
class AbstractResourceWriter {
public:
	virtual void write(const TResource &resource, QIODevice &device) = 0;

	void writeToFile(const TResource &resource, const QString& filename){
		QFile device(filename);
		device.open(QFile::WriteOnly);
		write(resource, (QIODevice&)device);
		device.close();
	}

	virtual ~AbstractResourceWriter() {}
};

template <typename TResource>
class AbstractResourceAccess:
		public AbstractResourceReader<TResource>,
		public AbstractResourceWriter<TResource> {
public:
	virtual ~AbstractResourceAccess() {}
};


}


#endif // RESOURCEACCESS_H
