#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDockWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    settings = new QSettings("Blarg City", "Goombatlas");

    ui->setupUi(this);
    setMapName();

    QString iconsPath(QCoreApplication::applicationDirPath() + "/goombatlas_data/icons/");

    // Load icons

    ui->actionNew->setIcon(QIcon(iconsPath + "new.png"));
    ui->actionOpen->setIcon(QIcon(iconsPath + "open.png"));
    ui->actionSave->setIcon(QIcon(iconsPath + "save.png"));
    ui->actionSaveAs->setIcon(QIcon(iconsPath + "save_as.png"));
    ui->actionZoomIn->setIcon(QIcon(iconsPath + "zoom_in.png"));
    ui->actionZoomOut->setIcon(QIcon(iconsPath + "zoom_out.png"));
    ui->actionZoom100->setIcon(QIcon(iconsPath + "zoom_100.png"));
    ui->actionMaximumZoom->setIcon(QIcon(iconsPath + "zoom_max.png"));
    ui->actionMinimumZoom->setIcon(QIcon(iconsPath + "zoom_min.png"));
    ui->actionGrid->setIcon(QIcon(iconsPath + "grid.png"));


    // Setup Map View

    map = new Map();

    mapView = new MapView(this, map);
    connect(mapView, SIGNAL(scrollTo(int,int)), this, SLOT(mapScrollTo(int,int)));
    connect(mapView, SIGNAL(scrollTo(QPoint)), this, SLOT(mapScrollTo(QPoint)));
    ui->mapViewArea->setWidget(mapView);


    // Setup Node Editor

    nodeEditor = new NodeEditorWidget(map, this);
    connect(nodeEditor, SIGNAL(redrawMap()), mapView, SLOT(update()));

    QDockWidget* nodeEditorDock = new QDockWidget("Node Editor", this);
    nodeEditorDock->setWidget(nodeEditor);

    addDockWidget(Qt::RightDockWidgetArea, nodeEditorDock);
    nodeEditorDock->toggleViewAction()->setShortcut(QKeySequence("Ctrl+1"));
    nodeEditorDock->toggleViewAction()->setIcon(QIcon(iconsPath + "node.png"));
    ui->menuWindow->addAction(nodeEditorDock->toggleViewAction());


    // Setup Node Editor

    pathBehaviorEditor = new PathBehaviorEditor(map, this);

    QDockWidget* pathBehaviorEditorDock = new QDockWidget("Path Setting Editor", this);
    pathBehaviorEditorDock->setWidget(pathBehaviorEditor);

    addDockWidget(Qt::RightDockWidgetArea, pathBehaviorEditorDock);
    pathBehaviorEditorDock->toggleViewAction()->setShortcut(QKeySequence("Ctrl+2"));
    pathBehaviorEditorDock->toggleViewAction()->setIcon(QIcon(iconsPath + "path_setting.png"));
    ui->menuWindow->addAction(pathBehaviorEditorDock->toggleViewAction());


    // Load Settings
    ui->actionGrid->setChecked(settings->value("GridEnabled", "1").toBool());

    // We cannot scroll before the window is created
    QTimer::singleShot(0, this, SLOT(onLoad()));
}

MainWindow::~MainWindow()
{
    settings->setValue("GridEnabled", ui->actionGrid->isChecked());

    delete settings;
    delete ui;
    delete mapView;
    delete map;
}

void MainWindow::onLoad()
{
    mapScrollTo(0, 0);
}

void MainWindow::updateMap(QFile* file)
{
    delete map;

    if (file == NULL)
        map = new Map();
    else
        map = new Map(file);

    mapView->setMap(map);
    nodeEditor->setMap(map);
    pathBehaviorEditor->setMap(map);
}


// Slots

void MainWindow::setMapName(QString mapName)
{
    setWindowTitle(mapName + " - Goombatlas");
}

void MainWindow::mapScrollTo(QPoint p)
{
    mapScrollTo(p.x(), p.y());
}

void MainWindow::mapScrollTo(int x, int y)
{
    float zoom = mapView->getZoom();

    int wR = mapView->visibleRegion().boundingRect().width();
    int hR = mapView->visibleRegion().boundingRect().height();

    ui->mapViewArea->horizontalScrollBar()->setValue(qMax((int)((x+mapView->getCenterX())*zoom)-wR/2, 0));
    ui->mapViewArea->verticalScrollBar()->setValue(qMax((int)((y+mapView->getCenterY())*zoom)-hR/2, 0));
}

void MainWindow::on_actionNew_triggered()
{
    updateMap();
    mapScrollTo(0, 0);
}

void MainWindow::on_actionOpen_triggered()
{
    QString openPath = QFileDialog::getOpenFileName(this, "Open Map", "", "Goombatlas Map (*.gbmap)");

    if (openPath.isEmpty() || openPath.isNull())
        return;

    QFileInfo fileInfo(openPath);

    if (!fileInfo.exists())
        return;

    QFile file(openPath);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::information(this, "Goombatlas", "Could not open or create " + openPath + " for reading!", QMessageBox::Ok);
        return;
    }

    updateMap(&file);
    setMapName(fileInfo.fileName());
}

void MainWindow::on_actionSave_triggered()
{
    if (!QFileInfo(map->getPath()).exists())
        on_actionSaveAs_triggered();
    else
        map->save(this);
}

void MainWindow::on_actionSaveAs_triggered()
{
    QString savePath = QFileDialog::getSaveFileName(this, "Save Map", "", "Goombatlas Map (*.gbmap)");
    map->save(this, savePath);
    QFileInfo fileInfo(savePath);
    setMapName(fileInfo.fileName());
}

void MainWindow::on_actionGrid_triggered(bool checked)
{
    mapView->toggleGrid(checked);
}

void MainWindow::on_actionZoom100_triggered()
{
    mapView->setZoom(2);
}

void MainWindow::on_actionZoomIn_triggered()
{
    mapView->zoomIn();
}

void MainWindow::on_actionZoomOut_triggered()
{
    mapView->zoomOut();
}

void MainWindow::on_actionMaximumZoom_triggered()
{
    mapView->zoomMax();
}

void MainWindow::on_actionMinimumZoom_triggered()
{
    mapView->zoomMin();
}
