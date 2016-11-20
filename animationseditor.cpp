#include "animationseditor.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QCoreApplication>
#include <QAction>

AnimationsEditor::AnimationsEditor(Map* map, MapView* mapView, QWidget* parent) : QWidget(parent)
{
    this->map = map;
    this->mapView = mapView;

    QString iconsPath(QCoreApplication::applicationDirPath() + "/goombatlas_data/icons/");


    QVBoxLayout* layout = new QVBoxLayout();


    pathsList = new QListWidget(this);
    pathsList->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(pathsList, SIGNAL(currentRowChanged(int)), this, SLOT(pathsListIndexChanged(int)));
    layout->addWidget(pathsList);


    QToolBar* buttonsBar = new QToolBar(this);

    QAction* addPathBtn = new QAction(QIcon(iconsPath + "add.png"), "Add Node", this);
    connect(addPathBtn, SIGNAL(triggered()), this, SLOT(addPathClicked()));
    buttonsBar->addAction(addPathBtn);

    QAction* removePathBtn = new QAction(QIcon(iconsPath + "remove.png"), "Remove Node", this);
    connect(removePathBtn, SIGNAL(triggered()), this, SLOT(removePathClicked()));
    buttonsBar->addAction(removePathBtn);

    QAction* movePathUpBtn = new QAction(QIcon(iconsPath + "move_up.png"), "Move Up", this);
    connect(movePathUpBtn, SIGNAL(triggered()), this, SLOT(movePathUpClicked()));
    buttonsBar->addAction(movePathUpBtn);

    QAction* movePathDownBtn = new QAction(QIcon(iconsPath + "move_down.png"), "Move Down", this);
    connect(movePathDownBtn, SIGNAL(triggered()), this, SLOT(movePathDownClicked()));
    buttonsBar->addAction(movePathDownBtn);

    layout->addWidget(buttonsBar);


    layout->addWidget(new HorLine());


    keyframeList = new QListWidget(this);
    keyframeList->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(keyframeList, SIGNAL(currentRowChanged(int)), this, SLOT(keyframeListIndexChanged(int)));
    layout->addWidget(keyframeList);


    kButtonsBar = new QToolBar(this);

    QAction* addKeyframeBtn = new QAction(QIcon(iconsPath + "keyframe_add.png"), "Add Node", this);
    connect(addKeyframeBtn, SIGNAL(triggered()), this, SLOT(addKeyframeClicked()));
    kButtonsBar->addAction(addKeyframeBtn);

    QAction* removeKeyframeBtn = new QAction(QIcon(iconsPath + "keyframe_remove.png"), "Remove Node", this);
    connect(removeKeyframeBtn, SIGNAL(triggered()), this, SLOT(removeKeyframeClicked()));
    kButtonsBar->addAction(removeKeyframeBtn);

    QAction* moveKeyframeUpBtn = new QAction(QIcon(iconsPath + "move_up.png"), "Move Up", this);
    connect(moveKeyframeUpBtn, SIGNAL(triggered()), this, SLOT(moveKeyframeUpClicked()));
    kButtonsBar->addAction(moveKeyframeUpBtn);

    QAction* moveKeyframeDownBtn = new QAction(QIcon(iconsPath + "move_down.png"), "Move Down", this);
    connect(moveKeyframeDownBtn, SIGNAL(triggered()), this, SLOT(moveKeyframeDownClicked()));
    kButtonsBar->addAction(moveKeyframeDownBtn);

    layout->addWidget(kButtonsBar);


    layout->addWidget(new HorLine());


    QGridLayout* editsLayout = new QGridLayout();

    editsLayout->addWidget(new QLabel("X:"), 0, 0, 1, 1, Qt::AlignRight);
    xPos = new QSpinBox(this);
    xPos->setRange(-0x80000, 0x7FFFF);
    xPos->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    connect(xPos, SIGNAL(valueChanged(int)), this, SLOT(xChanged(int)));
    editsLayout->addWidget(xPos, 0, 1);

    editsLayout->addWidget(new QLabel("Y:"), 1, 0, 1, 1, Qt::AlignRight);
    yPos = new QSpinBox(this);
    yPos->setRange(-0x80000, 0x7FFFF);
    connect(yPos, SIGNAL(valueChanged(int)), this, SLOT(yChanged(int)));
    editsLayout->addWidget(yPos, 1, 1);

    editsLayout->addWidget(new QLabel("Z:"), 2, 0, 1, 1, Qt::AlignRight);
    zPos = new QSpinBox(this);
    zPos->setRange(-0x80000, 0x7FFFF);
    connect(zPos, SIGNAL(valueChanged(int)), this, SLOT(zChanged(int)));
    editsLayout->addWidget(zPos, 2, 1);

    editsLayout->addWidget(new QLabel("Rotation:"), 3, 0, 1, 1, Qt::AlignRight);
    rot = new QSpinBox(this);
    rot->setRange(-0x80000, 0x7FFFF);
    connect(rot, SIGNAL(valueChanged(int)), this, SLOT(rotChanged(int)));
    editsLayout->addWidget(rot, 3, 1);

    editsLayout->addWidget(new QLabel("Pace:"), 4, 0, 1, 1, Qt::AlignRight);
    pace = new QSpinBox(this);
    pace->setRange(-0x80000, 0x7FFFF);
    connect(pace, SIGNAL(valueChanged(int)), this, SLOT(paceChanged(int)));
    editsLayout->addWidget(pace, 4, 1);


    layout->addLayout(editsLayout);


    this->setLayout(layout);

    this->setMinimumWidth(250);


    kSetEditsEnabled(false);
    kButtonsBar->setEnabled(false);
    kClearValues();
}

