#ifndef MAPVIEW_H
#define MAPVIEW_H

#include "map.h"

#include <QWidget>

class MapView : public QWidget
{
    Q_OBJECT
public:
    explicit MapView(QWidget *parent, Map *map);
    ~MapView();

    void setMap(Map* map);

    void setMode(int mode);
    void setCurrentAnimPath(int animPath);

    float getZoom() { return zoom; }
    void setZoom(float zoom);
    void zoomIn();
    void zoomOut();
    void zoomMax();
    void zoomMin();
    void toggleGrid(bool toggle) { showGrid = toggle; repaint(); }

    void setNodeWidth(int width);

    int getCenterX() { return centerX; }
    int getCenterY() { return centerY; }

    QPoint getCurrentPos();

signals:
    void scrollTo(int x, int y);
    void scrollTo(QPoint p);
    void scrollTo_(int x, int y);

    void changeSelectedNode(Node* node);
    void changeDeselectedNode();

    void changeSelectedMapObj(MapObject* mapObj);
    void changeDeselectedMapObj();

    void changeSelectedKeyframe(Keyframe* keyframe);
    void changeDeselectedKeyframe();

public slots:
    void viewAreaResized(QResizeEvent* evt);
    void select(MovableObject* obj);
    void deselect();

protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *) Q_DECL_OVERRIDE;

private:
    Map* map;

    int mode;

    MovableObject* selectedObj;
    bool selected;

    int currentAnimPath;

    bool drag;
    int dragX, dragY;
    int lastX, lastY;

    int sDragX, sDragY;

    float zoom;
    bool showGrid;
    int gridSize;

    int centerX;
    int centerY;

    int roundDown(int num, int factor);

    QPolygon keyframeShape;
    QPainterPath keyframeShapeL;
    QPainterPath keyframeShapeR;

    QPoint tempPos;

    QPointF rotateArroundPoint(QPointF point, QPointF center, float angle);

    int round(int number, int multiple)
    {
        return (number + (multiple / 2)) / multiple * multiple;
    }
};

#endif // MAPVIEW_H
