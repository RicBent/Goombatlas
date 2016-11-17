#include "mode.h"

Mode::Mode(QString name)
{
    this->name = name;
    active = true;
}

void Mode::activate()
{
    if (active)
        return;

    for (int i = 0; i < dockWidgets.length(); i++)
    {
        dockWidgets.at(i)->setHidden(!lastVisibilities.at(i));
        dockWidgets.at(i)->toggleViewAction()->setEnabled(true);
    }

    active = true;
}

void Mode::deactivate()
{
    if (!active)
        return;

    for (int i = 0; i < dockWidgets.length(); i++)
        lastVisibilities[i] = !dockWidgets.at(i)->isHidden();

    foreach (QDockWidget* d, dockWidgets)
    {
        d->setHidden(true);
        d->toggleViewAction()->setEnabled(false);
    }

    active = false;
}

void Mode::addDock(QDockWidget* dock)
{
    dockWidgets.append(dock);
    lastVisibilities.append(true);
}
