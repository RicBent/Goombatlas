#ifndef NODEEDITORWIDGET_H
#define NODEEDITORWIDGET_H

#include "map.h"
#include "mapview.h"

#include <QWidget>
#include <QListWidget>
#include <QFrame>
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>
#include <QCheckBox>
#include <QHash>

class NodeEditorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NodeEditorWidget(Map* map, MapView* mapView, QWidget* parent = 0);
    void select(Node* node);
    void deselect();
    void setMap(Map* map);

signals:
    redrawMap();

private slots:
    void nodeListIndexChanged(int index);
    void addNodeClicked();
    void removeNodeClicked();
    void moveUpClicked();
    void moveDownClicked();
    void xChanged(int x);
    void yChanged(int y);
    void zChanged(int z);
    void areaIdChnaged(int areaId);
    void iconIdChanged(int iconId);
    void iconChanged(int iconIndex);
    void bowserJrPathChanged(int pathIndex);
    void nodeVisibleToggled(bool toggle);
    void hasStarCoinsToggled(bool toggle);
    void goToNextWorldToggled(bool toggle);
    void unknownBitToggled(bool toggle);

private:
    class HorLine : public QFrame { public: HorLine() { setFrameStyle(QFrame::HLine | QFrame::Sunken); } };

    QListWidget* nodeList;
    QSpinBox* xPos;
    QSpinBox* yPos;
    QSpinBox* zPos;
    QSpinBox* areaId;
    QSpinBox* iconId;
    QComboBox* icon;
    QComboBox* bowserJrPath;
    QCheckBox* nodeVisible;
    QCheckBox* hasStarCoins;
    QCheckBox* goToNextWorld;
    QCheckBox* unknownBit;

    QHash<quint8, int> iconIndexes;

    Map* map;
    MapView* mapView;
    Node* editNode;

    void updateList();
    void setEditsEnabled(bool enabled);
    void updateInfo();
    void allowChanges(bool allow);
    void clearValues();
    void loadLevelIcons();
};

#endif // NODEEDITORWIDGET_H
