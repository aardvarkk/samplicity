#ifndef DESELECTABLE_TREE_VIEW_H
#define DESELECTABLE_TREE_VIEW_H

#include <QMouseEvent>
#include <QTreeView>

class DeselectableTreeView : public QTreeView
{
    Q_OBJECT

public:
    DeselectableTreeView(QWidget* parent = 0);
    virtual ~DeselectableTreeView() {}

private:
    virtual void mousePressEvent(QMouseEvent *event);
};

#endif // DESELECTABLE_TREE_VIEW_H
