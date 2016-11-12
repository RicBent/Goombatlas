#ifndef CSTREAMWRITER_H
#define CSTREAMWRITER_H

#include <mapobjects.h>

#include <QFile>
#include <QTextStream>

class CStreamWriter
{
public:
    CStreamWriter(QFile* file);

    void writeEndl();
    void writeInclude(QString file);

    void writeNode(Node* n, QString prefix, int index, bool comma=false);
    void writeNodes(QList<Node*>* nodes, QString prefix);

    void writePaths(QList<Node*>* nodes, QString prefix);

    void writeVisibleNode(Node* n, bool comma=false);
    void writeVisibleNodes(QList<Node*>* nodes, QString prefix);

    void writePathBehavior(PathBehavior* p, bool comma=false);
    void writePathBehaviors(QList<PathBehavior*>* paths, QString prefix);

    void writeMapObject(MapObject* mapObject);
    void writeMapObjects(QList<MapObject*>* mapObjects, quint8 type, QString prefix);

private:
    QFile* file;
    QTextStream* stream;

    int tabCount;
};

#endif // CSTREAMWRITER_H
