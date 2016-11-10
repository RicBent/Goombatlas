#include "nodeeditorwidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QCoreApplication>
#include <QAction>
#include <QTabWidget>
#include <QSpacerItem>

#include <QDebug>

NodeEditorWidget::NodeEditorWidget(Map* map, MapView *mapView, QWidget* parent) : QWidget(parent)
{
    this->map = map;
    this->mapView = mapView;

    QString iconsPath(QCoreApplication::applicationDirPath() + "/goombatlas_data/icons/");


    QVBoxLayout* layout = new QVBoxLayout();


    nodeList = new QListWidget(this);
    nodeList->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
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


    QTabWidget* tabWidget = new QTabWidget(this);
    tabWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    layout->addWidget(tabWidget);


    QGridLayout* editsLayout = new QGridLayout();
    QWidget* settingsTab = new QWidget(this);
    settingsTab->setLayout(editsLayout);
    tabWidget->addTab(settingsTab, "Settings");

    editsLayout->addWidget(new QLabel("X:"), 0, 0, 1, 1, Qt::AlignRight);
    xPos = new QSpinBox(this);
    xPos->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    xPos->setRange(-2147483647-1, 2147483647);
    connect(xPos, SIGNAL(valueChanged(int)), this, SLOT(xChanged(int)));
    editsLayout->addWidget(xPos, 0, 1, 1, 2);

    editsLayout->addWidget(new QLabel("Y:"), 1, 0, 1, 1, Qt::AlignRight);
    yPos = new QSpinBox(this);
    yPos->setRange(-2147483647-1, 2147483647);
    connect(yPos, SIGNAL(valueChanged(int)), this, SLOT(yChanged(int)));
    editsLayout->addWidget(yPos, 1, 1, 1, 2);

    editsLayout->addWidget(new QLabel("Z:"), 2, 0, 1, 1, Qt::AlignRight);
    zPos = new QSpinBox(this);
    zPos->setRange(-2147483647-1, 2147483647);
    connect(zPos, SIGNAL(valueChanged(int)), this, SLOT(zChanged(int)));
    editsLayout->addWidget(zPos, 2, 1, 1, 2);

    editsLayout->addWidget(new HorLine(), 3, 0, 1, 3);

    editsLayout->addWidget(new QLabel("Area ID:"), 4, 0, 1, 1, Qt::AlignRight);
    areaId = new QSpinBox(this);
    areaId->setRange(0, 255);
    connect(areaId, SIGNAL(valueChanged(int)), this, SLOT(areaIdChnaged(int)));
    editsLayout->addWidget(areaId, 4, 1, 1, 2);

    editsLayout->addWidget(new QLabel("Icon:"), 5, 0, 1, 1, Qt::AlignRight);
    iconId = new QSpinBox(this);
    iconId->setRange(0, 255);
    iconId->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    connect(iconId, SIGNAL(valueChanged(int)), this, SLOT(iconIdChanged(int)));
    editsLayout->addWidget(iconId, 5, 1, 1, 1);

    icon = new QComboBox(this);
    icon->setMinimumWidth(40);
    icon->setMaximumWidth(40);
    loadLevelIcons();
    icon->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
    connect(icon, SIGNAL(currentIndexChanged(int)), this, SLOT(iconChanged(int)));
    editsLayout->addWidget(icon, 5, 2, 1, 1);

    editsLayout->addWidget(new QLabel("Bowser Jr. Path:"), 6, 0, 1, 1, Qt::AlignRight);
    bowserJrPath = new QComboBox(this);
    bowserJrPath->addItem("0");
    bowserJrPath->addItem("1");
    bowserJrPath->addItem("2");
    bowserJrPath->addItem("3");
    bowserJrPath->addItem("4");
    connect(bowserJrPath, SIGNAL(currentIndexChanged(int)), this, SLOT(bowserJrPathChanged(int)));
    editsLayout->addWidget(bowserJrPath, 6, 1, 1, 2);


    nodeVisible = new QCheckBox("Node Visible", this);
    connect(nodeVisible, SIGNAL(toggled(bool)), this, SLOT(nodeVisibleToggled(bool)));
    editsLayout->addWidget(nodeVisible, 7, 1, 1, 2);

    hasStarCoins = new QCheckBox("Has Star Coins", this);
    connect(hasStarCoins, SIGNAL(toggled(bool)), this, SLOT(hasStarCoinsToggled(bool)));
    editsLayout->addWidget(hasStarCoins, 8, 1, 1, 2);

    goToNextWorld = new QCheckBox("Go To Next World", this);
    connect(goToNextWorld, SIGNAL(toggled(bool)), this, SLOT(goToNextWorldToggled(bool)));
    editsLayout->addWidget(goToNextWorld, 9, 1, 1, 2);

    unknownBit = new QCheckBox("Unknown Bit 7", this);
    connect(unknownBit, SIGNAL(toggled(bool)), this, SLOT(unknownBitToggled(bool)));
    editsLayout->addWidget(unknownBit, 10, 1, 1, 2);

    /*QWidget* spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
    editsLayout->addWidget(spacer, 11, 0, 1, 3);*/


    QVBoxLayout* pathsLayout = new QVBoxLayout();
    QWidget* pathsTab = new QWidget(this);
    pathsTab->setLayout(pathsLayout);
    tabWidget->addTab(pathsTab, "Paths");

    pathList = new QListWidget(this);
    pathList->setMaximumHeight(100);
    pathList->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    connect(pathList, SIGNAL(currentRowChanged(int)), this, SLOT(pathListIndexChanged(int)));
    pathsLayout->addWidget(pathList);

    pButtonsBar = new QToolBar(this);

    QAction* addPathBtn = new QAction(QIcon(iconsPath + "path_add.png"), "Add Path", this);
    connect(addPathBtn, SIGNAL(triggered()), this, SLOT(addPathClicked()));
    pButtonsBar->addAction(addPathBtn);

    QAction* removePathBtn = new QAction(QIcon(iconsPath + "path_remove.png"), "Remove Path", this);
    connect(removePathBtn, SIGNAL(triggered()), this, SLOT(removePathClicked()));
    pButtonsBar->addAction(removePathBtn);

    QAction* movePathUpBtn = new QAction(QIcon(iconsPath + "move_up.png"), "Move Up", this);
    connect(movePathUpBtn, SIGNAL(triggered()), this, SLOT(movePathUpClicked()));
    pButtonsBar->addAction(movePathUpBtn);

    QAction* movePathDownBtn = new QAction(QIcon(iconsPath + "move_down.png"), "Move Down", this);
    connect(movePathDownBtn, SIGNAL(triggered()), this, SLOT(movePathDownClicked()));
    pButtonsBar->addAction(movePathDownBtn);

    pathsLayout->addWidget(pButtonsBar);

    pathsLayout->addWidget(new HorLine());


    QGridLayout* pathEditsLayout = new QGridLayout();

    pathEditsLayout->addWidget(new QLabel("Ending Node:"), 0, 0, 1, 1, Qt::AlignRight);
    endingNodeId = new QSpinBox(this);
    endingNodeId->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    endingNodeId->setRange(0, 255);
    connect(endingNodeId, SIGNAL(valueChanged(int)), this, SLOT(endingNodeIdChanged(int)));
    pathEditsLayout->addWidget(endingNodeId, 0, 1, 1, 1);

    pathEditsLayout->addWidget(new QLabel("Path Setting ID:"), 1, 0, 1, 1, Qt::AlignRight);
    pathSettingId = new QSpinBox(this);
    pathSettingId->setRange(0, 255);
    connect(pathSettingId, SIGNAL(valueChanged(int)), this, SLOT(pathSettingIdChanged(int)));
    pathEditsLayout->addWidget(pathSettingId, 1, 1, 1, 1);

    pathEditsLayout->addWidget(new QLabel("D-pad Direction:"), 2, 0, 1, 1, Qt::AlignRight);
    direction = new QComboBox(this);
    direction->addItem("Down");
    direction->addItem("Right");
    direction->addItem("Up");
    direction->addItem("Left");
    connect(direction, SIGNAL(currentIndexChanged(int)), this, SLOT(directionChanged(int)));
    pathEditsLayout->addWidget(direction, 2, 1, 1, 1);

    reversePathAnim = new QCheckBox("Reverse Path Animation", this);
    connect(reversePathAnim, SIGNAL(toggled(bool)), this, SLOT(reversePathAnimToggled(bool)));
    pathEditsLayout->addWidget(reversePathAnim, 3, 1, 1, 1);

    pathsLayout->addLayout(pathEditsLayout);


    this->setLayout(layout);

    this->setMinimumWidth(250);

    setEditsEnabled(false);
    clearValues();
    updateList();
    pSetEditsEnabled(false);
    pClearValues();
}

