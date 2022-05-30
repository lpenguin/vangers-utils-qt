#ifndef BINARYREADER_H
#define BINARYREADER_H

#include <QIODevice>
#include <QDebug>
#include <vector>

namespace vangers {

class BinaryIO{
public:
    BinaryIO(QIODevice* device)
        :_device(device)
    {}

protected:
    QIODevice* _device;
};

class BinaryReader: BinaryIO
{
public:
    BinaryReader(QIODevice* device)
        :BinaryIO(device)
    {}

    QByteArray read(quint32 size){
        return _device->read(size);
    }

	template<typename T>
	T read() {
		T value;

		int size = _device->read((char*)(&value), sizeof (T));
		if(size != sizeof (T)){
			qWarning() << "invalid read size" << size << sizeof (T);
		}
		return value;
	}

	template<typename T>
	bool tryRead(T& value) {
		int size = _device->read((char*)(&value), sizeof (T));
		if(size != sizeof (T)){
			return false;
		}
		return true;
	}

	template<typename TContainer>
	bool tryReadArray(TContainer& value, int32_t numValues) {
		value.resize(numValues);

		for(int i = 0; i < numValues; i++){
			if(!tryRead<typename TContainer::value_type>(value[i])){
				return false;
			}
		}
		return true;
	}
};

class BinaryWriter: BinaryIO
{
public:
    BinaryWriter(QIODevice* device)
        :BinaryIO(device)
    {}

    template<typename T>
    void write(const T& value) {
       _device->write((char*)(&value), sizeof (T));
    }

    void write(const char* data, qsizetype len){
        _device->write(data, len);
    }

    void write(const uchar* data, qsizetype len){
        _device->write((char*)data, len);
    }

    void write(const QByteArray& value){
        _device->write(value.constData(), value.size());
    }

	template<typename TContainer>
	bool writeArray(const TContainer& value) {
		for(int i = 0; i < value.size(); i++){
			write<typename TContainer::value_type>(value[i]);
		}
	}
};

}


#endif // BINARYREADER_H
