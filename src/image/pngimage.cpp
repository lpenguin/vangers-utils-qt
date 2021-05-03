#include "pngimage.h"

#include <QFileInfo>


QSharedPointer<vangers::Image> vangers::PngImageAccess::read(QFile &file)
{
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
    image->load(&file, "png");
    // TODO: check for png8
    return QSharedPointer<vangers::Image>::create(image, meta);
}

void vangers::PngImageAccess::write(const QSharedPointer<vangers::Image> &image, QFile &file)
{
    QString metaFilename = file.fileName()+".meta.txt";
    QFile metaFile(metaFilename);
    metaFile.open(QFile::WriteOnly);
    _metaAccess.write(image->meta(), metaFile);

    image->image()->save(&file, "png");
}
