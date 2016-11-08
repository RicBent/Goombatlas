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

    float getZoom() { return zoom; }
    void setZoom(float zoom);
    void zoomIn();
    void zoomOut();
    void zoomMax();
    void zoomMin();
    void toggleGrid(bool toggle) { showGrid = toggle; repaint(); }

    int getCenterX() { return centerX; }
    int getCenterY() { return centerY; }

signals:
    void scrollTo(int x, int y);
    void scrollTo(QPoint p);

protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *) Q_DECL_OVERRIDE;

private:
    Map *map;

    float zoom;
    bool showGrid;
    int gridSize;

    int centerX;
    int centerY;

    QPoint getCurrentPos();
    int roundDown(int num, int factor);
};

#endif // MAPVIEW_H
