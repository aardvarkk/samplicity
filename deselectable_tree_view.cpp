#include <QDebug>

#include "deselectable_tree_view.h"

DeselectableTreeView::DeselectableTreeView(QWidget* parent) :
    QTreeView(parent)
{
}

void DeselectableTreeView::mousePressEvent(QMouseEvent *event)
{
    QModelIndex item = indexAt(event->pos());
    bool selected = selectionModel()->isSelected(indexAt(event->pos()));
    QTreeView::mousePressEvent(event);
    if ((item.row() == -1 && item.column() == -1) || selected)
    {
        clearSelection();
        const QModelIndex index;
        selectionModel()->setCurrentIndex(index, QItemSelectionModel::Select);
    }
}

void DeselectableTreeView::reset()
{
    QAbstractItemView::reset();
    expandAll();
}
