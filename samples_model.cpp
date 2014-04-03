#include "samples_model.h"

SamplesModel::SamplesModel(Database const& db) : db(db)
{
    reset();
}

SamplesModel::~SamplesModel()
{
}

// A slot that's called for when other models change that affect us
// (i.e. the list of directories)
void SamplesModel::reset()
{
    setSamples(&db.getFilteredSamples(QList<QDir>(), QList<Tag>()));
}

Sample const* SamplesModel::getSample(QModelIndex const& index)
{
    return static_cast<Sample*>(index.internalPointer());
}

void SamplesModel::setSamples(QList<Sample> const* samples)
{
    for (auto sample : items) {
        delete sample;
    }
    items.clear();

    if (samples) {
        for (auto sample : *samples) {
            items << new Sample(sample);
        }
    }
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
    return parent.isValid() ? 0 : items.count();
}

QVariant SamplesModel::data(const QModelIndex &index, int role) const
{
    auto sample = static_cast<Sample*>(index.internalPointer());
    if (!sample) {
        return QVariant();
    }

    switch (role) {
    case Qt::DisplayRole:
        return QVariant(sample->filename);
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

void SamplesModel::setFilter(
        QList<QDir> const& filterDirs,
        QList<Tag> const& filterTags
        )
{
    beginResetModel();
    setSamples(&db.getFilteredSamples(filterDirs, filterTags));
    endResetModel();
}
