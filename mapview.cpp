#include "mapview.h"

#include <QPaintEvent>
#include <QPainter>

MapView::MapView(QWidget *parent, Map *map) : QWidget(parent)
{
    this->map = map;
}

MapView::~MapView()
{

}

void MapView::paintEvent(QPaintEvent* evt)
{
    QPainter painter(this);
    painter.fillRect(evt->rect(), QBrush(Qt::gray));
}
