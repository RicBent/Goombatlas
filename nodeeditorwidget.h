#ifndef NODEEDITORWIDGET_H
#define NODEEDITORWIDGET_H

#include "map.h"

#include <QWidget>
#include <QListWidget>
#include <QFrame>
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>
#include <QHash>

class NodeEditorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NodeEditorWidget(Map* map, QWidget* parent = 0);
    void select(Node* node);
    void deselect();
    void setMap(Map* map);

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

private:
    class HorLine : public QFrame { public: HorLine() { setFrameStyle(QFrame::HLine | QFrame::Sunken); } };

    QListWidget* nodeList;
    QSpinBox* xPos;
    QSpinBox* yPos;
    QSpinBox* zPos;
    QSpinBox* areaId;
    QSpinBox* iconId;
    QComboBox* icon;

    QHash<quint8, int> iconIndexes;

    Map* map;
    Node* editNode;

    void updateList();
    void setEditsEnabled(bool enabled);
    void updateInfo();
    void allowChanges(bool allow);
    void clearValues();
    void loadLevelIcons();
};

#endif // NODEEDITORWIDGET_H
