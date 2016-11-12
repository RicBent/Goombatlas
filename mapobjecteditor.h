#ifndef MAPOBJECTEDITOR_H
#define MAPOBJECTEDITOR_H

#include "map.h"
#include "mapview.h"

#include <QWidget>
#include <QComboBox>
#include <QListWidget>
#include <QToolBar>
#include <QSpinBox>

class MapObjectEditor : public QWidget
{
    Q_OBJECT
public:
    explicit MapObjectEditor(Map* map, MapView* mapView, QWidget* parent = 0);
    void setMap(Map* map);

public slots:
    void select(MapObject* object);
    void deselect();

signals:
    void redrawMap();
    void objectSelected(MovableObject* o);
    void objectDeselected();

private slots:
    void typeChanged(int typeId);

    void objectListIndexChanged(int index);
    void addObjectClicked();
    void removeObjectClicked();
    void moveUpClicked();
    void moveDownClicked();
    void pathBehaviorChanged(int pathBehavior);
    void xChanged(int x);
    void yChanged(int y);
    void zChanged(int z);

private:
    class HorLine : public QFrame { public: HorLine() { setFrameStyle(QFrame::HLine | QFrame::Sunken); } };

    Map* map;
    MapView* mapView;
    MapObject* editObject;
    QList<MapObject*>* editList;

    QComboBox* type;
    QListWidget* objectList;
    QToolBar* toolbar;
    QSpinBox* pathBehaviorId;
    QSpinBox* xPos;
    QSpinBox* yPos;
    QSpinBox* zPos;

    void _select(MapObject* object);
    void _deselect();
    void updateList(bool keepIndex=true);
    void setEditsEnabled(bool enabled);
    void updateInfo();
    void allowChanges(bool allow);
    void clearValues();
};

#endif // MAPOBJECTEDITOR_H
