#ifndef MAP_H
#define MAP_H

#include "mapobjects.h"

class Map
{
public:
    Map();
    Map(QFile* file);
    ~Map();

    QString getPath() { return path; }

    bool save(QWidget* parent, QString path="");
    void exportAsC(QString cFilePath, QString rFilePath, QString prefix, int worldId);

    QList<Node*> nodes;
    QList<PathBehavior*> pathBehaviors;
    QList<MapObject*> starCoinSigns;
    QList<MapObject*> towersCastles;
    QList<MapObject*> mushroomHouses;

    Node* getNodePtr(int index);
    void addNode(Node* node, int index = -1);
    void removeNode(int index);
    void removeNode(Node* node);
    void moveNodeUp(int index);
    void moveNodeDown(int index);

    PathBehavior* getPathBehaviorPtr(int index);
    void addPathBehavior(PathBehavior* pathBehavior, int index = -1);
    void removePathBehavior(int index);
    void removePathBehavior(PathBehavior* node);
    void movePathBehaviorUp(int index);
    void movePathBehaviorDown(int index);

    void readNodes(QXmlStreamReader* xmlReader);
    void readPathSettings(QXmlStreamReader* xmlReader);
    void readMapObjects(QXmlStreamReader* xmlReader, quint8 type);

private:
    QString path;

    QString hex(uint addr);
};

#endif // MAP_H
