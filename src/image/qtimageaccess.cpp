#include "qtimageaccess.h"

#include <QFileInfo>
QSharedPointer<vangers::Image> vangers::QtImageAccess::read(QIODevice &device)
{
    try {
        QFile& file = dynamic_cast<QFile&>(device);

        QFileInfo fInfo(file);
        if(!fInfo.isFile() || !fInfo.exists()){
            return QSharedPointer<vangers::Image>();
        }

        QString metaFilename = file.fileName()+".meta.txt";
        QFileInfo metaFInfo(metaFilename);
        if(!metaFInfo.isFile() || !metaFInfo.exists()){
            return QSharedPointer<vangers::Image>();
        }


        QFile metaFile(metaFilename);
        metaFile.open(QFile::ReadOnly);


        auto meta = _metaAccess.read(metaFile);
        auto image = QSharedPointer<QImage>::create();
        image->load(&file, _format.toLocal8Bit().data());
        // TODO: check for png8
        return QSharedPointer<vangers::Image>::create(image, meta);
    }  catch (std::bad_cast) {
        return QSharedPointer<vangers::Image>();
    }

}

void vangers::QtImageAccess::write(const QSharedPointer<vangers::Image> &image, QIODevice &device)
{
    try {
        QFile& file = dynamic_cast<QFile&>(device);
        QString metaFilename = file.fileName()+".meta.txt";
        QFile metaFile(metaFilename);
        metaFile.open(QFile::WriteOnly);
        _metaAccess.write(image->meta(), metaFile);

        image->image()->save(&file, _format.toLocal8Bit().data());
    }  catch (std::bad_cast) {

    }

}
