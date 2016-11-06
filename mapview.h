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

protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

private:
    Map *map;
};

#endif // MAPVIEW_H
