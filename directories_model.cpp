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

DirectoriesModel::DirectoriesModel(Database& db) : db(db)
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

    //Qt::DisplayRole	0	The key data to be rendered in the form of text. (QString)
    //Qt::DecorationRole1	The data to be rendered as a decoration in the form of an icon. (QColor, QIcon or QPixmap)
    //Qt::EditRole      2	The data in a form suitable for editing in an editor. (QString)
    //Qt::ToolTipRole	3	The data displayed in the item's tooltip. (QString)
    //Qt::StatusTipRole	4	The data displayed in the status bar. (QString)
    //Qt::WhatsThisRole	5	The data displayed for the item in "What's This?" mode. (QString)
    //Qt::SizeHintRole	13	The size hint for the item that will be supplied to views. (QSize)

    TreeItem* item = static_cast<TreeItem*>(index.internalPointer());

    // Used to get the "full path"
    if (role == Qt::EditRole) {
        return QDir(item->data(index.column()).toString()).absolutePath();
    }

    // Unsupported role
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    // Return only the current level
    // Doesn't work for base drives, so have to account for that
    QDir dir = QDir(item->data(index.column()).toString());
    return dir.dirName().length() <= 0 ? dir.absolutePath() : dir.dirName();
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

void DirectoriesModel::addDirectory(QString const& path)
{
    beginResetModel();
    db.addDirectory(path);
    endResetModel();
}

void DirectoriesModel::removeDirectory(QString const& path)
{
    beginResetModel();
    db.removeDirectory(path);
    endResetModel();
}
