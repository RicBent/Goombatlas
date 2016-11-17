#ifndef MODE_H
#define MODE_H

#include <QDockWidget>
#include <QAction>

class Mode
{
public:
    Mode(QString name);

    void activate();
    void deactivate();
    void addDock(QDockWidget* dock);
    void addAction(QAction* action);

    QString getName() { return name; }

private:
    QList<QDockWidget*> dockWidgets;
    QList<bool> lastVisibilities;

    QString name;
    bool active;
};

#endif // MODE_H
