#include "mapobjecteditor.h"

#include <QTabWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QAction>
#include <QCoreApplication>

MapObjectEditor::MapObjectEditor(Map* map, MapView* mapView, QWidget* parent) : QWidget(parent)
{
    this->map = map;
    this->mapView = mapView;


    QVBoxLayout* layout = new QVBoxLayout();

    type = new QComboBox(this);
    type->addItem("Star Coin Signs");
    type->addItem("Towers/Castles");
    type->addItem("Mushroom Houses");
    connect(type, SIGNAL(currentIndexChanged(int)), this, SLOT(typeChanged(int)));
    layout->addWidget(type);

    objectList = new QListWidget(this);
    connect(objectList, SIGNAL(currentRowChanged(int)), this, SLOT(objectListIndexChanged(int)));
    layout->addWidget(objectList);


    toolbar = new QToolBar(this);

    QString iconsPath(QCoreApplication::applicationDirPath() + "/goombatlas_data/icons/");

    QAction* addObjectBtn = new QAction(QIcon(iconsPath + "add.png"), "Add Object", this);
    connect(addObjectBtn, SIGNAL(triggered()), this, SLOT(addObjectClicked()));
    toolbar->addAction(addObjectBtn);

    QAction* removeObjectBtn = new QAction(QIcon(iconsPath + "remove.png"), "Remove Object", this);
    connect(removeObjectBtn, SIGNAL(triggered()), this, SLOT(removeObjectClicked()));
    toolbar->addAction(removeObjectBtn);

    QAction* moveObjectUpBtn = new QAction(QIcon(iconsPath + "move_up.png"), "Move Up", this);
    connect(moveObjectUpBtn, SIGNAL(triggered()), this, SLOT(moveUpClicked()));
    toolbar->addAction(moveObjectUpBtn);

    QAction* moveObjectDownBtn = new QAction(QIcon(iconsPath + "move_down.png"), "Move Down", this);
    connect(moveObjectDownBtn, SIGNAL(triggered()), this, SLOT(moveDownClicked()));
    toolbar->addAction(moveObjectDownBtn);

    layout->addWidget(toolbar);

    layout->addWidget(new HorLine());


    QGridLayout* editsLayout = new QGridLayout();

    editsLayout->addWidget(new QLabel("Node ID:"), 0, 0, 1, 1, Qt::AlignRight);
    nodeId = new QSpinBox(this);
    nodeId->setRange(0, 255);
    nodeId->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    connect(nodeId, SIGNAL(valueChanged(int)), this, SLOT(nodeIdChanged(int)));
    editsLayout->addWidget(nodeId, 0, 1);

    editsLayout->addWidget(new QLabel("X:"), 1, 0, 1, 1, Qt::AlignRight);
    xPos = new QSpinBox(this);
    xPos->setRange(-0x80000, 0x7FFFF);
    connect(xPos, SIGNAL(valueChanged(int)), this, SLOT(xChanged(int)));
    editsLayout->addWidget(xPos, 1, 1);

    editsLayout->addWidget(new QLabel("Y:"), 2, 0, 1, 1, Qt::AlignRight);
    yPos = new QSpinBox(this);
    yPos->setRange(-0x80000, 0x7FFFF);
    connect(yPos, SIGNAL(valueChanged(int)), this, SLOT(yChanged(int)));
    editsLayout->addWidget(yPos, 2, 1);

    editsLayout->addWidget(new QLabel("Z:"), 3, 0, 1, 1, Qt::AlignRight);
    zPos = new QSpinBox(this);
    zPos->setRange(-0x80000, 0x7FFFF);
    connect(zPos, SIGNAL(valueChanged(int)), this, SLOT(zChanged(int)));
    editsLayout->addWidget(zPos, 3, 1);

    layout->addLayout(editsLayout);


    setLayout(layout);

    typeChanged(0);
}


void MapObjectEditor::select(MapObject* object)
{
    _select(object);
    updateList();
}

void MapObjectEditor::_select(MapObject* object)
{
    type->setCurrentIndex(object->getType());

    editObject = object;
    setEditsEnabled(true);
    objectList->blockSignals(true);
    objectList->setCurrentRow(editList->indexOf(object));
    objectList->blockSignals(false);
    updateInfo();
    emit objectSelected(dynamic_cast<MovableObject*>(object));
}

void MapObjectEditor::_deselect()
{
    deselect();
    emit objectDeselected();
}

void MapObjectEditor::deselect()
{
    objectList->selectionModel()->clear();
    setEditsEnabled(false);
    clearValues();
}