void NodeEditorWidget::select(Node* node)
{
    updateList();
    _select(node);
}

void NodeEditorWidget::_select(Node* node)
{
    editNode = node;
    setEditsEnabled(true);
    nodeList->blockSignals(true);
    nodeList->setCurrentRow(map->nodes.indexOf(node));
    nodeList->blockSignals(false);
    updateInfo();
    emit nodeSelected(node);

    pUpdateList();
    pDeselect();
    pClearValues();
    pathList->blockSignals(true);
    pathList->setCurrentRow(-1);
    pathList->blockSignals(false);
    pButtonsBar->setEnabled(true);
}

void NodeEditorWidget::deselect()
{
    nodeList->selectionModel()->clear();
    setEditsEnabled(false);
    clearValues();

    pathList->clear();
    pDeselect();

    emit nodeDeselected();
}

void NodeEditorWidget::setMap(Map* map)
{
    deselect();
    this->map = map;
    setEditsEnabled(false);
    nodeList->blockSignals(true);
    nodeList->setCurrentRow(-1);
    updateList();
    nodeList->blockSignals(false);
    pathList->clear();
    pDeselect();
}

void NodeEditorWidget::updateList(bool keepIndex)
{
    nodeList->blockSignals(true);

    int index = nodeList->currentRow();

    nodeList->clear();

    for (int i=0; i < map->nodes.length(); i++)
    {
        Node* n = map->nodes.at(i);
        nodeList->addItem(QString("Node %1 (%2/%3/%4)").arg(i).arg(n->getx()).arg(n->gety()).arg(n->getz()));
    }

    if (keepIndex)
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
    icon->setEnabled(enabled);
    bowserJrPath->setEnabled(enabled);
    nodeVisible->setEnabled(enabled);
    hasStarCoins->setEnabled(enabled);
    goToNextWorld->setEnabled(enabled);
    unknownBit->setEnabled(enabled);
}

