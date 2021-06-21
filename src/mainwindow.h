#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QItemSelection>
#include <QMainWindow>
#include <QModelIndex>
#include <QSettings>
#include "image/image.h"
#include "plugins/image/imageviewer.h"
#include "plugins/palette/paletteviewer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void openFile();
    void exportFile();
    void openFile(QModelIndex index);
    void openFolder();
private slots:
    void imageTabs_closeRequested(int index);
    void onCustomContextMenu(const QPoint & point);
    void onDirectorySelectionChanged(QItemSelection selected, QItemSelection deselected);
private:
    QSharedPointer<ResourceViewerPlugin> findImportPlugin(const QString& filename, ResourceType& outType);
    Ui::MainWindow *ui;

    ResourceViewer* selectedResourceViewer;
    QSettings settings;
    QList<QSharedPointer<ResourceViewerPlugin>> _plugins;
};
#endif // MAINWINDOW_H
