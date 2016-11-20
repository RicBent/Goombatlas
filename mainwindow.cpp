#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "exportmapdialog.h"

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


    // Setup Mode Selection Box

    modeBox = new QComboBox(this);
    ui->toolBar->insertWidget(ui->actionModePlaceholder, modeBox);
    ui->toolBar->removeAction(ui->actionModePlaceholder);
    connect(modeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(modeChanged(int)));

    modes.append(Mode("Edition"));
    modes.append(Mode("Animation"));


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
    ui->actionExportC->setIcon(QIcon(iconsPath + "export_c"));


    // Setup Map View

    map = new Map();

    mapViewArea = new QScrollAreaRE();
    ui->mainLayout->addWidget(mapViewArea);

    mapView = new MapView(this, map);
    connect(mapView, SIGNAL(scrollTo(int,int)), this, SLOT(mapScrollTo(int,int)));
    connect(mapView, SIGNAL(scrollTo(QPoint)), this, SLOT(mapScrollTo(QPoint)));
    connect(mapView, SIGNAL(scrollTo_(int,int)), this, SLOT(mapScrollTo_(int,int)));
    mapViewArea->setWidget(mapView);
    connect(mapViewArea, SIGNAL(resized(QResizeEvent*)), mapView, SLOT(viewAreaResized(QResizeEvent*)));


    // Setup Node Editor

    nodeEditor = new NodeEditorWidget(map, mapView, this);
    connect(nodeEditor, SIGNAL(redrawMap()), mapView, SLOT(repaint()));
    connect(mapView, SIGNAL(changeSelectedNode(Node*)), nodeEditor, SLOT(select(Node*)));
    connect(mapView, SIGNAL(changeDeselectedNode()), nodeEditor, SLOT(deselect()));
    connect(nodeEditor, SIGNAL(nodeSelected(MovableObject*)), mapView, SLOT(select(MovableObject*)));
    connect(nodeEditor, SIGNAL(nodeDeselected()), mapView, SLOT(deselect()));

    QDockWidget* nodeEditorDock = new QDockWidget("Node Editor", this);
    nodeEditorDock->setWidget(nodeEditor);

    addDockWidget(Qt::LeftDockWidgetArea, nodeEditorDock);
    nodeEditorDock->toggleViewAction()->setShortcut(QKeySequence("Ctrl+1"));
    nodeEditorDock->toggleViewAction()->setIcon(QIcon(iconsPath + "node.png"));
    ui->menuWindow->addAction(nodeEditorDock->toggleViewAction());
    modes[0].addDock(nodeEditorDock);


    // Setup Path Behavior Editor

    pathBehaviorEditor = new PathBehaviorEditor(map, this);

    QDockWidget* pathBehaviorEditorDock = new QDockWidget("Path Setting Editor", this);
    pathBehaviorEditorDock->setWidget(pathBehaviorEditor);

    addDockWidget(Qt::RightDockWidgetArea, pathBehaviorEditorDock);
    pathBehaviorEditorDock->toggleViewAction()->setShortcut(QKeySequence("Ctrl+2"));
    pathBehaviorEditorDock->toggleViewAction()->setIcon(QIcon(iconsPath + "path_setting.png"));
    ui->menuWindow->addAction(pathBehaviorEditorDock->toggleViewAction());
    modes[0].addDock(pathBehaviorEditorDock);


    // Setup Map Objects Editor

    mapObjectEditor = new MapObjectEditor(map, mapView, this);
    connect(mapObjectEditor, SIGNAL(redrawMap()), mapView, SLOT(repaint()));
    connect(mapView, SIGNAL(changeSelectedMapObj(MapObject*)), mapObjectEditor, SLOT(select(MapObject*)));
    connect(mapView, SIGNAL(changeDeselectedMapObj()), mapObjectEditor, SLOT(deselect()));
    connect(mapObjectEditor, SIGNAL(objectSelected(MovableObject*)), mapView, SLOT(select(MovableObject*)));
    connect(mapObjectEditor, SIGNAL(objectDeselected()), mapView, SLOT(deselect()));

    QDockWidget* mapObjectEditorDock = new QDockWidget("Map Objects Editor", this);
    mapObjectEditorDock->setWidget(mapObjectEditor);

    addDockWidget(Qt::RightDockWidgetArea, mapObjectEditorDock);
    mapObjectEditorDock->toggleViewAction()->setShortcut(QKeySequence("Ctrl+3"));
    mapObjectEditorDock->toggleViewAction()->setIcon(QIcon(iconsPath + "object.png"));
    ui->menuWindow->addAction(mapObjectEditorDock->toggleViewAction());
    modes[0].addDock(mapObjectEditorDock);


    // Setup Sprites Editor

    spritesEditor = new SpritesEditor(map, this);

    QDockWidget* spritesEditorDock = new QDockWidget("Sprites Editor", this);
    spritesEditorDock->setWidget(spritesEditor);

    addDockWidget(Qt::RightDockWidgetArea, spritesEditorDock);
    spritesEditorDock->toggleViewAction()->setShortcut(QKeySequence("Ctrl+4"));
    spritesEditorDock->toggleViewAction()->setIcon(QIcon(iconsPath + "sprite.png"));
    ui->menuWindow->addAction(spritesEditorDock->toggleViewAction());
    modes[0].addDock(spritesEditorDock);


    ui->menuWindow->addSeparator();


    // Setup Animations Editor

    animationsEditor = new AnimationsEditor(map, mapView, this);
    connect(animationsEditor, SIGNAL(redrawMap()), mapView, SLOT(repaint()));
    connect(mapView, SIGNAL(changeSelectedKeyframe(Keyframe*)), animationsEditor, SLOT(kSelect(Keyframe*)));
    connect(mapView, SIGNAL(changeDeselectedKeyframe()), animationsEditor, SLOT(kDeselect()));
    connect(animationsEditor, SIGNAL(keyframeSelected(MovableObject*)), mapView, SLOT(select(MovableObject*)));
    connect(animationsEditor, SIGNAL(keyframeDeselected()), mapView, SLOT(deselect()));

    QDockWidget* animationsEditorDock = new QDockWidget("Path Animations Editor", this);
    animationsEditorDock->setWidget(animationsEditor);

    addDockWidget(Qt::LeftDockWidgetArea, animationsEditorDock);
    animationsEditorDock->toggleViewAction()->setShortcut(QKeySequence("Ctrl+1"));
    animationsEditorDock->toggleViewAction()->setIcon(QIcon(iconsPath + "walk.png"));
    ui->menuWindow->addAction(animationsEditorDock->toggleViewAction());
    modes[1].addDock(animationsEditorDock);

    // Update Mode Selector
    for (int i = 0; i < modes.length(); i++)
    {
        modes[i].deactivate();
        modeBox->addItem(modes[i].getName());
    }
    modes[0].activate();

    // Load Settings
    ui->actionGrid->setChecked(settings->value("GridEnabled", "1").toBool());
    mapView->toggleGrid(settings->value("GridEnabled", "1").toBool());

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

