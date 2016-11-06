#include "nodeeditorwidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QCoreApplication>
#include <QToolBar>
#include <QAction>

#include <QDebug>

NodeEditorWidget::NodeEditorWidget(Map* map, QWidget* parent) : QWidget(parent)
{
    this->map = map;

    QString iconsPath(QCoreApplication::applicationDirPath() + "/goombatlas_data/icons/");


    QVBoxLayout* layout = new QVBoxLayout();


    nodeList = new QListWidget(this);
    connect(nodeList, SIGNAL(currentRowChanged(int)), this, SLOT(nodeListIndexChanged(int)));
    layout->addWidget(nodeList);


    QToolBar* buttonsBar = new QToolBar(this);

    QAction* addNodeBtn = new QAction(QIcon(iconsPath + "node_add.png"), "Add Node", this);
    connect(addNodeBtn, SIGNAL(triggered()), this, SLOT(addNodeClicked()));
    buttonsBar->addAction(addNodeBtn);

    QAction* removeNodeBtn = new QAction(QIcon(iconsPath + "node_remove.png"), "Remove Node", this);
    connect(removeNodeBtn, SIGNAL(triggered()), this, SLOT(removeNodeClicked()));
    buttonsBar->addAction(removeNodeBtn);

    QAction* moveNodeUpBtn = new QAction(QIcon(iconsPath + "move_up.png"), "Move Up", this);
    connect(moveNodeUpBtn, SIGNAL(triggered()), this, SLOT(moveUpClicked()));
    buttonsBar->addAction(moveNodeUpBtn);

    QAction* moveNodeDownBtn = new QAction(QIcon(iconsPath + "move_down.png"), "Move Down", this);
    connect(moveNodeDownBtn, SIGNAL(triggered()), this, SLOT(moveDownClicked()));
    buttonsBar->addAction(moveNodeDownBtn);

    layout->addWidget(buttonsBar);


    layout->addWidget(new HorLine());


    QGridLayout* editsLayout = new QGridLayout();

    editsLayout->addWidget(new QLabel("X:"), 0, 0, 1, 1, Qt::AlignRight);
    xPos = new QSpinBox(this);
    xPos->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    xPos->setRange(-2147483647-1, 2147483647);
    connect(xPos, SIGNAL(valueChanged(int)), this, SLOT(xChanged(int)));
    editsLayout->addWidget(xPos, 0, 1);

    editsLayout->addWidget(new QLabel("Y:"), 1, 0, 1, 1, Qt::AlignRight);
    yPos = new QSpinBox(this);
    yPos->setRange(-2147483647-1, 2147483647);
    connect(yPos, SIGNAL(valueChanged(int)), this, SLOT(yChanged(int)));
    editsLayout->addWidget(yPos, 1, 1);

    editsLayout->addWidget(new QLabel("Z:"), 2, 0, 1, 1, Qt::AlignRight);
    zPos = new QSpinBox(this);
    zPos->setRange(-2147483647-1, 2147483647);
    connect(zPos, SIGNAL(valueChanged(int)), this, SLOT(zChanged(int)));
    editsLayout->addWidget(zPos, 2, 1);

    editsLayout->addWidget(new QLabel("Area ID:"), 3, 0, 1, 1, Qt::AlignRight);
    areaId = new QSpinBox(this);
    areaId->setRange(0, 255);
    connect(areaId, SIGNAL(valueChanged(int)), this, SLOT(areaIdChnaged(int)));
    editsLayout->addWidget(areaId, 3, 1);

    editsLayout->addWidget(new QLabel("Icon:"), 4, 0, 1, 1, Qt::AlignRight);
    iconId = new QSpinBox(this);
    iconId->setRange(0, 255);
    connect(iconId, SIGNAL(valueChanged(int)), this, SLOT(iconIdChanged(int)));
    editsLayout->addWidget(iconId, 4, 1);

    layout->addLayout(editsLayout);


    this->setLayout(layout);

    this->setMinimumWidth(200);

    setEditsEnabled(false);
    updateList();
}

