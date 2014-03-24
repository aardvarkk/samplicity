#ifndef DIRECTORIESMODEL_H
#define DIRECTORIESMODEL_H

#include <QAbstractItemModel>

#include "database.h"

class TreeItem
{
public:
    explicit TreeItem(const QList<QVariant> &data, TreeItem* parent = nullptr);
    ~TreeItem();

    void appendChild(TreeItem *child);

    TreeItem* child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    TreeItem* parent();

private:
    QList<TreeItem*> childItems;
    QList<QVariant> itemData;
    TreeItem* parentItem;
};

class DirectoriesModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    DirectoriesModel(Database const& db);
    ~DirectoriesModel();

    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &index) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    void addTree(TreeItem* parent, Database const& db);

    Database const& db;
    TreeItem* rootItem;
};

#endif // DIRECTORIESMODEL_H
