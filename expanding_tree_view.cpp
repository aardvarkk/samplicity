#include <QDebug>

#include "expanding_tree_view.h"

ExpandingTreeView::ExpandingTreeView(QWidget* parent) :
    QTreeView(parent)
{
}

void ExpandingTreeView::reset()
{
    QAbstractItemView::reset();
    expandAll();
}
