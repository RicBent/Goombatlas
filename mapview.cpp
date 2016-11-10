#include "mapview.h"

#include <QPaintEvent>
#include <QPainter>
#include <QResizeEvent>

MapView::MapView(QWidget *parent, Map *map) : QWidget(parent)
{
    this->map = map;
    showGrid = true;
    gridSize = 64;
    nodeSelected = false;
    drag = false;

    centerX = 1600;
    centerY = 1600;

    setZoom(2);
}

MapView::~MapView()
{

}

void MapView::viewAreaResized(QResizeEvent* evt)
{
    float wdiff = evt->size().width() - evt->oldSize().width();
    float hdiff = evt->size().height() - evt->oldSize().height();

    QPoint wrongPos = getCurrentPos();
    emit scrollTo(QPoint(wrongPos.x() - wdiff/(2*zoom), wrongPos.y() - hdiff/(2*zoom)));
}

void MapView::setZoom(float zoom)
{
    QPoint currPos = getCurrentPos();

    this->zoom = zoom;

    setMinimumSize(centerX*2*zoom, centerY*2*zoom);
    setMaximumSize(centerX*2*zoom, centerY*2*zoom);
    resize(centerX*2*zoom, centerY*2*zoom);

    emit scrollTo(currPos);

    repaint();
}

void MapView::zoomIn()
{
    if (zoom < 3.5)
        setZoom(zoom+0.25);
}

void MapView::zoomOut()
{
    if (zoom > 0.75)
        setZoom(zoom-0.25);
}

void MapView::zoomMax()
{
    setZoom(3.5);
}

void MapView::zoomMin()
{
    setZoom(1);
}

void MapView::setMap(Map* map)
{
    this->map = map;
    update();
}

QPoint MapView::getCurrentPos()
{
    return QPoint((visibleRegion().boundingRect().x() + visibleRegion().boundingRect().width()/2)/zoom - centerX, (visibleRegion().boundingRect().y() + visibleRegion().boundingRect().height()/2)/zoom - centerY);
}

void MapView::paintEvent(QPaintEvent* evt)
{
    QPainter painter(this);
    painter.scale(zoom,zoom);

    QRect drawrect = QRect(evt->rect().x()/zoom, evt->rect().y()/zoom, evt->rect().width()/zoom+20, evt->rect().height()/zoom+20);

    painter.fillRect(drawrect, QColor(119,136,153));

    if (showGrid)
    {
        QPen lineBold(Qt::white);
        lineBold.setWidthF(3/zoom);

        painter.setPen(lineBold);

        painter.drawLine(drawrect.x(), centerY, drawrect.x() + drawrect.width(), centerY);
        painter.drawLine(centerX, drawrect.y(), centerX, drawrect.y() + drawrect.height());

        QPen line(Qt::white);
        line.setWidthF(1/zoom);
        painter.setPen(line);

        int y = roundDown(drawrect.y() - centerY, gridSize) + centerY;
        while (y < drawrect.y() + drawrect.height())
        {
            y += gridSize;
            if (y != centerY)
                painter.drawLine(drawrect.x(), y, drawrect.x() + drawrect.width(), y);
        }

        int x = roundDown(drawrect.x() - centerX, gridSize) + centerX;
        while (x < drawrect.x() + drawrect.width())
        {
            x += gridSize;
            if (x != centerX)
                painter.drawLine(x, drawrect.y(), x, drawrect.y() + drawrect.height());
        }
    }

    painter.translate(centerX, centerY);


    // Draw Paths

    QPen pathPen(QColor(Qt::yellow));
    pathPen.setWidth(2);
    QPen pathBackPen(QColor(0xFF, 0x88, 0));
    pathBackPen.setWidth(2);

    foreach (Node* n1, map->nodes)
    {
        for (int i = 0; i < n1->pathList()->length(); i++)
        {
            Path* p = n1->pathList()->at(i);
            if (p->endingNodeId > map->nodes.length()-1)
                continue;

            Node* n2 = map->getNodePtr(p->endingNodeId);

            QPointF start(n1->getx(), n1->getz());
            QPointF end(n2->getx(), n2->getz());

            int deltaY = n2->getz() - n1->getz();
            int deltaX = n2->getx() - n1->getx();

            float angle = atan2(deltaY, deltaX);
            if (p->settings & 0x40)
                angle += M_PI;

            int offset = 3;
            if (p->settings & 0x40)
                offset *= -1;

            QPointF start1 = start;
            start1.setY(start.y() - offset);
            start1 = rotateArroundPoint(start1, start, angle);
            QPointF end1 = end;
            end1.setY(end.y() - offset);
            end1 = rotateArroundPoint(end1, end, angle);

            //qDebug() << start1 << end1 << qRadiansToDegrees(angle);

            if (p->settings & 0x40)
                painter.setPen(pathPen);
            else
                painter.setPen(pathBackPen);

            painter.drawLine(start1, end1);
        }
    }


    // Draw Nodes

    QString nodesPath(QCoreApplication::applicationDirPath() + "/goombatlas_data/nodes/");

    foreach (Node* n, map->nodes)
    {
        QRect nodeRect(n->getx()-n->getwidth()/2, n->getz()-n->getheight()/2, n->getwidth(), n->getheight());

        QPixmap nodePixmap;

        if (n->getSetting(3))
        {
            if (n->getIconId() == 0)
                nodePixmap = QPixmap(nodesPath + "start.png");
            else
                nodePixmap = QPixmap(nodesPath + "normal.png");
        }
        else
            nodePixmap = QPixmap(nodesPath + "locked.png");


        painter.drawPixmap(nodeRect, nodePixmap);
    }

    if (nodeSelected)
    {
        QPen selPen(Qt::red);
        selPen.setWidthF(2/zoom);
        painter.setPen(selPen);
        painter.drawEllipse(QRect(selectedNode->getx() - selectedNode->getwidth()/2, selectedNode->getz() - selectedNode->getheight()/2, selectedNode->getwidth(), selectedNode->getheight()));
    }
}