void NodeEditorWidget::select(Node* node)
{
    editNode = node;
    setEditsEnabled(true);
    nodeList->blockSignals(true);
    nodeList->setCurrentRow(map->nodes.indexOf(node));
    nodeList->blockSignals(false);
    updateInfo();
}

void NodeEditorWidget::deselect()
{
    nodeList->setCurrentRow(-1);
    setEditsEnabled(false);
    zeroValues();
}

void NodeEditorWidget::setMap(Map* map)
{
    this->map = map;
    setEditsEnabled(false);
    nodeList->blockSignals(true);
    nodeList->setCurrentRow(0);
    updateList();
    nodeList->blockSignals(false);
}

void NodeEditorWidget::updateList()
{
    nodeList->blockSignals(true);

    int index = nodeList->currentRow();

    nodeList->clear();

    for (int i=0; i < map->nodes.length(); i++)
    {
        Node* n = map->nodes.at(i);
        nodeList->addItem(QString("Node %1 (%2/%3/%4)").arg(i).arg(n->getx()).arg(n->gety()).arg(n->getz()));
    }

    nodeList->setCurrentRow(index);

    nodeList->blockSignals(false);
}

void NodeEditorWidget::setEditsEnabled(bool enabled)
{
    xPos->setEnabled(enabled);
    yPos->setEnabled(enabled);
    zPos->setEnabled(enabled);
    areaId->setEnabled(enabled);
    iconId->setEnabled(enabled);
}

void NodeEditorWidget::updateInfo()
{
    allowChanges(false);
    xPos->setValue(editNode->getx());
    yPos->setValue(editNode->gety());
    zPos->setValue(editNode->getz());
    areaId->setValue(editNode->getAreaId());
    iconId->setValue(editNode->getIconId());
    allowChanges(true);
}

void NodeEditorWidget::allowChanges(bool allow)
{
    xPos->blockSignals(!allow);
    yPos->blockSignals(!allow);
    zPos->blockSignals(!allow);
    areaId->blockSignals(!allow);
    iconId->blockSignals(!allow);
}

void NodeEditorWidget::zeroValues()
{
    allowChanges(false);
    xPos->setValue(0);
    yPos->setValue(0);
    zPos->setValue(0);
    areaId->setValue(0);
    iconId->setValue(0);
    allowChanges(true);
}


// Slots

void NodeEditorWidget::nodeListIndexChanged(int index)
{
    if (index == -1)
        return;

    select(map->getNodePtr(index));
}

void NodeEditorWidget::addNodeClicked()
{
    Node* newNode = new Node(0, 0, 0);

    int insertIndex = -1;
    if (nodeList->currentIndex().row() != -1)
        insertIndex = nodeList->currentIndex().row() + 1;

    map->addNode(newNode, insertIndex);

    updateList();

    nodeList->setCurrentRow(map->nodes.indexOf(newNode));
}

void NodeEditorWidget::moveUpClicked()
{
    int currentIndex = nodeList->currentIndex().row();

    if (currentIndex == -1)
        return;

    Node* selNode = map->getNodePtr(currentIndex);
    map->moveNodeUp(currentIndex);
    updateList();
    select(selNode);
}

void NodeEditorWidget::moveDownClicked()
{
    int currentIndex = nodeList->currentIndex().row();

    if (currentIndex == -1)
        return;

    Node* selNode = map->getNodePtr(currentIndex);
    map->moveNodeDown(currentIndex);
    updateList();
    select(selNode);
}

void NodeEditorWidget::removeNodeClicked()
{
    if (nodeList->currentIndex().row() == -1)
        return;

    map->removeNode(nodeList->currentIndex().row());

    updateList();

    deselect();
}

void NodeEditorWidget::xChanged(int x)
{
    editNode->setx(x);
    updateList();
}

void NodeEditorWidget::yChanged(int y)
{
    editNode->sety(y);
    updateList();
}

void NodeEditorWidget::zChanged(int z)
{
    editNode->setz(z);
    updateList();
}

void NodeEditorWidget::areaIdChnaged(int areaId)
{
    editNode->setAreaId((quint8)areaId);
    updateList();
}

void NodeEditorWidget::iconIdChanged(int iconId)
{
    editNode->setIconId((quint8)iconId);
    updateList();
}
