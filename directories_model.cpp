#include "directories_model.h"

TreeItem::TreeItem(const QList<QVariant> &data, TreeItem* parent)
{
    parentItem = parent;
    itemData = data;
}

TreeItem::~TreeItem()
{
    qDeleteAll(childItems);
}

void TreeItem::appendChild(TreeItem* item)
{
    childItems.append(item);
}

TreeItem* TreeItem::child(int row)
{
    return childItems.value(row);
}

int TreeItem::childCount() const
{
    return childItems.count();
}

int TreeItem::row() const
{
    if (parentItem) {
        return parentItem->childItems.indexOf(const_cast<TreeItem*>(this));
    }

    return 0;
}

int TreeItem::columnCount() const
{
    return itemData.count();
}

QVariant TreeItem::data(int column) const
{
    return itemData.value(column);
}

TreeItem* TreeItem::parent()
{
    return parentItem;
}

DirectoriesModel::DirectoriesModel(Database const& db) : db(db)
{
    QList<QVariant> rootData;
    rootData << tr("Directory");
    rootItem = new TreeItem(rootData);
    addTree(rootItem, db);
}

void DirectoriesModel::addTree(TreeItem* parent, Database const& db)
{
    QList<QDir> children = parent == rootItem ? db.getChildren(nullptr) : db.getChildren(&QDir(parent->data(0).toString()));

    // Add the children, then recursively add all of their children
    for (auto child : children) {
        QList<QVariant> data;
        data << QVariant(child.absolutePath());
        auto childItem = new TreeItem(data, parent);
        parent->appendChild(childItem);
        addTree(childItem, db);
    }
}

DirectoriesModel::~DirectoriesModel()
{
    delete rootItem;
}

QModelIndex DirectoriesModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    TreeItem* parentItem;

    if (!parent.isValid()) {
        parentItem = rootItem;
    } else {
        parentItem = static_cast<TreeItem*>(parent.internalPointer());
    }

    TreeItem* childItem = parentItem->child(row);
    if (childItem) {
        return createIndex(row, column, childItem);
    } else {
        return QModelIndex();
    }
}

QModelIndex DirectoriesModel::parent(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return QModelIndex();
    }

    TreeItem* childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem* parentItem = childItem->parent();

    if (parentItem == rootItem) {
        return QModelIndex();
    }

    return createIndex(parentItem->row(), 0, parentItem);
}

int DirectoriesModel::rowCount(const QModelIndex &parent) const
{
    TreeItem* parentItem;
    if (parent.column() > 0) {
        return 0;
    }

    if (!parent.isValid()) {
        parentItem = rootItem;
    } else {
        parentItem = static_cast<TreeItem*>(parent.internalPointer());
    }

    return parentItem->childCount();
}

int DirectoriesModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    } else {
        return rootItem->columnCount();
    }
}

QVariant DirectoriesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    TreeItem* item = static_cast<TreeItem*>(index.internalPointer());

    return item->data(index.column());
}

Qt::ItemFlags DirectoriesModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}

QVariant DirectoriesModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return rootItem->data(section);
    }

    return QVariant();
}