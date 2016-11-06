#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mapview.h"
#include "nodeeditorwidget.h"

#include <QMainWindow>

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

    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSaveAs_triggered();

private:
    Ui::MainWindow *ui;

    Map* map;
    MapView* mapView;
    NodeEditorWidget* nodeEditor;

    void updateMap(QFile* file=NULL);
    void setMapName(QString mapName="Untitled Map");
};

#endif // MAINWINDOW_H
