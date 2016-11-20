#ifndef ANIMATIONSEDITOR_H
#define ANIMATIONSEDITOR_H

#include "map.h"
#include "mapview.h"

#include <QWidget>
#include <QListWidget>
#include <QFrame>
#include <QLabel>
#include <QSpinBox>
#include <QToolBar>

class AnimationsEditor : public QWidget
{
    Q_OBJECT
public:
    AnimationsEditor(Map* map, MapView* mapView, QWidget* parent = 0);
    void setMap(Map* map);

public slots:
    //void selectKeyframe(Keyframe* keyframe);
    //void deselectKeyframe();

signals:
    void redrawMap();
    void keyframeSelected(MovableObject* n);
    void keyframeDeselected();

public slots:
    void kSelect(Keyframe* k);
    void kDeselect();

private slots:
    void pathsListIndexChanged(int index);
    void addPathClicked();
    void removePathClicked();
    void movePathUpClicked();
    void movePathDownClicked();

    void keyframeListIndexChanged(int index);
    void addKeyframeClicked();
    void removeKeyframeClicked();
    void moveKeyframeUpClicked();
    void moveKeyframeDownClicked();

    void xChanged(int x);
    void yChanged(int y);
    void zChanged(int z);
    void rotChanged(int rot);
    void paceChanged(int pace);

private:
    class HorLine : public QFrame { public: HorLine() { setFrameStyle(QFrame::HLine | QFrame::Sunken); } };

    QListWidget* pathsList;

    QListWidget* keyframeList;
    QToolBar* kButtonsBar;
    QSpinBox* xPos;
    QSpinBox* yPos;
    QSpinBox* zPos;
    QSpinBox* rot;
    QSpinBox* pace;

    Map* map;
    MapView* mapView;
    AnimationPath* editPath;
    Keyframe* editKeyframe;

    void select(AnimationPath* path);
    void deselect();
    void updateList(bool keepIndex=true);

    void _kSelect(Keyframe* k);
    void _kDeselect();
    void kUpdateList(bool keepIndex=true);
    void kSetEditsEnabled(bool enabled);
    void kUpdateInfo();
    void kAllowChanges(bool allow);
    void kClearValues();
};

#endif // ANIMATIONSEDITOR_H
