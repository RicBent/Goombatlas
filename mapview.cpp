#include "mapview.h"
#include "misc.h"

#include <QPaintEvent>
#include <QPainter>
#include <QResizeEvent>

MapView::MapView(QWidget *parent, Map *map) : QWidget(parent)
{
    this->map = map;
    mode = 0;
    showGrid = true;
    gridSize = 64;
    selected = false;
    drag = false;
    currentAnimPath = -1;

    centerX = 1600;
    centerY = 1600;


    keyframeShape << QPoint(0, -8)
                  << QPoint(8, 0)
                  << QPoint(0, 8)
                  << QPoint(-8, 0);

    QPolygon keyframeShapeL_p;
    keyframeShapeL_p << keyframeShape.at(0) << keyframeShape.at(2) << keyframeShape.at(3);
    keyframeShapeL.addPolygon(keyframeShapeL_p);

    QPolygon keyframeShapeR_p;
    keyframeShapeR_p << keyframeShape.at(0) << keyframeShape.at(2) << keyframeShape.at(1);
    keyframeShapeR.addPolygon(keyframeShapeR_p);

    setZoom(2);
}

MapView::~MapView()
{

}

void MapView::setMode(int mode)
{
    this->mode = mode;
    selected = false;
    repaint();
}

void MapView::setCurrentAnimPath(int animPath)
{
    this->currentAnimPath = animPath;
    repaint();
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
    deselect();
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

    bool drawBothDirections = true;

    if (mode == 1)
        drawBothDirections = false;

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

            int offset = 0;
            if (drawBothDirections)
            {
                offset = 3;
                if (p->settings & 0x40)
                    offset *= -1;
            }

            QPointF start1 = start;
            start1.setY(start.y() - offset);
            start1 = rotateArroundPoint(start1, start, angle);
            QPointF end1 = end;
            end1.setY(end.y() - offset);
            end1 = rotateArroundPoint(end1, end, angle);

            painter.setPen(pathPen);

            if (drawBothDirections && p->settings & 0x40)
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


    // Draw Star Coin Signs

    QString objectsPath(QCoreApplication::applicationDirPath() + "/goombatlas_data/objects/");

    foreach (MapObject* o, map->starCoinSigns)
    {
        QRect signRect(o->getx()+o->getoffsetx(), o->getz()+o->getoffsetz(), o->getwidth(), o->getheight());
        QPixmap signPixmap(objectsPath + "star_coin_sign.png");
        painter.drawPixmap(signRect, signPixmap);
    }


    // Draw Mushroom Houses

    foreach (MapObject* o, map->mushroomHouses)
    {
        QRect mushroomRect(o->getx()+o->getoffsetx()-9, o->getz()+o->getoffsetz(), o->getwidth()+9, o->getheight());

        QPixmap mushroomPixmap(objectsPath + "mushroom_house_red.png");

        if (o->getNodeId() < map->nodes.length())
        {
            Node* parentNode = map->nodes.at(o->getNodeId());
            if (parentNode->getIconId() == 18)
                mushroomPixmap = QPixmap(objectsPath + "mushroom_house_green.png");
            else if (parentNode->getIconId() == 19)
            {
                mushroomPixmap = QPixmap(objectsPath + "mushroom_house_orange.png");
                mushroomRect.adjust(0, -6, 6, 0);
            }
            if (parentNode->getIconId() == 23)
                mushroomPixmap = QPixmap(objectsPath + "mushroom_house_blue.png");
        }

        painter.drawPixmap(mushroomRect, mushroomPixmap);
    }


    // Draw Towers/Castles

    foreach (MapObject* o, map->towersCastles)
    {
        if (o->getNodeId() < map->nodes.length())
        {
            Node* parentNode = map->nodes.at(o->getNodeId());

            if (parentNode->getIconId() == 14)
            {
                QRect castleRect(o->getx()+o->getoffsetx()-7, o->getz()+o->getoffsetz(), 73, o->getheight());
                QPixmap castlePixmap(objectsPath + "castle.png");
                painter.drawPixmap(castleRect, castlePixmap);
                continue;
            }
        }

        QRect towerRect(o->getx()+o->getoffsetx()+8, o->getz()+o->getoffsetz()+7, 55, 40);
        QPixmap towerPixmap(objectsPath + "tower.png");
        painter.drawPixmap(towerRect, towerPixmap);
    }


    // Draw Keyframes

    if (mode == 1 && currentAnimPath != -1 && currentAnimPath < map->animationpaths.length())
    {
        AnimationPath* p = map->animationpaths[currentAnimPath];

        QPen animPathPen(QColor(0x00, 0x35, 0x35));
        animPathPen.setWidth(2);
        painter.setPen(animPathPen);

        painter.setRenderHint(QPainter::Antialiasing);
        for (int i = 1; i < p->keyframeList()->length(); i++)
        {
            Keyframe* lK = p->keyframeList()->at(i-1);
            Keyframe* cK = p->keyframeList()->at(i);
            painter.drawLine(QPoint(lK->getx(), lK->getz()), QPoint(cK->getx(), cK->getz()));
        }
        painter.setRenderHint(QPainter::Antialiasing, false);

        for (int i = 0; i < p->keyframeList()->length(); i++)
        {
            Keyframe* k = p->keyframeList()->at(i);
            painter.fillPath(keyframeShapeL.translated(k->getx(), k->getz()), QBrush(QColor(0x00, 0xAC, 0xC1)));
            painter.fillPath(keyframeShapeR.translated(k->getx(), k->getz()), QBrush(QColor(0x0d, 0x82, 0x93)));
            painter.setPen(Qt::black);
            painter.drawPolygon(keyframeShape.translated(k->getx(), k->getz()));
        }
    }

    // Draw Selection

    QPen selPen(Qt::red);
    selPen.setWidthF(2/zoom);
    painter.setPen(selPen);

    if (selected)
    {
        if (mode == 0)
        {
            if (is<Node*>(selectedObj))
                painter.drawEllipse(QRect(selectedObj->getx() - selectedObj->getwidth()/2, selectedObj->getz() - selectedObj->getheight()/2, selectedObj->getwidth(), selectedObj->getheight()));
            else
                painter.drawRect(QRect(selectedObj->getx() + selectedObj->getoffsetx(), selectedObj->getz() + selectedObj->getoffsetz(), selectedObj->getwidth(), selectedObj->getheight()));
        }
        else if (mode == 1)
        {
            if (is<Keyframe*>(selectedObj))
                painter.drawPolygon(keyframeShape.translated(selectedObj->getx(), selectedObj->getz()));
        }
    }
}

