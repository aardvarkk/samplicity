#include "samples_model.h"

SamplesModel::SamplesModel(Database& db) : db(db)
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

Sample* SamplesModel::getSample(QModelIndex const& index)
{
    return static_cast<Sample*>(index.internalPointer());
}

bool SamplesModel::addRating(QModelIndex const& index, QVariant const& rating)
{
    auto sample = getSample(index);
    if (!sample) {
        return false;
    }

    auto success = db.addRating(*sample, rating);
    if (success) {
        db.getRating(*sample, sample->rating);
        auto changedIndex = this->index(index.row(), 1, index.parent());
        emit dataChanged(changedIndex, changedIndex);
    }

    return success;
}

bool SamplesModel::adjustRating(QModelIndex const& index, int adjustment)
{
    auto sample = getSample(index);
    if (!sample) {
        return false;
    }

    QVariant rating;
    auto success = db.getRating(*sample, rating);
    return addRating(index, rating == QVariant(QVariant::Int) ? 1 : rating.toInt()+adjustment);
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

int SamplesModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : 2;
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
        switch (index.column()) {
        case 0: return sample->name;
        case 1: return sample->rating;
        default: return QVariant();
        }

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
        switch (section) {
        case 0: return "Sample";
        case 1: return "Rating";
        default: return QVariant();
        }
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