void AnimationsEditor::setMap(Map* map)
{
    deselect();
    this->map = map;
    pathsList->blockSignals(true);
    pathsList->setCurrentRow(-1);
    updateList();
    pathsList->blockSignals(false);
    keyframeList->clear();
}

void AnimationsEditor::select(AnimationPath* path)
{
    editPath = path;
    pathsList->blockSignals(true);
    pathsList->setCurrentRow(map->animationpaths.indexOf(path));
    pathsList->blockSignals(false);

    kUpdateList();
    _kDeselect();
    kClearValues();
    keyframeList->blockSignals(true);
    keyframeList->setCurrentRow(-1);
    keyframeList->blockSignals(false);
    kButtonsBar->setEnabled(true);

    mapView->setCurrentAnimPath(map->animationpaths.indexOf(path));
}

void AnimationsEditor::deselect()
{
    keyframeList->clear();
    kButtonsBar->setEnabled(false);
    _kDeselect();
    mapView->setCurrentAnimPath(-1);
    pathsList->blockSignals(true);
    pathsList->selectionModel()->clear();
    pathsList->blockSignals(false);
}

void AnimationsEditor::updateList(bool keepIndex)
{
    pathsList->blockSignals(true);

    int index = pathsList->currentRow();

    pathsList->clear();

    for (int i=0; i < map->animationpaths.length(); i++)
        pathsList->addItem(QString("Animation path %1").arg(i));

    if (keepIndex)
        pathsList->setCurrentRow(index);
    pathsList->blockSignals(false);
}


// Keyframe Edits

void AnimationsEditor::_kSelect(Keyframe* k)
{
    editKeyframe = k;
    kUpdateInfo();

    emit keyframeSelected(k);

    kSetEditsEnabled(true);
    keyframeList->blockSignals(true);
    keyframeList->setCurrentRow(editPath->keyframeList()->indexOf(k));
    keyframeList->blockSignals(false);
}

void AnimationsEditor::kSelect(Keyframe* k)
{
    _kSelect(k);
    kUpdateList();
}

void AnimationsEditor::_kDeselect()
{
    kDeselect();
    emit keyframeDeselected();
}

void AnimationsEditor::kDeselect()
{
    kSetEditsEnabled(false);
    kClearValues();
    keyframeList->blockSignals(true);
    keyframeList->selectionModel()->clear();
    keyframeList->blockSignals(false);
}

void AnimationsEditor::kUpdateList(bool keepIndex)
{
    keyframeList->blockSignals(true);

    int index = keyframeList->currentRow();

    keyframeList->clear();

    for (int i=0; i < editPath->keyframeList()->length(); i++)
    {
        Keyframe* k = editPath->keyframeList()->at(i);
        keyframeList->addItem(QString("Keyframes %1 (%2/%3/%4)").arg(i).arg(k->getx()).arg(k->gety()).arg(k->getz()));
    }

    if (keepIndex)
        keyframeList->setCurrentRow(index);
    keyframeList->blockSignals(false);
}

void AnimationsEditor::kSetEditsEnabled(bool enabled)
{
    xPos->setEnabled(enabled);
    yPos->setEnabled(enabled);
    zPos->setEnabled(enabled);
    rot->setEnabled(enabled);
    pace->setEnabled(enabled);
}

void AnimationsEditor::kUpdateInfo()
{
    kAllowChanges(false);
    xPos->setValue(editKeyframe->getx());
    yPos->setValue(editKeyframe->gety());
    zPos->setValue(editKeyframe->getz());
    rot->setValue(editKeyframe->getRot());
    pace->setValue(editKeyframe->getPace());
    kAllowChanges(true);
}

