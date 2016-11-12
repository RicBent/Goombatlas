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
    qint32 getoffsetx() { return offsetx; }
    qint32 getoffsetz() { return offsetz; }
    qint32 getwidth() { return width; }
    qint32 getheight() { return height; }

    void setx(quint32 x) { this->x = x; }
    void sety(quint32 y) { this->y = y; }
    void setz(quint32 z) { this->z = z; }
    void setoffsetx(quint32 offsetx) { this->offsetx = offsetx; }
    void setoffsetz(quint32 offsetz) { this->offsetz = offsetz; }
    void setwidth(quint32 width) { this->width = width; }
    void setheight(quint32 height) { this->height = height; }

    bool clickDetection(int x, int y);

protected:
    qint32 x, y, z;
    qint32 offsetx, offsetz;
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
    ~Node();

    void writeXml(QXmlStreamWriter* xmlWriter);

    quint8 getAreaId() { return areaId; }
    quint8 getIconId() { return iconId; }
    quint8 getSettings() { return settings; }
    bool getSetting(int bit) { return ((settings >> bit)  & 1); }
    int getBowserJrPath();
    quint8 getUnk0() { return unk0; }
    quint8 getUnk1() { return unk1; }
    quint8 getUnlock(int index) { return unlockedPaths[index]; }
    quint8 getUnlockSecret(int index) { return unlockedPathsSecret[index]; }
    quint8 getCameraScroll() { return cameraScroll; }
    quint8 getCameraScrollSecret() { return cameraScrollSecret; }

    void setAreaId(quint8 areaId) { this->areaId = areaId; }
    void setIconId(quint8 iconId) { this->iconId = iconId; }
    void setSettings(quint8 settings) { this->settings = settings; }
    void setSetting(bool val, int bit) { settings ^= (-val ^ settings) & (1 << bit); }
    void setBowserJrPath(int index);
    void setUnk0(quint8 unk0) { this->unk0 = unk0; }
    void setUnk1(quint8 unk1) { this->unk1 = unk1; }
    void setUnlock(int index, quint8 val) { this->unlockedPaths[index] = val; }
    void setUnlockSecret(int index, quint8 val) { this->unlockedPathsSecret[index] = val; }
    void setCameraScroll(quint8 cameraScroll) { this->cameraScroll = cameraScroll; }
    void setCameraScrollSecret(quint8 cameraScrollSecret) { this->cameraScrollSecret = cameraScrollSecret; }

    QList<Path*> *pathList() { return &paths; }

    bool clickDetection(int x, int y);

private:
    // Data Node
    QList<Path*> paths;
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


// Path Behavior

class PathBehavior
{
public:
    PathBehavior();
    PathBehavior(QXmlStreamReader* xmlReader);

    void writeXml(QXmlStreamWriter* xmlWriter);

    quint8 getAnimationId() { return animationId; }
    quint8 getStarCoinCost() { return starCoinCost; }
    quint8 getSettings() { return settings; }

    void setAnimationId(quint8 animationId) { this->animationId = animationId; }
    void setStarCoinCost(quint8 starCoinCost) { this->starCoinCost = starCoinCost; }
    void setSettings(quint8 settings) { this->settings = settings; }

private:
    quint8 animationId;
    quint8 starCoinCost;
    quint8 settings;

    void clearValues();
};


// Map Object

class MapObject: public MovableObject
{
public:
    MapObject(quint8 type);
    MapObject(QXmlStreamReader* xmlReader, quint8 type);

    void writeXml(QXmlStreamWriter* xmlWriter);

    quint8 getPathBehaviorId() { return pathbehaviorId; }
    void setPathBehaviorId(quint8 pathbehaviorId) { this->pathbehaviorId = pathbehaviorId; }

    quint8 getType() { return type; }

private:
    quint8 type;
    quint8 pathbehaviorId;
};

#endif // MAPOBJECTS_H
