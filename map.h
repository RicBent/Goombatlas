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

    QList<Node*> nodes;
    QList<PathBehavior*> pathBehaviors;

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

private:
    QString path;
};

#endif // MAP_H
