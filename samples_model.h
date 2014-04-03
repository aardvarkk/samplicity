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
    SamplesModel(Database const& db);
    ~SamplesModel();
    void setSamples(QList<Sample> const* samples = nullptr);

    virtual QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const;

    void setFilter(
            QList<QDir> const& filterDirs,
            QList<Tag> const& filterTags
            );

    Sample const* getSample(QModelIndex const& index);

public slots:
    void reset();

private:
    QList<Sample*> items;
    Database const& db;
};

#endif // SAMPLES_MODEL_H
