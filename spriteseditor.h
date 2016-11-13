#ifndef SPRITESEDITOR_H
#define SPRITESEDITOR_H

#include "map.h"

#include <QWidget>
#include <QComboBox>
#include <QSpinBox>

class SpritesEditor : public QWidget
{
    Q_OBJECT
public:
    explicit SpritesEditor(Map* map, QWidget* parent = 0);
    void setMap(Map* map);

private slots:
    void sprite1TypeChanged(int sprite1Type);
    void sprite1StartNodeChanged(int sprite1StartNode);
    void sprite2TypeChanged(int sprite2Type);
    void sprite2StartNodeChanged(int sprite2StartNode);

private:
    class HorLine : public QFrame { public: HorLine() { setFrameStyle(QFrame::HLine | QFrame::Sunken); } };

    Map* map;

    QComboBox* sprite1Type;
    QSpinBox* sprite1StartNode;
    QComboBox* sprite2Type;
    QSpinBox* sprite2StartNode;

    void updateInfo();
};

#endif // SPRITESEDITOR_H
