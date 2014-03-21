#include "directories_model.h"

DirectoriesModel::DirectoriesModel(Database const& db) : db(db)
{
}

QModelIndex DirectoriesModel::index(int row, int column, const QModelIndex &parent) const
{
    return QModelIndex();
}

QModelIndex DirectoriesModel::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

int DirectoriesModel::rowCount(const QModelIndex &parent) const
{
    return 1;
}

int DirectoriesModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant DirectoriesModel::data(const QModelIndex &index, int role) const
{
    return QVariant(0);
}

