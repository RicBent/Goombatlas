#include "pathbehavioreditor.h"

#include <QVBoxLayout>
#include <QToolBar>
#include <QAction>
#include <QLabel>

PathBehaviorEditor::PathBehaviorEditor(Map* map, QWidget* parent)
{
    this->map = map;

    QString iconsPath(QCoreApplication::applicationDirPath() + "/goombatlas_data/icons/");


    QVBoxLayout* layout = new QVBoxLayout();


    pathBehaviorList = new QListWidget(this);
    connect(pathBehaviorList, SIGNAL(currentRowChanged(int)), this, SLOT(pathBehaviorsListIndexChanged(int)));
    layout->addWidget(pathBehaviorList);


    QToolBar* buttonsBar = new QToolBar(this);

    QAction* addPathBehaviorBtn = new QAction(QIcon(iconsPath + "path_setting_add.png"), "Add Path Behavior", this);
    connect(addPathBehaviorBtn, SIGNAL(triggered()), this, SLOT(addPathBehaviorClicked()));
    buttonsBar->addAction(addPathBehaviorBtn);

    QAction* removePathBehaviorBtn = new QAction(QIcon(iconsPath + "path_setting_remove.png"), "Remove Path Behavior", this);
    connect(removePathBehaviorBtn, SIGNAL(triggered()), this, SLOT(removePathBehaviorClicked()));
    buttonsBar->addAction(removePathBehaviorBtn);

    QAction* movePathBehaviorBtn = new QAction(QIcon(iconsPath + "move_up.png"), "Move Up", this);
    connect(movePathBehaviorBtn, SIGNAL(triggered()), this, SLOT(moveUpClicked()));
    buttonsBar->addAction(movePathBehaviorBtn);

    QAction* movePathBehaviorDownBtn = new QAction(QIcon(iconsPath + "move_down.png"), "Move Down", this);
    connect(movePathBehaviorDownBtn, SIGNAL(triggered()), this, SLOT(moveDownClicked()));
    buttonsBar->addAction(movePathBehaviorDownBtn);

    layout->addWidget(buttonsBar);


    layout->addWidget(new HorLine());

    QGridLayout* editsLayout = new QGridLayout();

    editsLayout->addWidget(new QLabel("Animation ID:"), 0, 0, 1, 1, Qt::AlignRight);
    animationId = new QSpinBox(this);
    animationId->setRange(0, 255);
    animationId->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    connect(animationId, SIGNAL(valueChanged(int)), this, SLOT(animationIdChanged(int)));
    editsLayout->addWidget(animationId, 0, 1, 1, 1);

    editsLayout->addWidget(new QLabel("Star Coin Cost:"), 1, 0, 1, 1, Qt::AlignRight);
    starCoinCost = new QSpinBox(this);
    starCoinCost->setRange(0, 255);
    connect(starCoinCost, SIGNAL(valueChanged(int)), this, SLOT(starCoinCostChanged(int)));
    editsLayout->addWidget(starCoinCost, 1, 1, 1, 1);

    editsLayout->addWidget(new QLabel("Settings:"), 2, 0, 1, 1, Qt::AlignRight);
    settings = new QComboBox(this);
    settings->addItem("Semi-Invisible Path");
    settingsHash.insert(0, 0);
    settings->addItem("Invisible Path");
    settingsHash.insert(1, 1);
    settings->addItem("Star Coin Sign");
    settingsHash.insert(2, 2);
    settings->addItem("Worldmap Pipe");
    settingsHash.insert(3, 4);
    settings->addItem("Cutscene");
    settingsHash.insert(4, 0x40);
    settings->addItem("Unknown (0x80)");
    settingsHash.insert(5, 0x80);
    connect(settings, SIGNAL(currentIndexChanged(int)), this, SLOT(settingsChanged(int)));
    editsLayout->addWidget(settings, 2, 1, 1, 1);

    layout->addLayout(editsLayout);


    this->setLayout(layout);

    this->setMinimumWidth(200);

    setEditsEnabled(false);
    clearValues();
    updateList();
}

