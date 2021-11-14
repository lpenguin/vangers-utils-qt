#include "vmapviewerplugin.h"
#include "ui_vmapviewer.h"
#include "vmapreader.h"
#include "vmapwriter.h"

#include <QSettings>
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QImage>
#include <QAbstractButton>
#include <graphicsviewzoom.h>

#include <splay/splay.h>
#include "../../image/palette.h"
#include "layer/terrainimagelayer.h"
using namespace vangers;

const ResourceType VmapViewerPlugin::VmcType = {
    .name = "Vangers level",
    .extensions = {"*.ini"}
};

const ResourceType VmapViewerPlugin::LevelType = {
    .name = "Uncompressed level directory",
    .extensions = {"*.ini"}
};

VmapViewer::VmapViewer(VmapViewerPlugin *plugin, QWidget *parent)
    : ResourceViewer(plugin, parent)
    , _ui(new Ui::VmapViewer())
    , _currentFile()
    , _vmap()
    , _heightItem(nullptr)
    , _metaItem(nullptr)
	, _layers()
{
    _ui->setupUi(this);
    auto* gz = new GraphicsViewZoom(_ui->graphicsView);
    gz->set_modifiers(Qt::NoModifier);
    QObject::connect(_ui->heightButton, &QAbstractButton::toggled,
                     this, &VmapViewer::onHeightToggled);

    QObject::connect(_ui->metaButton, &QAbstractButton::toggled,
                     this, &VmapViewer::onMetaToggled);


	_ui->maskCombo->clear();
	_ui->maskCombo->addItems({
								 "All",
								 "Delta",
								 "Terrain",
								 "DoubleLevel",
								 "Shadow",
							 });

	_layers = {
		{"Terrain", new TerrainImageLayer(this)}
	};

	QObject::connect(_ui->maskCombo, SIGNAL(currentIndexChanged(int)),
					 this, SLOT(onMaskTypeChanged(int)));

}

VmapViewer::~VmapViewer()
{

}

bool VmapViewer::importResource(const QString &filename, const ResourceType &resourceType)
{
    _currentFile = filename;

	_vmap = QSharedPointer<Vmap>::create();
	VmapReader reader;
	QFile file(filename);
	file.open(QFile::ReadOnly);
	if(!reader.read(*_vmap, file)){
		qDebug() << "Cannot read file" << filename;
		file.close();
		return false;
	}
	file.close();

	auto* old = _ui->graphicsView->scene();
	if(old != nullptr){
		old->clear();
		delete old;
	}
//    _ui->graphicsView
    QGraphicsScene* scene = new QGraphicsScene(this);
	_ui->graphicsView->setScene(scene);
    {
		const uchar* buf = _vmap->heightConst().data();

		int sizeX = _vmap->size().width();
		int sizeY = _vmap->size().height();
        QImage image(buf, sizeX, sizeY, sizeX, QImage::Format_Indexed8);

        image.setColorTable(vangers::Palette::grayscale());

        QPixmap pixmap = QPixmap::fromImage(image);
        _heightItem = scene->addPixmap(pixmap);
        scene->setSceneRect(image.rect());
    }

    {
		const auto& meta = _vmap->meta();
		uchar* buf = new uchar[meta.size()];
		for(size_t i =0; i < meta.size(); i++){
			buf[i] = meta[i];
		}

        QImage image(buf, sizeX, sizeY, sizeX, QImage::Format_Indexed8);

        image.setColorTable(vangers::Palette::grayscale());


        QPixmap pixmap = QPixmap::fromImage(image);
        _metaItem = scene->addPixmap(pixmap);
        _metaItem->setVisible(false);
    }

//    item->setTransformationMode(Qt::SmoothTransformation);
//    qDebug() << i.rect() << _image->image()->rect() << _image->image();



    _ui->heightButton->setChecked(true);
    _ui->metaButton->setChecked(false);

	_ui->maskCombo->setCurrentIndex(0);

    return true;

}

void VmapViewer::exportResource(const QString &filename, const ResourceType &)
{
    if(_vmap.isNull()){
        return;
    }

	VmapWriter writer;
	writer.write(*_vmap, filename);
}

QString VmapViewer::currentFile() const
{
    return _currentFile;
}

void VmapViewer::applyMask(QString layerName)
{
	if(!_layers.contains(layerName)){
		qWarning() << "No layer definition" << layerName;
		return;
	}

	QSharedPointer<QImage> metaImage = _layers[layerName]->getImage(*_vmap);
	QPixmap pixmap = QPixmap::fromImage(*metaImage);
	_metaItem->setPixmap(pixmap);
}

void VmapViewer::onHeightToggled(bool checked)
{
    if(_heightItem == nullptr){
        return;
    }

    _heightItem->setVisible(checked);
}

void VmapViewer::onMetaToggled(bool checked)
{
    if(_metaItem == nullptr){
        return;
    }

	_metaItem->setVisible(checked);
}

void VmapViewer::onMaskTypeChanged(int maskIndex)
{
	applyMask(_ui->maskCombo->itemText(maskIndex));
}


ResourceViewer *VmapViewerPlugin::makeResourceViewer(QWidget *parent)
{
    return new VmapViewer(this, parent);
}