QMenu* MainWindow::createPopupMenu()
{
    QMenu* filteredMenu = QMainWindow::createPopupMenu();
    filteredMenu->removeAction(ui->toolBar->toggleViewAction());
    return filteredMenu;
}

void MainWindow::modeChanged(int modeIndex)
{
    for (int i = 0; i < modes.length(); i++)
        modes[i].deactivate();
    modes[modeIndex].activate();

    mapView->setMode(modeIndex);
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
    mapObjectEditor->setMap(map);
    spritesEditor->setMap(map);
    animationsEditor->setMap(map);
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

    mapViewArea->horizontalScrollBar()->setValue(qMax((int)((x+mapView->getCenterX())*zoom)-wR/2, 0));
    mapViewArea->verticalScrollBar()->setValue(qMax((int)((y+mapView->getCenterY())*zoom)-hR/2, 0));
}

void MainWindow::mapScrollTo_(int x, int y)
{
    qMax(0, x);
    qMax(0, y);
    mapViewArea->horizontalScrollBar()->setValue(x);
    mapViewArea->verticalScrollBar()->setValue(y);
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
        QMessageBox::information(this, "Goombatlas", "Could not open " + openPath + " for reading!", QMessageBox::Ok);
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
    if (savePath == "")
        return;
    map->save(this, savePath);
    QFileInfo fileInfo(savePath);
    setMapName(fileInfo.fileName());
}

void MainWindow::on_actionExportC_triggered()
{
    ExportMapDialog emd(this);
    emd.exec();

    map->exportAsC(emd.getCPath(), emd.getReplacesPath(), emd.getPrefix(), emd.getWorldId());
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
