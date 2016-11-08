#ifndef PATHBEHAVIOREDITOR_H
#define PATHBEHAVIOREDITOR_H

#include "map.h"

#include <QWidget>
#include <QListWidget>
#include <QSpinBox>
#include <QComboBox>
#include <QHash>

class PathBehaviorEditor : public QWidget
{
    Q_OBJECT
public:
    explicit PathBehaviorEditor(Map* map, QWidget* parent = 0);
    void select(PathBehavior* pathBehavior);
    void deselect();
    void setMap(Map* map);

private slots:
    void pathBehaviorsListIndexChanged(int index);
    void addPathBehaviorClicked();
    void removePathBehaviorClicked();
    void moveUpClicked();
    void moveDownClicked();
    void animationIdChanged(int animationId);
    void starCoinCostChanged(int starCoinCost);
    void settingsChanged(int index);

private:
    class HorLine : public QFrame { public: HorLine() { setFrameStyle(QFrame::HLine | QFrame::Sunken); } };

    QListWidget* pathBehaviorList;
    QComboBox* settings;
    QSpinBox* animationId;
    QSpinBox* starCoinCost;

    QHash<int, quint8> settingsHash;

    Map* map;
    PathBehavior* editPathBehavior;

    void updateList();
    void setEditsEnabled(bool enabled);
    void updateInfo();
    void allowChanges(bool allow);
    void clearValues();
};

#endif // PATHBEHAVIOREDITOR_H