void NodeEditorWidget::updateInfo()
{
    allowChanges(false);
    xPos->setValue(editNode->getx());
    yPos->setValue(editNode->gety());
    zPos->setValue(editNode->getz());
    areaId->setValue(editNode->getAreaId());
    iconId->setValue(editNode->getIconId());
    icon->setCurrentIndex(iconIndexes.value(editNode->getIconId(), -1));
    bowserJrPath->setCurrentIndex(editNode->getBowserJrPath());
    nodeVisible->setChecked(editNode->getSetting(3));
    hasStarCoins->setChecked(editNode->getSetting(0));
    goToNextWorld->setChecked(editNode->getSetting(4));
    unknownBit->setChecked(editNode->getSetting(7));
    allowChanges(true);
}

void NodeEditorWidget::allowChanges(bool allow)
{
    xPos->blockSignals(!allow);
    yPos->blockSignals(!allow);
    zPos->blockSignals(!allow);
    areaId->blockSignals(!allow);
    iconId->blockSignals(!allow);
    icon->blockSignals(!allow);
    bowserJrPath->blockSignals(!allow);
    nodeVisible->blockSignals(!allow);
    hasStarCoins->blockSignals(!allow);
    goToNextWorld->blockSignals(!allow);
    unknownBit->blockSignals(!allow);
}

void NodeEditorWidget::clearValues()
{
    allowChanges(false);
    xPos->setValue(0);
    yPos->setValue(0);
    zPos->setValue(0);
    areaId->setValue(0);
    iconId->setValue(0);
    icon->setCurrentIndex(-1);
    bowserJrPath->setCurrentIndex(-1);
    bowserJrPath->setCurrentIndex(-1);
    nodeVisible->setChecked(false);
    hasStarCoins->setChecked(false);
    goToNextWorld->setChecked(false);
    unknownBit->setChecked(false);
    allowChanges(true);
}

