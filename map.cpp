#include "map.h"

#include <QMessageBox>

Map::Map()
{
    path = "";
}

Map::Map(QFile* file)
{
    path = file->fileName();

    QXmlStreamReader xmlReader(file);

    xmlReader.readNext();

    while (!xmlReader.atEnd())
    {
        if (xmlReader.isStartElement())
        {
            if (xmlReader.name() == "nodes")
                readNodes(&xmlReader);
            else if (xmlReader.name() == "pathSettings")
                readPathSettings(&xmlReader);
            else
                xmlReader.readNext();
        }
        else
            xmlReader.readNext();
    }

    file->close();
}

Map::~Map()
{
    foreach (Node* n, nodes)
        delete n;

    foreach (PathBehavior* p, pathBehaviors)
        delete p;
}

void Map::readNodes(QXmlStreamReader* xmlReader)
{
    while (!xmlReader->atEnd())
    {
        if(xmlReader->isEndElement())
        {
            xmlReader->readNext();
            break;
        }
        else if(xmlReader->isStartElement())
        {
            if (xmlReader->name() == "node")
                addNode(new Node(xmlReader));
        }
        xmlReader->readNext();
    }
}

void Map::readPathSettings(QXmlStreamReader* xmlReader)
{
    while (!xmlReader->atEnd())
    {
        if(xmlReader->isEndElement())
        {
            xmlReader->readNext();
            break;
        }
        else if(xmlReader->isStartElement())
        {
            if (xmlReader->name() == "pathSetting")
                addPathBehavior(new PathBehavior(xmlReader));
        }
        xmlReader->readNext();
    }
}

bool Map::save(QWidget* parent, QString path)
{
    if (path.isEmpty() || path.isEmpty())
        path = this->path;
    else
        this->path = path;

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::information(parent, "Goombatlas", "Could not open or create " + path + " for writing!", QMessageBox::Ok);
        return false;
    }

    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();

    xmlWriter.writeStartElement("map");

        xmlWriter.writeStartElement("nodes");
        foreach (Node* n, nodes)
            n->writeXml(&xmlWriter);
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("pathSettings");
        foreach (PathBehavior* p, pathBehaviors)
            p->writeXml(&xmlWriter);
        xmlWriter.writeEndElement();

    xmlWriter.writeEndElement();

    xmlWriter.writeEndElement();
    file.close();

    return true;
}


Node* Map::getNodePtr(int index)
{
    return nodes.at(index);
}

void Map::addNode(Node* node, int index)
{
    if (index == -1)
        nodes.append(node);
    else
        nodes.insert(index, node);
}

void Map::removeNode(int index)
{
    nodes.removeAt(index);
}

void Map::removeNode(Node* node)
{
    nodes.removeOne(node);
}

void Map::moveNodeUp(int index)
{
    if (index <= 0)
        return;

    nodes.move(index, index-1);
}

void Map::moveNodeDown(int index)
{
    if (index >= nodes.length()-1)
        return;

    nodes.move(index, index+1);
}


PathBehavior* Map::getPathBehaviorPtr(int index)
{
    return pathBehaviors.at(index);
}

void Map::addPathBehavior(PathBehavior* pathBehavior, int index)
{
    if (index == -1)
        pathBehaviors.append(pathBehavior);
    else
        pathBehaviors.insert(index, pathBehavior);
}

void Map::removePathBehavior(int index)
{
    pathBehaviors.removeAt(index);
}

void Map::removePathBehavior(PathBehavior* pathBehavior)
{
    pathBehaviors.removeOne(pathBehavior);
}

void Map::movePathBehaviorUp(int index)
{
    if (index <= 0)
        return;

    pathBehaviors.move(index, index-1);
}

void Map::movePathBehaviorDown(int index)
{
    if (index >= pathBehaviors.length()-1)
        return;

    pathBehaviors.move(index, index+1);
}
