#include "tags_model.h"
#include "tag.h"

TagsModel::TagsModel(Database& db) : db(db)
{
}

QModelIndex TagsModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    if (parent.isValid()) {
        auto tag = static_cast<Tag*>(parent.internalPointer());
        return createIndex(row, column, &db.getTagChildren(tag->id)[row]);
    } else {
        return createIndex(row, column, &db.getTagChildren(0)[row]);
    }
}

QModelIndex TagsModel::parent(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return QModelIndex();
    }

    auto tag = static_cast<Tag*>(index.internalPointer());
    return createIndex(db.getTagChildIndex(*tag), 0, &db.getTag(tag->parent_id));
}

int TagsModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return db.getTagChildren(0).length();
    }

    auto tag = static_cast<Tag*>(parent.internalPointer());
    return db.getTagChildren(tag->id).length();
}

int TagsModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant TagsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }
    auto tag = static_cast<Tag*>(index.internalPointer());
    return tag->name;
}

QVariant TagsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return "Tags";
    }

    return QVariant();
}


