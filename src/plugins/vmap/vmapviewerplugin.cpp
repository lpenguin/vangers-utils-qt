#include "vmapviewerplugin.h"
#include "ui_vmapviewer.h"
#include "vmapaccess.h"

#include <QSettings>
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QImage>
#include <QAbstractButton>
#include <graphicsviewzoom.h>

#include <splay/splay.h>
#include "../../image/palette.h"

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
    , _mask(255)
{
    _ui->setupUi(this);
    auto* gz = new GraphicsViewZoom(_ui->graphicsView);
    gz->set_modifiers(Qt::NoModifier);
    QObject::connect(_ui->heightButton, &QAbstractButton::toggled,
                     this, &VmapViewer::onHeightToggled);

    QObject::connect(_ui->metaButton, &QAbstractButton::toggled,
                     this, &VmapViewer::onMetaToggled);


    QObject::connect(_ui->deltaCheckBox, &QCheckBox::toggled,
                     this, &VmapViewer::onDeltaMaskToggled);

    QObject::connect(_ui->objShadowCheckBox, &QCheckBox::toggled,
                     this, &VmapViewer::onObjShadowMaskToggled);

    QObject::connect(_ui->terrainCheckBox, &QCheckBox::toggled,
                     this, &VmapViewer::onTerrainTypeMaskToggled);

    QObject::connect(_ui->doubleLevelCheckBox, &QCheckBox::toggled,
                     this, &VmapViewer::onDoubleLevelMaskToggled);

    QObject::connect(_ui->shadowCheckBox, &QCheckBox::toggled,
                     this, &VmapViewer::onShadowMaskToggled);
}

VmapViewer::~VmapViewer()
{

}

bool VmapViewer::importResource(const QString &filename, const ResourceType &resourceType)
{
    _currentFile = filename;
    QSettings settings(filename, QSettings::IniFormat);
    int mapPowerX = settings.value("Global Parameters/Map Power X").toInt();
    int mapPowerY = settings.value("Global Parameters/Map Power Y").toInt();

    int sizeX = 1 << mapPowerX;
    int sizeY = 1 << mapPowerY;

    qDebug() << "sizeX, sizeY" << sizeX << sizeY;

    QString levelBaseName = settings.value("Storage/File Name").toString();

    QFileInfo fileInfo(filename);

    QString vmcFileName = QDir(fileInfo.absoluteDir()).filePath(levelBaseName + ".vmc");
    QFileInfo vmcFileInfo = vmcFileName;

    if(!vmcFileInfo.exists()){
        qWarning() << "VMC file doesn't exsit" << vmcFileName;
        return false;
    }

    QFile vmcFile(vmcFileName);
    if(!vmcFile.open(QFile::ReadOnly)){
        qWarning() << "Cannot open file for reading" << vmcFileName;
        return false;
    }

    VmapAccess access(sizeX, sizeY);
    _vmap = access.read(vmcFile);

//    _ui->graphicsView
    QGraphicsScene* scene = new QGraphicsScene(this);
    {
        uchar* buf = _vmap->height().data();

        QImage image(buf, sizeX, sizeY, sizeX, QImage::Format_Indexed8);

        image.setColorTable(vangers::Palette::grayscale());


        QPixmap pixmap = QPixmap::fromImage(image);
        _heightItem = scene->addPixmap(pixmap);
        scene->setSceneRect(image.rect());
    }

    {
        uchar* buf = _vmap->meta().data();

        QImage image(buf, sizeX, sizeY, sizeX, QImage::Format_Indexed8);

        image.setColorTable(vangers::Palette::grayscale());


        QPixmap pixmap = QPixmap::fromImage(image);
        _metaItem = scene->addPixmap(pixmap);
        _metaItem->setVisible(false);
    }

//    item->setTransformationMode(Qt::SmoothTransformation);
//    qDebug() << i.rect() << _image->image()->rect() << _image->image();

    _ui->graphicsView->setScene(scene);

    _ui->heightButton->setChecked(true);
    _ui->metaButton->setChecked(false);

    _ui->deltaCheckBox->setChecked(true);
    _ui->objShadowCheckBox->setChecked(true);
    _ui->terrainCheckBox->setChecked(true);
    _ui->doubleLevelCheckBox->setChecked(true);
    _ui->shadowCheckBox->setChecked(true);

    _mask = 0b11111111;
    return true;

}

void VmapViewer::exportResource(const QString &filename, const ResourceType &)
{
    if(_vmap.isNull()){
        return;
    }

    QSettings settings(filename, QSettings::IniFormat);
    QDir baseDir(QFileInfo(filename).absoluteDir());
    QString heightFilename = baseDir.filePath("height.png");
    QString metaFilename = baseDir.filePath("meta.png");

    settings.setValue("height", heightFilename);
    settings.setValue("meta", metaFilename);
    settings.sync();

    _vmap->heightImage()->save(heightFilename);
    _vmap->metaImage()->save(metaFilename);
}

QString VmapViewer::currentFile() const
{
    return _currentFile;
}

void VmapViewer::applyMask(uint8_t mask, bool apply)
{
    if(apply) {
        _mask = _mask | mask;
    } else {
        _mask = _mask & ~mask;
    }
    QSharedPointer<QImage> metaImage = _vmap->metaImage(_mask);
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

void VmapViewer::onDeltaMaskToggled(bool checked)
{
    applyMask(DeltaMask, checked);
}

void VmapViewer::onObjShadowMaskToggled(bool checked)
{
    applyMask(ObjShadowMask, checked);
}

void VmapViewer::onTerrainTypeMaskToggled(bool checked)
{
    applyMask(TerrainTypeMask, checked);
}

void VmapViewer::onDoubleLevelMaskToggled(bool checked)
{
    applyMask(DoubleLevelMask, checked);
}

void VmapViewer::onShadowMaskToggled(bool checked)
{
    applyMask(ShadowMask, checked);
}

ResourceViewer *VmapViewerPlugin::makeResourceViewer(QWidget *parent)
{
    return new VmapViewer(this, parent);
}