void MapObjectEditor::setMap(Map* map)
{
    this->map = map;
    setEditsEnabled(false);
    objectList->blockSignals(true);
    objectList->setCurrentRow(0);
    type->setCurrentIndex(0);
    typeChanged(0);
    updateList();
    objectList->blockSignals(false);
}


void MapObjectEditor::typeChanged(int typeId)
{
    deselect();

    if (typeId == 1)
        editList = &map->towersCastles;
    else if (typeId == 2)
        editList = &map->mushroomHouses;
    else
        editList = &map->starCoinSigns;

    updateList();
}

void MapObjectEditor::updateList(bool keepIndex)
{
    objectList->blockSignals(true);

    int index = objectList->currentRow();

    objectList->clear();

    QString objetName;

    if (type->currentIndex() == 1)
        objetName = "Tower/Castle";
    else if (type->currentIndex() == 2)
        objetName = "Mushroom House";
    else
        objetName = "Star Coin Sign";

    for (int i=0; i < editList->length(); i++)
    {
        MapObject* o = editList->at(i);
        objectList->addItem(QString("%1 %2 (%3/%4/%5)").arg(objetName).arg(i).arg(o->getx()).arg(o->gety()).arg(o->getz()));
    }

    if (keepIndex)
        objectList->setCurrentRow(index);
    objectList->blockSignals(false);
}

void MapObjectEditor::setEditsEnabled(bool enabled)
{
    nodeId->setEnabled(enabled);
    xPos->setEnabled(enabled);
    yPos->setEnabled(enabled);
    zPos->setEnabled(enabled);
}

void MapObjectEditor::updateInfo()
{
    allowChanges(false);
    nodeId->setValue(editObject->getNodeId());
    xPos->setValue(editObject->getx());
    yPos->setValue(editObject->gety());
    zPos->setValue(editObject->getz());
    allowChanges(true);
}

void MapObjectEditor::allowChanges(bool allow)
{
    nodeId->blockSignals(!allow);
    xPos->blockSignals(!allow);
    yPos->blockSignals(!allow);
    zPos->blockSignals(!allow);
}

void MapObjectEditor::clearValues()
{
    allowChanges(false);
    nodeId->setValue(0);
    xPos->setValue(0);
    yPos->setValue(0);
    zPos->setValue(0);
    allowChanges(true);
}


// Slots

void MapObjectEditor::objectListIndexChanged(int index)
{
    if (index == -1)
        return;

    _select(editList->at(index));
}

void MapObjectEditor::addObjectClicked()
{
    MapObject* newObj = new MapObject(type->currentIndex());
    QPoint currPos = mapView->getCurrentPos();
    newObj->setx(currPos.x());
    newObj->setz(currPos.y());

    if (objectList->currentIndex().row() == -1)
        editList->append(newObj);
    else
        editList->insert(objectList->currentIndex().row() + 1, newObj);

    updateList();

    objectList->setCurrentRow(editList->indexOf(newObj));

    emit redrawMap();
}

void MapObjectEditor::removeObjectClicked()
{
    if (objectList->currentIndex().row() == -1)
        return;

    editList->removeAt(objectList->currentIndex().row());

    updateList();

    _deselect();

    emit redrawMap();
}

void MapObjectEditor::moveUpClicked()
{
    int currentIndex = objectList->currentIndex().row();

    if (currentIndex < 1)
        return;

    MapObject* selMapObj = editList->at(currentIndex);
    editList->move(editList->indexOf(selMapObj), editList->indexOf(selMapObj)-1);
    updateList();
    _select(selMapObj);
}

void MapObjectEditor::moveDownClicked()
{
    int currentIndex = objectList->currentIndex().row();

    if (currentIndex == -1 || currentIndex > editList->length()-2)
        return;

    MapObject* selMapObj = editList->at(currentIndex);
    editList->move(editList->indexOf(selMapObj), editList->indexOf(selMapObj)+1);
    updateList();
    _select(selMapObj);
}

void MapObjectEditor::nodeIdChanged(int nodeId)
{
    editObject->setNodeId(nodeId);
}

void MapObjectEditor::xChanged(int x)
{
    editObject->setx(x);
    updateList();
    emit redrawMap();
}

void MapObjectEditor::yChanged(int y)
{
    editObject->sety(y);
    updateList();
}

void MapObjectEditor::zChanged(int z)
{
    editObject->setz(z);
    updateList();
    emit redrawMap();
}
