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

    Node* getNodePtr(int index);
    void addNode(Node* node, int index = -1);
    void removeNode(int index);
    void removeNode(Node* node);
    void moveNodeUp(int index);
    void moveNodeDown(int index);

    void readNodes(QXmlStreamReader* xmlReader);

private:
    QString path;
};

#endif // MAP_H