void PathBehaviorEditor::select(PathBehavior* pathBehavior)
{
    editPathBehavior = pathBehavior;
    setEditsEnabled(true);
    pathBehaviorList->blockSignals(true);
    pathBehaviorList->setCurrentRow(map->pathBehaviors.indexOf(pathBehavior));
    pathBehaviorList->blockSignals(false);
    updateInfo();
}

void PathBehaviorEditor::deselect()
{
    pathBehaviorList->selectionModel()->clear();
    setEditsEnabled(false);
    clearValues();
}

void PathBehaviorEditor::setMap(Map* map)
{
    this->map = map;
    setEditsEnabled(false);
    pathBehaviorList->blockSignals(true);
    pathBehaviorList->setCurrentRow(0);
    updateList();
    pathBehaviorList->blockSignals(false);
}

void PathBehaviorEditor::updateList()
{
    pathBehaviorList->blockSignals(true);
    int index = pathBehaviorList->currentRow();
    pathBehaviorList->clear();

    for (int i=0; i < map->pathBehaviors.length(); i++)
    {
        pathBehaviorList->addItem(QString("Path Behavior %1").arg(i));
    }

    pathBehaviorList->setCurrentRow(index);
    pathBehaviorList->blockSignals(false);
}

void PathBehaviorEditor::setEditsEnabled(bool enabled)
{
    settings->setEnabled(enabled);
    animationId->setEnabled(enabled);
    starCoinCost->setEnabled(enabled);
}

void PathBehaviorEditor::updateInfo()
{
    allowChanges(false);
    animationId->setValue(editPathBehavior->getAnimationId());
    starCoinCost->setValue(editPathBehavior->getStarCoinCost());
    settings->setCurrentIndex(settingsHash.key(editPathBehavior->getSettings(), -1));
    allowChanges(true);
}

void PathBehaviorEditor::allowChanges(bool allow)
{
    settings->blockSignals(!allow);
    animationId->blockSignals(!allow);
    starCoinCost->blockSignals(!allow);
}

void PathBehaviorEditor::clearValues()
{
    allowChanges(false);
    settings->setCurrentIndex(-1);
    animationId->setValue(0);
    starCoinCost->setValue(0);
    allowChanges(true);
}


// Slots

void PathBehaviorEditor::pathBehaviorsListIndexChanged(int index)
{
    if (index == -1)
        return;

    select(map->getPathBehaviorPtr(index));
}

void PathBehaviorEditor::addPathBehaviorClicked()
{
    PathBehavior* newPathBehavior = new PathBehavior();

    int insertIndex = -1;
    if (pathBehaviorList->currentIndex().row() != -1)
        insertIndex = pathBehaviorList->currentIndex().row() + 1;

    map->addPathBehavior(newPathBehavior, insertIndex);

    updateList();

    pathBehaviorList->setCurrentRow(map->pathBehaviors.indexOf(newPathBehavior));
}

void PathBehaviorEditor::moveUpClicked()
{
    int currentIndex = pathBehaviorList->currentIndex().row();

    if (currentIndex == -1)
        return;

    PathBehavior* selPathBehavior = map->getPathBehaviorPtr(currentIndex);
    map->movePathBehaviorUp(currentIndex);
    updateList();
    select(selPathBehavior);
}

void PathBehaviorEditor::moveDownClicked()
{
    int currentIndex = pathBehaviorList->currentIndex().row();

    if (currentIndex == -1)
        return;

    PathBehavior* selPathBehavior = map->getPathBehaviorPtr(currentIndex);
    map->movePathBehaviorDown(currentIndex);
    updateList();
    select(selPathBehavior);
}

void PathBehaviorEditor::removePathBehaviorClicked()
{
    if (pathBehaviorList->currentIndex().row() == -1)
        return;

    map->removePathBehavior(pathBehaviorList->currentIndex().row());

    updateList();

    deselect();
}

void PathBehaviorEditor::animationIdChanged(int animationId)
{
    editPathBehavior->setAnimationId((quint8)animationId);
    updateList();
}

void PathBehaviorEditor::starCoinCostChanged(int starCoinCost)
{
    editPathBehavior->setStarCoinCost((quint8)starCoinCost);
    updateList();
}

void PathBehaviorEditor::settingsChanged(int index)
{
    editPathBehavior->setSettings(settingsHash.value(index, 0));
    updateList();
}