void NodeEditorWidget::loadLevelIcons()
{
    QDir iconDir(QCoreApplication::applicationDirPath() + "/goombatlas_data/level_icons");

    QFileInfoList iconList = iconDir.entryInfoList(QDir::Files);

    QHash<quint8, QIcon> tempIcons;
    QList<quint8> iconNbrs;

    for (int i = 0; i < iconList.size(); ++i)
    {
        QFileInfo fileInfo = iconList.at(i);
        QStringList splitted = fileInfo.fileName().split(".");
        if (splitted.length() == 2 && splitted.at(1) == "png")
        {
            bool ok = false;
            uint index = splitted.at(0).toUInt(&ok);

            if (!ok || index > 255)
                continue;

            iconNbrs.append(index);
        }
    }

    qSort(iconNbrs);

    for (int i = 0; i < iconNbrs.length(); i++)
    {
        icon->addItem(QIcon(QString("%1/%2.png").arg(iconDir.path()).arg(iconNbrs[i])), "", iconNbrs[i]);
        iconIndexes.insert(iconNbrs[i], i);
    }
}


// Path edits

void NodeEditorWidget::pSelect(Path* p)
{
    editPath = p;
    pUpdateInfo();
    pSetEditsEnabled(true);

    pathList->blockSignals(true);
    pathList->setCurrentRow(editNode->pathList()->indexOf(p));
    pathList->blockSignals(false);
}

void NodeEditorWidget::pDeselect()
{
    pSetEditsEnabled(false);
    pClearValues();
}

void NodeEditorWidget::pUpdateList(bool keepIndex)
{
    pathList->blockSignals(true);

    int index = pathList->currentRow();

    pathList->clear();

    for (int i=0; i < editNode->pathList()->length(); i++)
    {
        pathList->addItem(QString("Path %1").arg(i));
    }

    if (keepIndex)
        pathList->setCurrentRow(index);
    pathList->blockSignals(false);
}

void NodeEditorWidget::pSetEditsEnabled(bool enabled)
{
    endingNodeId->setEnabled(enabled);
    pathSettingId->setEnabled(enabled);
    direction->setEnabled(enabled);
    reversePathAnim->setEnabled(enabled);
    pButtonsBar->setEnabled(enabled);
}

void NodeEditorWidget::pUpdateInfo()
{
    pAllowChanges(false);
    endingNodeId->setValue(editPath->endingNodeId);
    pathSettingId->setValue(editPath->animationId);
    direction->setCurrentIndex(editPath->settings & 3);
    reversePathAnim->setChecked(editPath->settings >> 6 & 1);
    pAllowChanges(true);
}

void NodeEditorWidget::pAllowChanges(bool allow)
{
    endingNodeId->blockSignals(!allow);
    pathSettingId->blockSignals(!allow);
    direction->blockSignals(!allow);
    reversePathAnim->blockSignals(!allow);
}

void NodeEditorWidget::pClearValues()
{
    pAllowChanges(false);
    endingNodeId->setValue(0);
    pathSettingId->setValue(0);
    direction->setCurrentIndex(-1);
    reversePathAnim->setChecked(false);
    pAllowChanges(true);
}


// Slots

void NodeEditorWidget::nodeListIndexChanged(int index)
{
    if (index == -1)
        return;

    _select(map->getNodePtr(index));
}

void NodeEditorWidget::addNodeClicked()
{
    QPoint currPos = mapView->getCurrentPos();

    Node* newNode = new Node(currPos.x(), 0, currPos.y());

    int insertIndex = -1;
    if (nodeList->currentIndex().row() != -1)
        insertIndex = nodeList->currentIndex().row() + 1;

    map->addNode(newNode, insertIndex);
    updateList();
    nodeList->setCurrentRow(map->nodes.indexOf(newNode));
    emit redrawMap();
}

void NodeEditorWidget::moveUpClicked()
{
    int currentIndex = nodeList->currentIndex().row();

    if (currentIndex == -1)
        return;

    Node* selNode = map->getNodePtr(currentIndex);
    map->moveNodeUp(currentIndex);
    updateList();
    _select(selNode);
    emit redrawMap();
}

void NodeEditorWidget::moveDownClicked()
{
    int currentIndex = nodeList->currentIndex().row();

    if (currentIndex == -1)
        return;

    Node* selNode = map->getNodePtr(currentIndex);
    map->moveNodeDown(currentIndex);
    updateList();
    _select(selNode);
    emit redrawMap();
}

