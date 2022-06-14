#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <functional>
#include <QItemSelection>
#include <QMainWindow>
#include <QModelIndex>
#include <QSettings>
#include <vangers/core/plugin/resourceviewer.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class RecentData {

};

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
	void onTabDoubleClicked(int index);
private:
	const int MAX_RECENT = 12;
	void loadFolder(const QString& folder);
	void loadFile(const QString& file, bool current, QString selectedFilter = QString());

	void loadRecent();
	void setRecent(const QStringList& recent, QMenu* menu, std::function<void (QString)> callback);
	void addRecent(const QString& recent,
				   const QString& settingName,
				   QMenu* menu,
				   std::function<void (QString)> callback);

	void addRecentFile(const QString& recentFile);
	void addRecentFolder(const QString& recentFolder);
	QSharedPointer<vangers::core::plugin::ResourceViewerPlugin> findImportPlugin(
			const QString& filename,
			const QString& selectedPlugin,
			vangers::core::plugin::ResourceType& outType);

    Ui::MainWindow *ui;

	vangers::core::plugin::ResourceViewer* selectedResourceViewer;
    QSettings settings;
	QList<QSharedPointer<vangers::core::plugin::ResourceViewerPlugin>> _plugins;
};
#endif // MAINWINDOW_H
