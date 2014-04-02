#ifndef EXPANDING_TREE_VIEW_H
#define EXPANDING_TREE_VIEW_H

#include <QTreeView>

class ExpandingTreeView : public QTreeView
{
    Q_OBJECT

public:
    ExpandingTreeView(QWidget* parent = 0);
    virtual ~ExpandingTreeView() {}

protected:
    virtual void reset();
};

#endif // EXPANDING_TREE_VIEW_H
