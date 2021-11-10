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
    std::vector<T> readArray(quint32 size) {
        std::vector<T> result(size);
        result.resize(size);
        QByteArray bytes = _device->read(size);
        const char* bytesData = bytes.constData();

        std::copy(bytesData, bytesData + bytes.size(), result.data());

        return result;
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
};

}


#endif // BINARYREADER_H
