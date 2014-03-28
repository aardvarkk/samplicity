#ifndef TAGS_MODEL_H
#define TAGS_MODEL_H

#include <QAbstractItemModel>

#include "database.h"

class TagsModel : public QAbstractItemModel
{
public:
    TagsModel(Database& db);

    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &index) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    Database& db;
};

#endif // TAGS_MODEL_H
