#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QSettings>
#include <QMenu>

#include <QItemSelectionModel>

#include "plugins/vmap/vmapviewerplugin.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , selectedResourceViewer(nullptr)
    , settings("lpenguin", "Vangers Resource Explorer")
{
    _plugins = {
        QSharedPointer<ImageViewerPlugin>::create(this),
        QSharedPointer<PaletteViewerPlugin>::create(this),
        QSharedPointer<VmapViewerPlugin>::create(this),
    };

    ui->setupUi(this);
	ui->resourceViewerTabWidget->setTabsClosable(true);
	QObject::connect(ui->resourceViewerTabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::imageTabs_closeRequested);
    ui->directoryTreeView->hide();
    ui->directoryTreeView->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->actionExport->setDisabled(true);
    QObject::connect(ui->directoryTreeView, &QTreeView::customContextMenuRequested, this, &MainWindow::onCustomContextMenu);
	QObject::connect(ui->resourceViewerTabWidget, &QTabWidget::tabBarDoubleClicked, this, &MainWindow::onTabDoubleClicked);
	loadRecent();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::openFile(QModelIndex index){

    QFileSystemModel* model = (QFileSystemModel*)ui->directoryTreeView->model();
	QString filename = model->filePath(index);
	loadFile(filename, true);
}

void MainWindow::openFolder()
{
    QString lastDir = settings.value("lastDir").toString();

    QString directory = QFileDialog::getExistingDirectory(this, "Open Vangers Resource Folder", lastDir);
    if(directory.isNull()){
        return;
    }
	loadFolder(directory);
	settings.setValue("lastDir", directory);
}

void MainWindow::loadFolder(const QString& folder)
{
	addRecentFolder(folder);
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
	QString rootDir = folder;
	model->setRootPath(rootDir);

	if(ui->directoryTreeView->selectionModel() != nullptr){
		QObject::disconnect(ui->directoryTreeView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::onDirectorySelectionChanged);
	}

	ui->directoryTreeView->setModel(model);
	ui->directoryTreeView->setRootIndex(model->index(rootDir));
//    ui->directoryTreeView->hideColumn(1);
	ui->directoryTreeView->hideColumn(2);
	ui->directoryTreeView->hideColumn(3);
	ui->directoryTreeView->setColumnWidth(0, 300);
	ui->directoryTreeView->show();
	QObject::connect(ui->directoryTreeView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::onDirectorySelectionChanged);

}

void MainWindow::loadFile(const QString& filename, bool current)
{
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

	ResourceViewer* viewer;
	if(current){
		if(selectedResourceViewer == nullptr || selectedResourceViewer->plugin() != plugin){
			if(selectedResourceViewer != nullptr){
				ui->resourceViewerTabWidget->removeTab(0);
			}

			selectedResourceViewer = viewer =  plugin->makeResourceViewer(ui->resourceViewerTabWidget);;
			ui->resourceViewerTabWidget->insertTab(0, selectedResourceViewer, title);
		} else {
			viewer = selectedResourceViewer;
		}
		ui->resourceViewerTabWidget->setCurrentWidget(selectedResourceViewer);
		ui->resourceViewerTabWidget->setTabText(0, title);
		ui->resourceViewerTabWidget->setTabToolTip(0, fInfo.absoluteFilePath());
	} else {
		for(int i = 0; i < ui->resourceViewerTabWidget->count(); i++){
			if(fInfo.absoluteFilePath() == ui->resourceViewerTabWidget->tabToolTip(i)){
				ui->resourceViewerTabWidget->setCurrentIndex(i);
				return;
			}
		}

		viewer = plugin->makeResourceViewer(ui->resourceViewerTabWidget);
		ui->resourceViewerTabWidget->addTab(viewer, fInfo.fileName());
		ui->resourceViewerTabWidget->setCurrentWidget(viewer);
		ui->resourceViewerTabWidget->setTabToolTip(ui->resourceViewerTabWidget->currentIndex(), fInfo.absoluteFilePath());

		addRecentFile(filename);
	}

	viewer->importResource(filename, type);
	ui->actionExport->setEnabled(true);
}

void MainWindow::imageTabs_closeRequested(int index)
{
	QWidget* selected = ui->resourceViewerTabWidget->widget(index);
	ui->resourceViewerTabWidget->removeTab(index);
    delete selected;
    if(index == 0){
        selectedResourceViewer = nullptr;
    }
	if(ui->resourceViewerTabWidget->count() == 0){
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

void MainWindow::onDirectorySelectionChanged(QItemSelection selected, QItemSelection deselected)
{
     if(selected.size() == 0){
        return;
    }

    auto indexes = selected[0].indexes();
    if(indexes.size() == 0){
        return;
    }

    QModelIndex index = indexes[0];
	openFile(index);
}

void MainWindow::onTabDoubleClicked(int index)
{
	if(index == 0 && selectedResourceViewer != nullptr){
		QString title = ui->resourceViewerTabWidget->tabText(index);
		ui->resourceViewerTabWidget->setTabText(0, title.remove("Selected: "));
		selectedResourceViewer = nullptr;
	}
}

void MainWindow::loadRecent()
{
	QStringList recentFiles = settings.value("recentFiles").toStringList();
	QStringList recentFolders = settings.value("recentFolders").toStringList();

	setRecent(recentFiles, ui->menuRecent_Files, [this](auto s) { loadFile(s, false);});
	setRecent(recentFolders, ui->menuRecent_Folders, [this](auto s) { loadFolder(s);});
}

void MainWindow::setRecent(const QStringList& recent, QMenu* menu, std::function<void (QString)> callback)
{
	menu->clear();
	for(const QString& recent: recent){
		QAction* action = new QAction(recent, this);
		action->setData(recent);
		connect(action, &QAction::triggered, [callback, recent](bool){callback(recent);});
		menu->addAction(action);
	}
}

void MainWindow::addRecent(const QString& recent,
						   const QString& settingName,
						   QMenu* menu,
						   std::function<void (QString)> callback)
{
	QStringList recents = settings.value(settingName).toStringList();

	recents.removeAll(recent);
	recents.prepend(recent);

	while(recents.size() > MAX_RECENT){
		recents.pop_back();
	}
	settings.setValue(settingName, recents);

	setRecent(recents, menu, callback);
}

void MainWindow::addRecentFile(const QString& recentFile)
{
	addRecent(recentFile, "recentFiles", ui->menuRecent_Files, [this](auto s) { loadFile(s, false); });
}

void MainWindow::addRecentFolder(const QString& recentFolder)
{
	addRecent(recentFolder, "recentFolders", ui->menuRecent_Folders, [this](auto s) { loadFolder(s);});
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

	loadFile(filename, false);
	QFileInfo fInfo(filename);
	settings.setValue("lastFileDir", fInfo.absoluteDir().absolutePath());
}

void MainWindow::exportFile()
{
	ResourceViewer* viewer = (ResourceViewer*)(ui->resourceViewerTabWidget->currentWidget());
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
                 QFileInfo(viewer->currentFile()).dir().path(),
                filtersList.join(";;"),
                &selectedFilter);

    int typeIndex = filtersList.indexOf(selectedFilter);
    if(typeIndex < 0 || typeIndex >= supportedTypes.size()){
        return;
    }

    viewer->exportResource(filename, supportedTypes[typeIndex]);
}

