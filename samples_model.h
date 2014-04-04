#ifndef SAMPLES_MODEL_H
#define SAMPLES_MODEL_H

#include <QAbstractListModel>
#include <QHash>
#include "database.h"
#include "sample.h"

class SamplesModel : public QAbstractListModel
{
    Q_OBJECT

public:
    SamplesModel(Database& db);
    ~SamplesModel();

    virtual QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const;

    void setFilter(
            QList<QDir> const& filterDirs,
            QList<Tag> const& filterTags
            );

    Sample* getSample(QModelIndex const& index);
    bool addRating(QModelIndex const& index, QVariant const& rating);
    bool adjustRating(QModelIndex const& index, int adjustment);

public slots:
    void reset();

private:
    void setSamples(QList<Sample> const* samples = nullptr);
    QList<Sample*> items;
    Database& db;
};

#endif // SAMPLES_MODEL_H
