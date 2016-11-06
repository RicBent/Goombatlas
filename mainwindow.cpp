#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDockWidget>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setMapName();

    map = new Map();

    mapView = new MapView(this, map);
    ui->mapViewArea->setWidget(mapView);


    QString iconsPath(QCoreApplication::applicationDirPath() + "/goombatlas_data/icons/");


    // Load icons

    ui->actionNew->setIcon(QIcon(iconsPath + "new.png"));
    ui->actionOpen->setIcon(QIcon(iconsPath + "open.png"));
    ui->actionSave->setIcon(QIcon(iconsPath + "save.png"));
    ui->actionSaveAs->setIcon(QIcon(iconsPath + "save_as.png"));


    // Setup the Node Editor

    nodeEditor = new NodeEditorWidget(map, this);

    QDockWidget* nodeEditorDock = new QDockWidget("Node Editor", this);
    nodeEditorDock->setWidget(nodeEditor);

    addDockWidget(Qt::RightDockWidgetArea, nodeEditorDock);
    nodeEditorDock->toggleViewAction()->setShortcut(QKeySequence("Ctrl+1"));
    nodeEditorDock->toggleViewAction()->setIcon(QIcon(iconsPath + "node.png"));
    ui->menuWindow->addAction(nodeEditorDock->toggleViewAction());
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mapView;
    delete map;
}

void MainWindow::updateMap(QFile* file)
{
    delete map;

    if (file == NULL)
        map = new Map();
    else
        map = new Map(file);

    nodeEditor->setMap(map);
}


// Slots

void MainWindow::setMapName(QString mapName)
{
    setWindowTitle(mapName + " - Goombatlas");
}

void MainWindow::on_actionNew_triggered()
{
    updateMap();
}

void MainWindow::on_actionOpen_triggered()
{
    QString openPath = QFileDialog::getOpenFileName(this, "Open Map", "", "Koopatlas Map (*.gbmap)");

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
    QString savePath = QFileDialog::getSaveFileName(this, "Save Map", "", "Koopatlas Map (*.gbmap)");
    map->save(this, savePath);
    QFileInfo fileInfo(savePath);
    setMapName(fileInfo.fileName());
}
