#include "cstreamwriter.h"

CStreamWriter::CStreamWriter(QFile* file)
{
    this->file = file;
    tabCount = 0;

    stream = new QTextStream(file);
}

void CStreamWriter::writeEndl()
{
    *stream << endl;
}

void CStreamWriter::writeInclude(QString file)
{
    *stream << "#include \"" << file << "\"" << endl;
}


void CStreamWriter::writeNode(Node* n, QString prefix, int index, bool comma)
{
    *stream << "\t{ &" << prefix << "_path_" << index << ", " << n->getAreaId() << ", " << n->getUnk0()
            << ", " << n->getUnk1() << ", " << n->getIconId() << ", " << n->getSettings() << ", {0, 0, 0} }";

    if (comma)
        *stream << ",";
    *stream << endl;
}

void CStreamWriter::writeNodes(QList<Node*>* nodes, QString prefix)
{
    writePaths(nodes, prefix);

    *stream << "node " << prefix << "_nodes[] =" << endl << "{" << endl;

    for (int i = 0; i < nodes->length(); i++)
    {
        Node* n = nodes->at(i);
        bool comma = (i != (nodes->length()-1));
        writeNode(n, prefix, i, comma);
    }

    *stream << "};" << endl;
}


void CStreamWriter::writePaths(QList<Node *> *nodes, QString prefix)
{
    for (int i = 0; i < nodes->length(); i++)
    {
        Node* n = nodes->at(i);

        *stream << "path " << prefix << "_path_" << i << " =" << endl << "{" << endl;

        for (int j = 0; j < n->pathList()->count(); j++)
        {
            Path* p = n->pathList()->at(j);
            *stream << "\t{" << p->endingNodeId << ", " << p->animationId << ", " << p->settings << ", 0}," << endl;
        }

        *stream << "\t{255, 0, 0, 0}" << endl << "};" << endl << endl;
    }
}


void CStreamWriter::writeVisibleNode(Node* n, bool comma)
{
    *stream << "\t{ "
            << "{" << n->getUnlock(0) << ", " << n->getUnlock(1) << ", " << n->getUnlock(2) << ", " << n->getUnlock(3) << "}, "
            << "{" << n->getUnlockSecret(0) << ", " << n->getUnlockSecret(1) << ", " << n->getUnlockSecret(2) << ", " << n->getUnlockSecret(3) << "}, "
            << n->getCameraScroll() << ", "
            << n->getCameraScrollSecret() << ", "
            << n->getx() << ", "
            << n->gety() << ", "
            << n->getz()
            << " }";

    if (comma)
        *stream << ",";
    *stream << endl;
}

void CStreamWriter::writeVisibleNodes(QList<Node*>* nodes, QString prefix)
{
    *stream << "visibleNode " << prefix << "_visibleNodes[] =" << endl << "{" << endl;

    for (int i = 0; i < nodes->length(); i++)
    {
        Node* n = nodes->at(i);
        bool comma = (i != (nodes->length()-1));
        writeVisibleNode(n, comma);
    }

    *stream << "};" << endl;
}


void CStreamWriter::writePathBehavior(PathBehavior* p, bool comma)
{
    *stream << "\t{" << p->getAnimationId() << ", " << p->getStarCoinCost() << ", " << p->getSettings() << ", 0}";
    if (comma)
        *stream << ",";
    *stream << endl;
}

void CStreamWriter::writePathBehaviors(QList<PathBehavior*>* paths, QString prefix)
{
    *stream << "pathBehavior " << prefix << "_pathBehaviors[] =" << endl << "{" << endl;

    for (int i = 0; i < paths->length(); i++)
    {
        PathBehavior* p = paths->at(i);
        bool comma = (i != (paths->length()-1));
        writePathBehavior(p, comma);
    }

    *stream << "};" << endl;
}


void CStreamWriter::writeMapObject(MapObject* mapObject)
{
     *stream << "\t{ " << mapObject->getPathBehaviorId() << ", {0, 0, 0}, " << mapObject->getx()*16 << ", " << mapObject->gety()*16 << ", " << mapObject->getz()*16 << " }," << endl;
}

void CStreamWriter::writeMapObjects(QList<MapObject*>* mapObjects, quint8 type, QString prefix)
{
    QString typeName;
    if (type == 1)
        typeName = "towersCastles";
    else if (type == 2)
        typeName = "mushroomHouses";
    else
        typeName = "starCoinSigns";

    *stream << "pathBehavior " << prefix << "_" << typeName << "[] =" << endl << "{" << endl;

    for (int i = 0; i < mapObjects->length(); i++)
    {
        MapObject* o = mapObjects->at(i);
        writeMapObject(o);
    }

    *stream << "\t{ 0xFF, {0xFF, 0xFF, 0xFF}, 0, 0, 0 }" << endl;
    *stream << "};" << endl;
}