void AnimationsEditor::kAllowChanges(bool allow)
{
    xPos->blockSignals(!allow);
    yPos->blockSignals(!allow);
    zPos->blockSignals(!allow);
    rot->blockSignals(!allow);
    pace->blockSignals(!allow);
}

void AnimationsEditor::kClearValues()
{
    kAllowChanges(false);
    xPos->setValue(0);
    yPos->setValue(0);
    zPos->setValue(0);
    rot->setValue(0);
    pace->setValue(0);
    kAllowChanges(true);
}


// Path Slots

void AnimationsEditor::pathsListIndexChanged(int index)
{
    if (index == -1)
        return;

    select(map->animationpaths[index]);
}

void AnimationsEditor::addPathClicked()
{
    AnimationPath* newPath = new AnimationPath();

    if (pathsList->currentIndex().row() != -1)
        map->animationpaths.insert(pathsList->currentIndex().row() + 1, newPath);
    else
        map->animationpaths.append(newPath);

    updateList();
    pathsList->setCurrentRow(map->animationpaths.indexOf(newPath));
    emit redrawMap();
}

void AnimationsEditor::removePathClicked()
{
    if (pathsList->currentIndex().row() == -1)
        return;

    map->animationpaths.removeAt(pathsList->currentIndex().row());
    updateList();
    deselect();
    emit redrawMap();
}

void AnimationsEditor::movePathUpClicked()
{
    int currentIndex = pathsList->currentIndex().row();

    if (currentIndex < 1)
        return;

    map->animationpaths.move(currentIndex, currentIndex - 1);
    pathsList->setCurrentRow(currentIndex - 1);
}

void AnimationsEditor::movePathDownClicked()
{
    int currentIndex = pathsList->currentIndex().row();

    if (currentIndex == -1)
        return;

    if (currentIndex > map->animationpaths.length()-2)
        return;

    map->animationpaths.move(currentIndex, currentIndex + 1);
    pathsList->setCurrentRow(currentIndex + 1);
}


// Keyframe Slots

void AnimationsEditor::keyframeListIndexChanged(int index)
{
    if (index == -1)
        return;

    _kSelect(editPath->keyframeList()->at(index));
}

void AnimationsEditor::addKeyframeClicked()
{
    Keyframe* newKeyframe = new Keyframe();

    QPoint currPos = mapView->getCurrentPos();

    newKeyframe->setx(currPos.x());
    newKeyframe->sety(0);
    newKeyframe->setz(currPos.y());
    newKeyframe->setRot(0);
    newKeyframe->setPace(25);

    if (keyframeList->currentIndex().row() != -1)
        editPath->keyframeList()->insert(keyframeList->currentIndex().row() + 1, newKeyframe);
    else
        editPath->keyframeList()->append(newKeyframe);

    kUpdateList();
    kSelect(newKeyframe);

    emit redrawMap();
}

void AnimationsEditor::moveKeyframeUpClicked()
{
    int currentIndex = keyframeList->currentIndex().row();

    if (currentIndex < 1)
        return;

    Keyframe* cK = editPath->keyframeList()->at(currentIndex);
    editPath->keyframeList()->move(currentIndex, currentIndex - 1);
    kSelect(cK);
}

void AnimationsEditor::moveKeyframeDownClicked()
{
    int currentIndex = keyframeList->currentIndex().row();

    if (currentIndex == -1)
        return;

    if (currentIndex > editPath->keyframeList()->length()-2)
        return;

    Keyframe* cK = editPath->keyframeList()->at(currentIndex);
    editPath->keyframeList()->move(currentIndex, currentIndex + 1);
    kSelect(cK);
}

void AnimationsEditor::removeKeyframeClicked()
{
    if (keyframeList->currentIndex().row() == -1)
        return;

    editPath->keyframeList()->removeAt(keyframeList->currentIndex().row());
    kUpdateList();
    _kDeselect();
    emit redrawMap();
}

void AnimationsEditor::xChanged(int x)
{
    editKeyframe->setx(x);
    kUpdateList();
    emit redrawMap();
}

void AnimationsEditor::yChanged(int y)
{
    editKeyframe->sety(y);
    kUpdateList();
}

void AnimationsEditor::zChanged(int z)
{
    editKeyframe->setz(z);
    kUpdateList();
    emit redrawMap();
}

void AnimationsEditor::rotChanged(int rot)
{
    editKeyframe->setRot(rot);
}

void AnimationsEditor::paceChanged(int pace)
{
    editKeyframe->setPace(pace);
}
