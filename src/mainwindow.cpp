#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QSettings>
#include <QMenu>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , selectedResourceViewer(nullptr)
    , settings("lpenguin", "Vangers Resource Explorer")
{
    _plugins = {
        QSharedPointer<ImageViewerPlugin>::create(this),
        QSharedPointer<PaletteViewerPlugin>::create(this),
    };

    ui->setupUi(this);
    ui->selectedImageTab->setTabsClosable(true);
    QObject::connect(ui->selectedImageTab, &QTabWidget::tabCloseRequested, this, &MainWindow::imageTabs_closeRequested);
    ui->directoryTreeView->hide();
    ui->directoryTreeView->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->actionExport->setDisabled(true);
    QObject::connect(ui->directoryTreeView, &QTreeView::customContextMenuRequested, this, &MainWindow::onCustomContextMenu);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::openFile(QModelIndex index){

    QFileSystemModel* model = (QFileSystemModel*)ui->directoryTreeView->model();
    auto filename = model->filePath(index);
    qDebug() << filename;

    QFileInfo fInfo(filename);
    if(!fInfo.exists() || !fInfo.isFile()){
        return;
    }
    QString title = QString("Selected: %1").arg(fInfo.fileName());

    ResourceType type;
    auto plugin = findImportPlugin(filename, type);
    if(plugin.isNull()){
        return;
    }

    if(selectedResourceViewer == nullptr || selectedResourceViewer->plugin() != plugin){
        if(selectedResourceViewer != nullptr){
//            delete selectedResourceViewer;
            ui->selectedImageTab->removeTab(0);
        }

        selectedResourceViewer = plugin->makeResourceViewer(ui->selectedImageTab);
        ui->selectedImageTab->insertTab(0, selectedResourceViewer, title);
    }
    selectedResourceViewer->importResource(filename, type);
    ui->selectedImageTab->setCurrentWidget(selectedResourceViewer);
    ui->selectedImageTab->setTabText(0, title);
    ui->selectedImageTab->setTabToolTip(0, filename);
    ui->actionExport->setEnabled(true);

}

void MainWindow::openFolder()
{
    QString lastDir = settings.value("lastDir").toString();

    QString directory = QFileDialog::getExistingDirectory(this, "Open Vangers Resource Folder", lastDir);
    if(directory.isNull()){
        return;
    }
    settings.setValue("lastDir", directory);

    QFileSystemModel* model = new QFileSystemModel(this);
    QStringList nameFilters;
    for(auto& plugin: _plugins){
        for(auto& resourceType: plugin->supportedImportTypes()){
            for(auto& extension: resourceType.extensions){
                nameFilters << extension;
            }
        }
    }

    model->setNameFilters(nameFilters);
    QString rootDir = directory;
    model->setRootPath(rootDir);

    ui->directoryTreeView->setModel(model);
    ui->directoryTreeView->setRootIndex(model->index(rootDir));
//    ui->directoryTreeView->hideColumn(1);
    ui->directoryTreeView->hideColumn(2);
    ui->directoryTreeView->hideColumn(3);
    ui->directoryTreeView->setColumnWidth(0, 300);
    ui->directoryTreeView->show();

}

void MainWindow::imageTabs_closeRequested(int index)
{
    QWidget* selected = ui->selectedImageTab->widget(index);
    ui->selectedImageTab->removeTab(index);
    delete selected;
    if(index == 0){
        selectedResourceViewer = nullptr;
    }
    if(ui->selectedImageTab->count() == 0){
        ui->actionExport->setDisabled(true);
    }
}

void MainWindow::onCustomContextMenu(const QPoint& point)
{
#if 0
    QModelIndex index = ui->directoryTreeView->indexAt(point);
    if (index.isValid()) {
        QFileSystemModel* model = (QFileSystemModel*)ui->directoryTreeView->model();
        auto filename = model->filePath(index);

        QMenu* contextMenu = new QMenu(ui->directoryTreeView);
        for(int iPlugin = 0; iPlugin < _plugins.size(); iPlugin++) {
            const auto& plugin = _plugins[iPlugin];
            for(auto& type : plugin->supportedImportTypes()){
                for(auto& extension: type.extensions){
                    QRegExp re(extension);
                    re.setPatternSyntax(QRegExp::Wildcard);
                    if(re.exactMatch(filename)){
                        QAction* action = contextMenu->addAction(plugin->name());
                        QObject::connect(action, &QAction::triggered, [this, iPlugin](bool){
                            qDebug() << _plugins[iPlugin]->name();
                        });
                        continue;
                    }
                }
            }

        }
        contextMenu->exec(ui->directoryTreeView->viewport()->mapToGlobal(point));
    }
#endif
}

QSharedPointer<ResourceViewerPlugin> MainWindow::findImportPlugin(const QString& filename, ResourceType& outType)
{
    for(auto& plugin: _plugins){
        for(auto& resourceType: plugin->supportedImportTypes()){
            for(auto& extension: resourceType.extensions){
                QRegExp rx(extension);
                rx.setPatternSyntax(QRegExp::Wildcard);
                if(rx.exactMatch(filename)){
                    outType = resourceType;
                    return plugin;
                }
            }
        }
    }
    return QSharedPointer<ResourceViewerPlugin>{nullptr};
}

void MainWindow::openFile()
{
    QString root = settings.value("lastFileDir").toString();

    QStringList nameFilters;

    QList<int> pluginIndices;
    QList<ResourceType> types;
    for(int iPlugin = 0; iPlugin < _plugins.size(); iPlugin++) {
        const auto& plugin = _plugins[iPlugin];
        for(auto& resourceType: plugin->supportedImportTypes()){
            pluginIndices << iPlugin;
            types << resourceType;
            nameFilters << QString("%1 (%2)")
                           .arg(resourceType.name)
                           .arg(resourceType.extensions.join(" "));
        }
    }

    QString selectedFilter;
    auto filename = QFileDialog::getOpenFileName(this,
                                                 tr("Open file"),
                                                 root,
                                                 nameFilters.join(";;"),
                                                 &selectedFilter);

    if(filename.isNull() || filename.isEmpty()){
        return;
    }

    int selectedIndex = nameFilters.indexOf(selectedFilter);
    if(selectedIndex < 0){
        return;
    }

    auto plugin = _plugins[pluginIndices[selectedIndex]];
    const auto& type = types[selectedIndex];

//    ResourceType type;
//    auto plugin = findImportPlugin(filename, type);
//    if(plugin.isNull()){
//        return;
//    }
    auto viewer = plugin->makeResourceViewer();
    QFileInfo fInfo(filename);
    settings.setValue("lastFileDir", fInfo.dir().path());

    ui->selectedImageTab->addTab(viewer, fInfo.fileName());    
    ui->selectedImageTab->setCurrentWidget(viewer);
    ui->selectedImageTab->setTabToolTip(ui->selectedImageTab->currentIndex(), filename);
    ui->actionExport->setEnabled(true);

    viewer->importResource(filename, type);
}

void MainWindow::exportFile()
{
    ResourceViewer* viewer = (ResourceViewer*)(ui->selectedImageTab->currentWidget());
    QStringList filtersList;

    auto supportedTypes = viewer->plugin()->supportedExportTypes();
    for(auto& type: supportedTypes){
        filtersList << QString("%1 (%2)")
                       .arg(type.name)
                       .arg(type.extensions.join(" "));
    }



    QString selectedFilter;

    auto filename = QFileDialog::getSaveFileName(
                this,
                tr("Save file"),
                 QFileInfo(viewer->currentFile()).baseName(),
                filtersList.join(";;"),
                &selectedFilter);

    int typeIndex = filtersList.indexOf(selectedFilter);
    if(typeIndex < 0 || typeIndex >= supportedTypes.size()){
        return;
    }

    viewer->exportResource(filename, supportedTypes[typeIndex]);
}

