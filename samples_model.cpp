#include "samples_model.h"

SamplesModel::SamplesModel(Database const& db) : db(db)
{
    setSamples(&db.getSamples(nullptr));
}

SamplesModel::~SamplesModel()
{
    setSamples(nullptr);
}

void SamplesModel::setSamples(QList<Sample> const* samples)
{
    beginResetModel();

    for (auto sample : items) {
        delete sample;
    }
    items.clear();

    if (samples) {
        for (auto sample : *samples) {
            items << new Sample(sample);
        }
    }

    endResetModel();
}

QModelIndex SamplesModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    return createIndex(row, column, items[row]);
}

int SamplesModel::rowCount(const QModelIndex &parent) const
{
    return items.count();
}

QVariant SamplesModel::data(const QModelIndex &index, int role) const
{
    auto sample = static_cast<Sample*>(index.internalPointer());
    if (!sample) {
        return QVariant();
    }

    switch (role) {
    case Qt::DisplayRole:
        return QVariant(sample->name);
        break;
    case Qt::EditRole:
        return QVariant(sample->path);
        break;
    default:
        return QVariant();
    }
}

QVariant SamplesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return QVariant("Samples");
    }

    return QVariant();
}

void SamplesModel::setFilterDirs(QList<QDir> const& filterDirs)
{
    qDebug() << __FUNCSIG__;
    qDebug() << filterDirs;

    setSamples(&db.getSamples(&filterDirs));
}
