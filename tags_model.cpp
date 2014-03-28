#include "tags_model.h"
#include "tag.h"

TagsModel::TagsModel(Database& db) : db(db)
{
}

QModelIndex TagsModel::index(int row, int column, const QModelIndex &parent) const
{
    return QModelIndex();
}

QModelIndex TagsModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}

int TagsModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : 1;
}

int TagsModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : 1;
}

QVariant TagsModel::data(const QModelIndex &index, int role) const
{
    return QVariant();
}

QVariant TagsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return "Tags";
    }

    return QVariant();
}


