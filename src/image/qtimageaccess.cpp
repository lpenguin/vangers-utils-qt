#include "qtimageaccess.h"

#include <QFileInfo>
bool vangers::QtImageAccess::read(Image& image, QIODevice &device)
{
    try {
        QFile& file = dynamic_cast<QFile&>(device);

        QFileInfo fInfo(file);
        if(!fInfo.isFile() || !fInfo.exists()){
			return false;
        }

        QString metaFilename = file.fileName()+".meta.txt";
        QFileInfo metaFInfo(metaFilename);
        if(!metaFInfo.isFile() || !metaFInfo.exists()){
			return false;
        }


        QFile metaFile(metaFilename);
        metaFile.open(QFile::ReadOnly);


		ImageMeta meta;
		_metaAccess.read(meta, metaFile);
		auto img = QSharedPointer<QImage>::create();
		img->load(&file, _format.toLocal8Bit().data());
		image.setImage(img);
		image.setMeta(meta);
        // TODO: check for png8
		return true;
    }  catch (std::bad_cast) {
		return false;
    }

}

void vangers::QtImageAccess::write(const vangers::Image &image, QIODevice &device)
{
    try {
        QFile& file = dynamic_cast<QFile&>(device);
        QString metaFilename = file.fileName()+".meta.txt";
        QFile metaFile(metaFilename);
        metaFile.open(QFile::WriteOnly);
		_metaAccess.write(image.meta(), metaFile);

		image.image()->save(&file, _format.toLocal8Bit().data());
    }  catch (std::bad_cast) {

    }

}