void NodeEditorWidget::removeNodeClicked()
{
    if (nodeList->currentIndex().row() == -1)
        return;

    map->removeNode(nodeList->currentIndex().row());
    updateList();
    deselect();
    emit redrawMap();
}

void NodeEditorWidget::xChanged(int x)
{
    editNode->setx(x);
    updateList();
    emit redrawMap();
}

void NodeEditorWidget::yChanged(int y)
{
    editNode->sety(y);
    updateList();
    emit redrawMap();
}

void NodeEditorWidget::zChanged(int z)
{
    editNode->setz(z);
    updateList();
    emit redrawMap();
}

void NodeEditorWidget::areaIdChnaged(int areaId)
{
    editNode->setAreaId((quint8)areaId);
    updateList();
    emit redrawMap();
}

void NodeEditorWidget::iconIdChanged(int iconId)
{
    editNode->setIconId((quint8)iconId);
    icon->blockSignals(true);
    icon->setCurrentIndex(iconIndexes.value(editNode->getIconId(), -1));
    icon->blockSignals(false);
    updateList();
    emit redrawMap();
}

void NodeEditorWidget::iconChanged(int iconIndex)
{
    iconId->setValue(iconIndexes.key(iconIndex));
}

void NodeEditorWidget::bowserJrPathChanged(int pathIndex)
{
    editNode->setBowserJrPath(pathIndex);
    updateList();
    emit redrawMap();
}

void NodeEditorWidget::nodeVisibleToggled(bool toggle)
{
    editNode->setSetting(toggle, 3);
    updateList();
    emit redrawMap();
}

void NodeEditorWidget::hasStarCoinsToggled(bool toggle)
{
    editNode->setSetting(toggle, 0);
    updateList();
    emit redrawMap();
}

void NodeEditorWidget::goToNextWorldToggled(bool toggle)
{
    editNode->setSetting(toggle, 4);
    updateList();
    emit redrawMap();
}

void NodeEditorWidget::unknownBitToggled(bool toggle)
{
    editNode->setSetting(toggle, 7);
    updateList();
    emit redrawMap();
}


// Slots Path Settings

void NodeEditorWidget::pathListIndexChanged(int index)
{
    if (index == -1)
        return;

    pSelect(editNode->pathList()->at(index));
}

void NodeEditorWidget::endingNodeIdChanged(int endingNodeId)
{
    editPath->endingNodeId = endingNodeId;
    emit redrawMap();
}

void NodeEditorWidget::pathSettingIdChanged(int pathSettingId)
{
    editPath->animationId = pathSettingId;
}

void NodeEditorWidget::directionChanged(int direction)
{
    editPath->settings &= 0xFC;
    editPath->settings |= (direction & 3);
    emit redrawMap();
}

void NodeEditorWidget::reversePathAnimToggled(bool toggle)
{
    editPath->settings &= 0xBF;
    editPath->settings |= (toggle << 6);
}

void NodeEditorWidget::addPathClicked()
{
    if (editNode->pathList()->length() >= 4)
        return;

    Path* newPath = new Path();
    newPath->animationId = 0;
    newPath->endingNodeId = 0;
    newPath->settings = 0;

    if (pathList->currentIndex().row() != -1)
        editNode->pathList()->insert(pathList->currentIndex().row() + 1, newPath);
    else
        editNode->pathList()->append(newPath);

    pUpdateList();
    pSelect(newPath);

    emit redrawMap();
}

void NodeEditorWidget::movePathUpClicked()
{
    int currentIndex = pathList->currentIndex().row();

    if (currentIndex < 1)
        return;

    editNode->pathList()->move(currentIndex, currentIndex - 1);
    pathList->setCurrentRow(currentIndex - 1);
}

void NodeEditorWidget::movePathDownClicked()
{
    int currentIndex = pathList->currentIndex().row();

    if (currentIndex == -1)
        return;

    if (currentIndex > editNode->pathList()->length()-2)
        return;

    editNode->pathList()->move(currentIndex, currentIndex + 1);
    pathList->setCurrentRow(currentIndex + 1);
}

void NodeEditorWidget::removePathClicked()
{
    if (pathList->currentIndex().row() == -1)
        return;

    editNode->pathList()->removeAt(pathList->currentIndex().row());
    pUpdateList();
    pDeselect();
    emit redrawMap();
}