int MapView::roundDown(int num, int factor)
{
    return num - (num % factor) - factor;
}

void MapView::setNodeWidth(int width)
{
    foreach (Node* n, map->nodes)
    {
        n->setwidth(width);
        n->setheight(width);
    }
}

void MapView::selectNode(Node* node)
{
    nodeSelected = true;
    selectedNode = node;
    repaint();
}

void MapView::deselectNode()
{
    nodeSelected = false;
    repaint();
}


// Mouse Functions

void MapView::mousePressEvent(QMouseEvent* evt)
{
    int mouseX = evt->x()/zoom - centerX;
    int mouseY = evt->y()/zoom - centerY;

    if (evt->button() == Qt::LeftButton)
    {
        bool actualNodeClicked = false;

        if (nodeSelected && selectedNode->clickDetection(mouseX, mouseY))
        {
            actualNodeClicked = true;
            dragX = selectedNode->getx();
            dragY = selectedNode->getz();
            lastX = mouseX;
            lastY = mouseY;
            drag = true;
        }

        if (!actualNodeClicked)
        {
            nodeSelected = false;
            for (int i = map->nodes.length()-1; i >= 0; i--)
            {
                Node* n = map->getNodePtr(i);
                if (n->clickDetection(mouseX, mouseY))
                {
                    selectedNode = n;
                    nodeSelected = true;
                    emit changeSelectedNode(selectedNode);
                    break;
                }
            }
            if (!nodeSelected)
                emit changeDeselectedNode();
        }
        repaint();
    }
    else if (evt->button() == Qt::RightButton)
    {
        Node* newNode = new Node(mouseX, 0, mouseY);
        selectedNode = newNode;
        dragX = selectedNode->getx();
        dragY = selectedNode->getz();
        lastX = mouseX;
        lastY = mouseY;
        drag = true;
        map->addNode(newNode);
        emit changeSelectedNode(newNode);
    }
}

void MapView::mouseMoveEvent(QMouseEvent* evt)
{
    int mouseX = evt->x()/zoom - centerX;
    int mouseY = evt->y()/zoom - centerY;

    if (drag)
    {
        int deltaX = mouseX - lastX;
        int deltaY = mouseY - lastY;

        selectedNode->setx(selectedNode->getx() + deltaX);
        selectedNode->setz(selectedNode->getz() + deltaY);

        lastX += deltaX;
        lastY += deltaY;

        emit changeSelectedNode(selectedNode);

        repaint();
    }
}

void MapView::mouseReleaseEvent(QMouseEvent* evt)
{
    drag = false;
}


// Misc functions

QPointF MapView::rotateArroundPoint(QPointF point, QPointF center, float angle)
{
    float s = sin(angle);
    float c = cos(angle);

    // translate point back to origin:
    point.setX(point.x() - center.x());
    point.setY(point.y() - center.y());

    // rotate point
    float xnew = point.x() * c - point.y() * s;
    float ynew = point.x() * s + point.y() * c;

    // translate point back:
    point.setX(xnew + center.x());
    point.setY(ynew + center.y());
    return point;
}
