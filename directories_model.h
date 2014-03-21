#ifndef DIRECTORIESMODEL_H
#define DIRECTORIESMODEL_H

#include <QAbstractItemModel>

#include "database.h"

class DirectoriesModel : public QAbstractItemModel
{
public:
    DirectoriesModel(Database const& db);

    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &child) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

private:
    Database const& db;
};

#endif // DIRECTORIESMODEL_H
