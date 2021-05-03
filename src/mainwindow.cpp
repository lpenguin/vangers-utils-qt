#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QSettings>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , selectedImageViewer(nullptr)
{
    ui->setupUi(this);
    ui->selectedImageTab->setTabsClosable(true);
    QObject::connect(ui->selectedImageTab, &QTabWidget::tabCloseRequested, this, &MainWindow::imageTabs_closeRequested);
    ui->directoryTreeView->hide();
    ui->actionExport->setDisabled(true);
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
    if(selectedImageViewer == nullptr){
        selectedImageViewer = new ImageViewer(ui->selectedImageTab);
        ui->selectedImageTab->insertTab(0, selectedImageViewer, title);
    }
    selectedImageViewer->setImage(filename);
    ui->selectedImageTab->setCurrentWidget(selectedImageViewer);
    ui->selectedImageTab->setTabText(0, title);
    ui->selectedImageTab->setTabToolTip(0, filename);
    ui->actionExport->setEnabled(true);
}

void MainWindow::openFolder()
{
    QString lastDir = getLastOpenedDirectory();

    QString directory = QFileDialog::getExistingDirectory(this, "Open Vangers Resource Folder", lastDir);
    if(directory.isNull()){
        return;
    }
    setLastOpenedDirectory(directory);
    QFileSystemModel* model = new QFileSystemModel(this);
    model->setNameFilters(QStringList() << "*.bmp" << "*.bml" << "*.bmo" << "*.xbm" << "*.png");
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
        selectedImageViewer = nullptr;
    }
    if(ui->selectedImageTab->count() == 0){
        ui->actionExport->setDisabled(true);
    }
}

QString MainWindow::getLastOpenedDirectory() const
{
    QSettings settings("lpenguin", "Vangers Resource Explorer");
    return settings.value("lastDir").toString();
}

void MainWindow::setLastOpenedDirectory(QString dir)
{
    QSettings settings("lpenguin", "Vangers Resource Explorer");
    settings.setValue("lastDir", dir);
}

void MainWindow::openFile()
{
    QString root = getLastOpenedDirectory();

    auto filename = QFileDialog::getOpenFileName(this, tr("Open file"), root);
    if(filename.isNull() || filename.isEmpty()){
        return;
    }

    ImageViewer* viewer = new ImageViewer();
    QFileInfo fInfo(filename);
    setLastOpenedDirectory(fInfo.dir().path());

    ui->selectedImageTab->addTab(viewer, fInfo.fileName());    
    ui->selectedImageTab->setCurrentWidget(viewer);
    ui->selectedImageTab->setTabToolTip(ui->selectedImageTab->currentIndex(), filename);
    ui->actionExport->setEnabled(true);

    viewer->setImage(filename);
}

void MainWindow::exportFile()
{
    ImageViewer* viewer = (ImageViewer*)(ui->selectedImageTab->currentWidget());
    viewer->exportImage();
}

