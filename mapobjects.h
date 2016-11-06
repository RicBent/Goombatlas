#ifndef MAPOBJECTS_H
#define MAPOBJECTS_H

#include <QtCore>


// Movable Object Baseclass

class MovableObject
{
public:
    MovableObject();
    virtual ~MovableObject() {}

    qint32 getx() { return x; }
    qint32 gety() { return y; }
    qint32 getz() { return z; }
    qint32 getwidth() { return width; }
    qint32 getheight() { return height; }

    void setx(quint32 x) { this->x = x; }
    void sety(quint32 y) { this->y = y; }
    void setz(quint32 z) { this->z = z; }
    void setwidth(quint32 width) { this->width = width; }
    void setheight(quint32 height) { this->height = height; }

protected:
    qint32 x, y, z;
    qint32 width, height;   // For click detection and rendering
};


// Path

struct Path
{
    quint8 endingNodeId;
    quint8 animationId;
    quint8 settings;
    quint8 unk;
};


// Node

class Node: public MovableObject
{
public:
    Node();
    Node(qint32 x, qint32 y, qint32 z);
    Node(QXmlStreamReader* xmlReader);

    void writeXml(QXmlStreamWriter* xmlWriter);

    quint8 getAreaId() { return areaId; }
    quint8 getIconId() { return iconId; }
    quint8 getSettings() { return settings; }
    quint8 getUnk0() { return unk0; }
    quint8 getUnk1() { return unk1; }

    void setAreaId(quint8 areaId) { this->areaId = areaId; }
    void setIconId(quint8 iconId) { this->iconId = iconId; }
    void setSettings(quint8 settings) { this->settings = settings; }
    void setUnk0(quint8 unk0) { this->unk0 = unk0; }
    void setUnk1(quint8 unk1) { this->unk1 = unk1; }

private:
    // Data Node
    QList<Path> paths;
    quint8 areaId;
    quint8 iconId;
    quint8 settings;
    quint8 unk0;
    quint8 unk1;

    // Visible Node
    quint8 unlockedPaths[4];
    quint8 unlockedPathsSecret[4];
    quint8 cameraScroll;
    quint8 cameraScrollSecret;

    void clearValues();
};


#endif // MAPOBJECTS_H
