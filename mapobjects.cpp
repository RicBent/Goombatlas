#include "mapobjects.h"

MovableObject::MovableObject()
{
    x = 0;
    y = 0;
    z = 0;
    width = 0;
    height = 0;
}

Node::Node()
{
    clearValues();
}

Node::Node(qint32 x, qint32 y, qint32 z)
{
    clearValues();
    this->x = x;
    this->y = y;
    this->z = z;
}

Node::Node(QXmlStreamReader* xmlReader)
{
    clearValues();

    while (!xmlReader->atEnd())
    {
        if(xmlReader->isEndElement())
        {
            xmlReader->readNext();
            break;
        }
        else if(xmlReader->isStartElement())
        {
            if (xmlReader->name() == "x")
                x = xmlReader->readElementText().toInt();
            else if (xmlReader->name() == "y")
                y = xmlReader->readElementText().toInt();
            else if (xmlReader->name() == "z")
                z = xmlReader->readElementText().toInt();
            else if (xmlReader->name() == "areaId")
                areaId = xmlReader->readElementText().toUInt();
            else if (xmlReader->name() == "iconId")
                iconId = xmlReader->readElementText().toUInt();
            else if (xmlReader->name() == "settings")
                settings = xmlReader->readElementText().toUInt();
            else if (xmlReader->name() == "unk0")
                unk0 = xmlReader->readElementText().toUInt();
            else if (xmlReader->name() == "unk1")
                unk1 = xmlReader->readElementText().toUInt();
            else if (xmlReader->name() == "unlockedPaths")
            {
                QStringList vals = xmlReader->readElementText().split("/");
                for (int i = 0; i < 4; i++)
                    unlockedPaths[i] = vals.at(i).toUInt();
            }
            else if (xmlReader->name() == "unlockedPathsSecret")
            {
                QStringList vals = xmlReader->readElementText().split("/");
                for (int i = 0; i < 4; i++)
                    unlockedPathsSecret[i] = vals.at(i).toUInt();
            }
            else if (xmlReader->name() == "cameraScroll")
                cameraScroll = xmlReader->readElementText().toUInt();
            else if (xmlReader->name() == "cameraScrollSecret")
                cameraScrollSecret = xmlReader->readElementText().toUInt();
        }
        xmlReader->readNext();
    }
}

void Node::clearValues()
{
    this->x = 0;
    this->y = 0;
    this->z = 0;
    this->areaId = 0;
    this->iconId = 0;
    this->settings = 0;
    this->unk0 = 0;
    this->unk1 = 0;
    std::fill(unlockedPaths, unlockedPaths+4, 0);
    std::fill(unlockedPathsSecret, unlockedPathsSecret+4, 0);
    this->cameraScroll = 0;
    this->cameraScrollSecret = 0;
}

void Node::writeXml(QXmlStreamWriter* xmlWriter)
{
    xmlWriter->writeStartElement("node");
    xmlWriter->writeTextElement("x", QString::number(x));
    xmlWriter->writeTextElement("y", QString::number(y));
    xmlWriter->writeTextElement("z", QString::number(z));
    xmlWriter->writeTextElement("areaId", QString::number(areaId));
    xmlWriter->writeTextElement("iconId", QString::number(iconId));
    xmlWriter->writeTextElement("settings", QString::number(settings));
    xmlWriter->writeTextElement("unlockedPaths", QString("%1/%2/%3/%4").arg(unlockedPaths[0]).arg(unlockedPaths[1]).arg(unlockedPaths[2]).arg(unlockedPaths[3]));
    xmlWriter->writeTextElement("unlockedPathsSecret", QString("%1/%2/%3/%4").arg(unlockedPathsSecret[0]).arg(unlockedPathsSecret[1]).arg(unlockedPathsSecret[2]).arg(unlockedPathsSecret[3]));
    xmlWriter->writeTextElement("cameraScroll", QString::number(cameraScroll));
    xmlWriter->writeTextElement("cameraScrollSecret", QString::number(cameraScrollSecret));
    xmlWriter->writeTextElement("unk0", QString::number(unk0));
    xmlWriter->writeTextElement("unk1", QString::number(unk1));
    xmlWriter->writeEndElement();
}

int Node::getBowserJrPath()
{
    bool tower = (settings & 0x2) != 0;
    bool castle = (settings & 0x4) != 0;
    bool tower2 = (settings & 0x20) != 0;
    bool castle2 = (settings & 0x40) != 0;

    if (tower && !castle && !tower2 && !castle2)
        return 1;
    else if (!tower && castle && !tower2 && !castle2)
        return 2;
    else if (!tower && !castle && tower2 && !castle2)
        return 3;
    else if (!tower && !castle && !tower2 && castle2)
        return 4;
    else
        return 0;
}

void Node::setBowserJrPath(int index)
{
    settings &= 0x99;
    if (index == 1)
        settings |= 0x2;
    else if (index == 2)
        settings |= 0x4;
    else if (index == 3)
        settings |= 0x20;
    else if (index == 4)
        settings |= 0x40;
}


PathBehavior::PathBehavior()
{
    clearValues();
}

PathBehavior::PathBehavior(QXmlStreamReader *xmlReader)
{
    clearValues();

    while (!xmlReader->atEnd())
    {
        if(xmlReader->isEndElement())
        {
            xmlReader->readNext();
            break;
        }
        else if(xmlReader->isStartElement())
        {
            if (xmlReader->name() == "animationId")
                animationId = xmlReader->readElementText().toInt();
            else if (xmlReader->name() == "starCoinCost")
                starCoinCost = xmlReader->readElementText().toInt();
            else if (xmlReader->name() == "settings")
                settings = xmlReader->readElementText().toInt();
        }
        xmlReader->readNext();
    }
}

void PathBehavior::clearValues()
{
    animationId = 0;
    starCoinCost = 0;
    settings = 0;
}

void PathBehavior::writeXml(QXmlStreamWriter *xmlWriter)
{
    xmlWriter->writeStartElement("pathSetting");
    xmlWriter->writeTextElement("animationId", QString::number(animationId));
    xmlWriter->writeTextElement("starCoinCost", QString::number(starCoinCost));
    xmlWriter->writeTextElement("settings", QString::number(settings));
    xmlWriter->writeEndElement();
}
