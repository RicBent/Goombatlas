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
#include <QToolBar>

class NodeEditorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NodeEditorWidget(Map* map, MapView* mapView, QWidget* parent = 0);
    void setMap(Map* map);

public slots:
    void select(Node* node);
    void deselect();

signals:
    void redrawMap();
    void nodeSelected(Node* n);
    void nodeDeselected();

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

    void pathListIndexChanged(int index);
    void addPathClicked();
    void removePathClicked();
    void movePathUpClicked();
    void movePathDownClicked();
    void endingNodeIdChanged(int endingNodeId);
    void pathSettingIdChanged(int pathSettingId);
    void directionChanged(int direction);
    void reversePathAnimToggled(bool toggle);

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

    QListWidget* pathList;
    QToolBar* pButtonsBar;
    QSpinBox* endingNodeId;
    QSpinBox* pathSettingId;
    QComboBox* direction;
    QCheckBox* reversePathAnim;


    QHash<quint8, int> iconIndexes;

    Map* map;
    MapView* mapView;
    Node* editNode;
    Path* editPath;

    void _select(Node* node);
    void updateList(bool keepIndex=true);
    void setEditsEnabled(bool enabled);
    void updateInfo();
    void allowChanges(bool allow);
    void clearValues();
    void loadLevelIcons();

    void pSelect(Path* p);
    void pDeselect();
    void pUpdateList(bool keepIndex=true);
    void pSetEditsEnabled(bool enabled);
    void pUpdateInfo();
    void pAllowChanges(bool allow);
    void pClearValues();
};

#endif // NODEEDITORWIDGET_H