int MapView::roundDown(int num, int factor)
{
    return num - (num % factor) - factor;
}

void MapView::select(MovableObject* obj)
{
    if (is<Node*>(obj))
        emit changeDeselectedMapObj();
    if (is<MapObject*>(obj))
        emit changeDeselectedNode();

    selected = true;
    selectedObj = obj;
    repaint();
}

void MapView::deselect()
{
    selected = false;
    repaint();
}


// Mouse Functions

void MapView::mousePressEvent(QMouseEvent* evt)
{
    if (evt->buttons() == Qt::MiddleButton)
    {
        sDragX = evt->x();
        sDragY = evt->y();
        drag = false;
        return;
    }

    int mouseX = evt->x()/zoom - centerX;
    int mouseY = evt->y()/zoom - centerY;

    if (evt->button() == Qt::LeftButton)
    {
        bool actualObjClicked = false;

        if (selected && selectedObj->clickDetection(mouseX, mouseY))
        {
            actualObjClicked = true;
            dragX = selectedObj->getx();
            dragY = selectedObj->getz();
            lastX = mouseX;
            lastY = mouseY;
            drag = true;
        }

        if (!actualObjClicked)
        {
            selected = false;

            if (mode == 0)
            {
                if (!selected) for (int i = map->towersCastles.length()-1; i >= 0; i--)
                {
                    MapObject* o = map->towersCastles.at(i);
                    if (o->clickDetection(mouseX, mouseY))
                    {
                        selectedObj = o;
                        selected = true;
                        emit changeSelectedMapObj(o);
                        break;
                    }
                }

                if (!selected) for (int i = map->mushroomHouses.length()-1; i >= 0; i--)
                {
                    MapObject* o = map->mushroomHouses.at(i);
                    if (o->clickDetection(mouseX, mouseY))
                    {
                        selectedObj = o;
                        selected = true;
                        emit changeSelectedMapObj(o);
                        break;
                    }
                }

                if (!selected) for (int i = map->starCoinSigns.length()-1; i >= 0; i--)
                {
                    MapObject* o = map->starCoinSigns.at(i);
                    if (o->clickDetection(mouseX, mouseY))
                    {
                        selectedObj = o;
                        selected = true;
                        emit changeSelectedMapObj(o);
                        break;
                    }
                }

                if (!selected) for (int i = map->nodes.length()-1; i >= 0; i--)
                {
                    Node* n = map->getNodePtr(i);
                    if (n->clickDetection(mouseX, mouseY))
                    {
                        selectedObj = n;
                        selected = true;
                        emit changeSelectedNode(n);
                        break;
                    }
                }
            }

            else if (mode == 1)
            {
                if (!selected && currentAnimPath != -1 && currentAnimPath < map->animationpaths.length())
                {
                    for (int i = map->animationpaths[currentAnimPath]->keyframeList()->length()-1; i >= 0; i--)
                    {
                        Keyframe* k = map->animationpaths[currentAnimPath]->keyframeList()->at(i);
                        if (k->clickDetection(mouseX, mouseY))
                        {
                            selectedObj = k;
                            selected = true;
                            emit changeSelectedKeyframe(k);
                            break;
                        }
                    }
                }
            }


            if (selected)
            {
                if (mode == 0)
                {
                    if (!is<Node*>(selectedObj))
                        emit changeDeselectedNode();

                    if (!is<MapObject*>(selectedObj))
                        emit changeDeselectedMapObj();
                }
            }
            else
            {
                if (mode == 0)
                {
                    emit changeDeselectedNode();
                    emit changeDeselectedMapObj();
                }
                else if (mode == 1)
                {
                    emit changeDeselectedKeyframe();
                }
            }
        }
        repaint();
    }
    else if (evt->button() == Qt::RightButton)
    {
        if (mode == 0)
        {
            Node* newNode = new Node(mouseX, 0, mouseY);
            selectedObj = newNode;
            dragX = newNode->getx();
            dragY = newNode->getz();
            lastX = mouseX;
            lastY = mouseY;
            drag = true;
            map->addNode(newNode);
            emit changeSelectedNode(newNode);
            emit changeDeselectedMapObj();
        }
        else if (mode == 1 && currentAnimPath != -1)
        {
            Keyframe* newKeyframe = new Keyframe();
            newKeyframe->setx(mouseX);
            newKeyframe->setz(mouseY);
            dragX = newKeyframe->getx();
            dragY = newKeyframe->getz();
            lastX = mouseX;
            lastY = mouseY;
            drag = true;

            if (!selected)
                map->animationpaths[currentAnimPath]->keyframeList()->append(newKeyframe);
            else
            {
                int insertAt = map->animationpaths[currentAnimPath]->keyframeList()->indexOf(dynamic_cast<Keyframe*>(selectedObj)) + 1;
                qDebug() << "Insert at" << insertAt;
                map->animationpaths[currentAnimPath]->keyframeList()->insert(insertAt, newKeyframe);
            }

            selectedObj = newKeyframe;
            emit changeSelectedKeyframe(newKeyframe);
        }
    }
}

void MapView::mouseMoveEvent(QMouseEvent* evt)
{
    if (evt->buttons() & Qt::MiddleButton)
    {
        int x = evt->x();
        int y = evt->y();

        emit scrollTo_(visibleRegion().boundingRect().x() - x + sDragX, visibleRegion().boundingRect().y() - y + sDragY);
        return;
    }


    int mouseX = evt->x()/zoom - centerX;
    int mouseY = evt->y()/zoom - centerY;

    if (drag)
    {
        int deltaX = mouseX - lastX;
        int deltaY = mouseY - lastY;

        selectedObj->setx(selectedObj->getx() + deltaX);
        selectedObj->setz(selectedObj->getz() + deltaY);

        lastX += deltaX;
        lastY += deltaY;

        if (is<Node*>(selectedObj))
            emit changeSelectedNode(dynamic_cast<Node*>(selectedObj));
        else if (is<MapObject*>(selectedObj))
            emit changeSelectedMapObj(dynamic_cast<MapObject*>(selectedObj));
        else if (is<Keyframe*>(selectedObj))
            emit changeSelectedKeyframe(dynamic_cast<Keyframe*>(selectedObj));

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
