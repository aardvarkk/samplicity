#ifndef SAMPLES_MODEL_H
#define SAMPLES_MODEL_H

#include <QAbstractListModel>
#include <QHash>
#include "database.h"
#include "sample.h"

class SamplesModel : public QAbstractListModel
{
public:
    SamplesModel(Database const& db);
    ~SamplesModel();

    virtual QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const;

private:
    QList<Sample*> items;
    Database const& db;
};

#endif // SAMPLES_MODEL_H
