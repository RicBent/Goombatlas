#include "map.h"
#include "cstreamwriter.h"

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
            else if (xmlReader.name() == "starCoinSigns")
                readMapObjects(&xmlReader, 0);
            else if (xmlReader.name() == "towersCastles")
                readMapObjects(&xmlReader, 1);
            else if (xmlReader.name() == "mushroomHouses")
                readMapObjects(&xmlReader, 2);
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

void Map::readMapObjects(QXmlStreamReader* xmlReader, quint8 type)
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
            if (type == 0)
            {
                if (xmlReader->name() == "starCoinSign")
                    starCoinSigns.append(new MapObject(xmlReader, 0));
            }
            else if (type == 1)
            {
                if (xmlReader->name() == "towerCastle")
                    towersCastles.append(new MapObject(xmlReader, 1));
            }
            else if (type == 2)
            {
                if (xmlReader->name() == "mushroomHouse")
                    mushroomHouses.append(new MapObject(xmlReader, 2));
            }
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

        xmlWriter.writeStartElement("starCoinSigns");
        foreach (MapObject* o, starCoinSigns)
            o->writeXml(&xmlWriter);
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("towersCastles");
        foreach (MapObject* o, towersCastles)
            o->writeXml(&xmlWriter);
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("mushroomHouses");
        foreach (MapObject* o, mushroomHouses)
            o->writeXml(&xmlWriter);
        xmlWriter.writeEndElement();

    xmlWriter.writeEndElement();

    xmlWriter.writeEndElement();
    file.close();

    return true;
}

void Map::exportAsC(QString cFilePath, QString rFilePath, QString prefix, int worldId)
{
    QFile cFile(cFilePath);
    if (!cFile.open(QFile::WriteOnly | QFile::Truncate))
    {
        QMessageBox::information(NULL, "Goombatlas", "Could not open or create " + cFilePath + " for writing!", QMessageBox::Ok);
        return;
    }

    QFile rFile(rFilePath);
    if (!rFile.open(QFile::ReadOnly))
    {
        QMessageBox::information(NULL, "Goombatlas", "Could not open or create " + rFilePath + " for reading!", QMessageBox::Ok);
        return;
    }


    // Write C File

    CStreamWriter cWriter(&cFile);

    cWriter.writeInclude("worldmap.h");
    cWriter.writeEndl();
    cWriter.writeNodes(&nodes, prefix);
    cWriter.writeEndl();
    cWriter.writeVisibleNodes(&nodes, prefix);
    cWriter.writeEndl();
    cWriter.writePathBehaviors(&pathBehaviors, prefix);
    cWriter.writeEndl();
    cWriter.writeMapObjects(&starCoinSigns, 0, prefix);
    cWriter.writeEndl();
    cWriter.writeMapObjects(&towersCastles, 1, prefix);
    cWriter.writeEndl();
    cWriter.writeMapObjects(&mushroomHouses, 2, prefix);

    cFile.close();


    // Write Replaces File

    QTextStream rIn(&rFile);
    QList<QString> lines;

    bool ignoreLine = false;
    bool ignoredLine = false;

    while(!rIn.atEnd())
    {
        QString line = rIn.readLine();

        if (ignoredLine)
        {
            ignoredLine = false;
            if (line == "")
                continue;
        }

        if (line.startsWith(QString("@ goombatlas_map_%1_end").arg(worldId)))
        {
            ignoreLine = false;
            ignoredLine = true;
            continue;
        }

        if (ignoreLine)
            continue;

        if (line.startsWith(QString("@ goombatlas_map_%1 - Do not modify these comments and replaces!").arg(worldId)))
        {
            ignoreLine = true;
            continue;
        }

        lines.append(line);
    }

    if (lines.length() != 0)
    {
        if (lines[lines.length() - 1] == "")
            lines.removeAt(lines.length() - 1);
    }

    lines.append("");
    lines.append(QString("@ goombatlas_map_%1 - Do not modify these comments and replaces!").arg(worldId));

    uint worldOffset = 0x20E79C4 + (worldId-1)*0x28;
    lines.append(hex(worldOffset) + "_ov_08: " + prefix + "_nodes");
    lines.append(hex(worldOffset+0x4) + "_ov_08: " + prefix + "_pathBehaviors");
    // Red Flying Block / Hammer Bros.
    lines.append(hex(worldOffset+0xC) + "_ov_08: " + prefix + "_visibleNodes");
    lines.append(hex(worldOffset+0x10) + "_ov_08: " + prefix + "_starCoinSigns");
    lines.append(hex(worldOffset+0x14) + "_ov_08: " + prefix + "_towersCastles");
    lines.append(hex(worldOffset+0x18) + "_ov_08: " + prefix + "_mushroomHouses");
    // Bowser Jr. Paths
    uint counts = nodes.count() & 0xFFFF;
    counts |= pathBehaviors.count() << 16;
    lines.append(hex(worldOffset+0x20) + "_ov_08: 0x" + hex(counts));

    lines.append(QString("@ goombatlas_map_%1_end").arg(worldId));

    rFile.close();
    if (!rFile.open(QFile::WriteOnly | QFile::Truncate))
    {
        QMessageBox::information(NULL, "Goombatlas", "Could not open " + rFilePath + " for writing!", QMessageBox::Ok);
        return;
    }

    QTextStream rOut(&rFile);

    foreach (QString str, lines)
        rOut << str << endl;

    rFile.close();
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

QString Map::hex(uint addr)
{
    return QString("%1").arg(addr, 8, 16, QChar('0')).toUpper();
}
