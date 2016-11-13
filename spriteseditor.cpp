#include "spriteseditor.h"

#include <QGridLayout>
#include <QLabel>

SpritesEditor::SpritesEditor(Map* map, QWidget* parent) : QWidget(parent)
{
    this->map = map;

    QStringList types;
    types << "Hammer Bro." << "Red Flying ? Block";


    QGridLayout* layout = new QGridLayout();

    layout->addWidget(new QLabel("Sprite 1 Type:"), 0, 0, 1, 1, Qt::AlignRight);
    sprite1Type = new QComboBox(this);
    sprite1Type->addItems(types);
    sprite1Type->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    connect(sprite1Type, SIGNAL(currentIndexChanged(int)), this, SLOT(sprite1TypeChanged(int)));
    layout->addWidget(sprite1Type, 0, 1);

    layout->addWidget(new QLabel("Sprite 1 Start Node:"), 1, 0, 1, 1, Qt::AlignRight);
    sprite1StartNode = new QSpinBox(this);
    sprite1StartNode->setRange(0, 255);
    connect(sprite1StartNode, SIGNAL(valueChanged(int)), this, SLOT(sprite1StartNodeChanged(int)));
    layout->addWidget(sprite1StartNode, 1, 1);

    layout->addWidget(new HorLine(), 2, 0, 1, 2);

    layout->addWidget(new QLabel("Sprite 2 Type:"), 3, 0, 1, 1, Qt::AlignRight);
    sprite2Type = new QComboBox(this);
    sprite2Type->addItems(types);
    sprite2Type->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    connect(sprite2Type, SIGNAL(currentIndexChanged(int)), this, SLOT(sprite2TypeChanged(int)));
    layout->addWidget(sprite2Type, 3, 1);

    layout->addWidget(new QLabel("Sprite 2 Start Node:"), 4, 0, 1, 1, Qt::AlignRight);
    sprite2StartNode = new QSpinBox(this);
    sprite2StartNode->setRange(0, 255);
    connect(sprite2StartNode, SIGNAL(valueChanged(int)), this, SLOT(sprite2StartNodeChanged(int)));
    layout->addWidget(sprite2StartNode, 4, 1);

    this->setLayout(layout);


    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}

void SpritesEditor::setMap(Map* map)
{
    this->map = map;
    updateInfo();
}

void SpritesEditor::updateInfo()
{
    sprite1Type->blockSignals(true);
    sprite2Type->blockSignals(true);
    sprite1StartNode->blockSignals(true);
    sprite2StartNode->blockSignals(true);

    sprite1Type->setCurrentIndex(map->sprite1Type);
    sprite2Type->setCurrentIndex(map->sprite2Type);
    sprite1StartNode->setValue(map->sprite1StartNode);
    sprite2StartNode->setValue(map->sprite2StartNode);

    sprite1Type->blockSignals(false);
    sprite2Type->blockSignals(false);
    sprite1StartNode->blockSignals(false);
    sprite2StartNode->blockSignals(false);
}

void SpritesEditor::sprite1TypeChanged(int sprite1Type)
{
    map->sprite1Type = sprite1Type;
}

void SpritesEditor::sprite1StartNodeChanged(int sprite1StartNode)
{
    map->sprite1StartNode = sprite1StartNode;
}

void SpritesEditor::sprite2TypeChanged(int sprite2Type)
{
    map->sprite2Type = sprite2Type;
}

void SpritesEditor::sprite2StartNodeChanged(int sprite2StartNode)
{
    map->sprite2StartNode = sprite2StartNode;
}
