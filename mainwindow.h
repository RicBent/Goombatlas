#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mode.h"
#include "mapview.h"
#include "nodeeditorwidget.h"
#include "pathbehavioreditor.h"
#include "mapobjecteditor.h"
#include "spriteseditor.h"

#include <QMainWindow>
#include <QCoreApplication>
#include <QSettings>
#include <QScrollArea>
#include <QResizeEvent>
#include <QDockWidget>
#include <QComboBox>

namespace Ui {
class MainWindow;
}

class QScrollAreaRE : public QScrollArea
{
    Q_OBJECT
public:
    QScrollAreaRE() {}
    virtual ~QScrollAreaRE() {}
signals:
    void resized(QResizeEvent* evt);
protected:
    void resizeEvent(QResizeEvent* evt)
    {
        QScrollArea::resizeEvent(evt);
        emit resized(evt);
    }
};

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
    void modeChanged(int modeIndex);

    void mapScrollTo(int x, int y);

    void mapScrollTo(QPoint p);

    void mapScrollTo_(int x, int y);

    void on_actionZoomIn_triggered();

    void on_actionZoom100_triggered();

    void on_actionZoomOut_triggered();

    void on_actionMaximumZoom_triggered();

    void on_actionMinimumZoom_triggered();

    void on_actionExportC_triggered();

private:
    Ui::MainWindow *ui;
    QSettings* settings;
    QComboBox* modeBox;

    QList<Mode> modes;

    Map* map;
    MapView* mapView;
    QScrollAreaRE* mapViewArea;
    NodeEditorWidget* nodeEditor;
    PathBehaviorEditor* pathBehaviorEditor;
    MapObjectEditor* mapObjectEditor;
    SpritesEditor* spritesEditor;

    void updateMap(QFile* file=NULL);
    void setMapName(QString mapName="Untitled Map");
};

#endif // MAINWINDOW_H
