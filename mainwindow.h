#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mapview.h"
#include "nodeeditorwidget.h"
#include "pathbehavioreditor.h"

#include <QMainWindow>
#include <QCoreApplication>
#include <QSettings>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void onLoad();

    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSaveAs_triggered();

    void on_actionGrid_triggered(bool checked);

private slots:
    void mapScrollTo(int x, int y);
    void mapScrollTo(QPoint p);

    void on_actionZoomIn_triggered();

    void on_actionZoom100_triggered();

    void on_actionZoomOut_triggered();

    void on_actionMaximumZoom_triggered();

    void on_actionMinimumZoom_triggered();

private:
    Ui::MainWindow *ui;
    QSettings* settings;

    Map* map;
    MapView* mapView;
    NodeEditorWidget* nodeEditor;
    PathBehaviorEditor* pathBehaviorEditor;

    void updateMap(QFile* file=NULL);
    void setMapName(QString mapName="Untitled Map");
};

#endif // MAINWINDOW_H
