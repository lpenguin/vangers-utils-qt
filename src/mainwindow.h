#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
#include "image/image.h"
#include "imageviewer.h"

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
private:
    QString getLastOpenedDirectory() const;
    void setLastOpenedDirectory(QString dir);
    Ui::MainWindow *ui;
    ImageViewer* selectedImageViewer;
};
#endif // MAINWINDOW_H
