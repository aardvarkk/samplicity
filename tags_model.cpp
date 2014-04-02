#include "tags_model.h"

void TagsModel::addTagWrappers(QList<Tag> const& tags, TagWrapper* parent)
{
    // Add all of the tags as children
    for (auto t : tags) {
        TagWrapper wrapper;
        wrapper.tag = t;
        wrapper.parent = parent;
        wrapper.row = parent->children.count();
        parent->children.push_back(wrapper);
    }

    // Add their children
    for (auto &w : parent->children) {
        addTagWrappers(db.getTagChildren(w.tag.id), &w);
    }
}

TagsModel::TagsModel(Database& db) : db(db)
{
    tags = new TagWrapper;
    refresh();
}

TagsModel::~TagsModel()
{
    delete tags;
}

void TagsModel::refresh()
{
    beginResetModel();

    // Create our tree structure from the database
    this->tags->children.clear();
    addTagWrappers(db.getTagChildren(), this->tags);

    endResetModel();
}

bool TagsModel::addTag(QString const& name, int parent_id)
{
    auto tag = db.addTag(name, parent_id);
    if (tag.valid()) {
        refresh();
        return true;
    }

    return false;
}

bool TagsModel::removeTag(Tag const& tag)
{
    auto success = db.removeTag(tag);
    refresh();
    return success;
}

bool TagsModel::renameTag(Tag& tag, QString const& newName)
{
    auto success = db.renameTag(tag, newName);
    refresh();
    return success;
}

bool TagsModel::reparentTag(Tag& tag, int parent_id)
{
    auto success = db.reparentTag(tag, parent_id);
    refresh();
    return success;
}

QModelIndex TagsModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return createIndex(row, column, &static_cast<TagWrapper*>(parent.internalPointer())->children[row]);
    }

    if (column == 0 && row >= 0 && row < tags->children.count()) {
        return createIndex(row, column, &tags->children[row]);
    }

    return QModelIndex();
}

QModelIndex TagsModel::parent(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return QModelIndex();
    }

    auto parent = static_cast<TagWrapper*>(index.internalPointer())->parent;
    return parent->row >= 0 ? createIndex(parent->row, 0, parent) : QModelIndex();
}

int TagsModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? static_cast<TagWrapper*>(parent.internalPointer())->children.count() : tags->children.count();
}

int TagsModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant TagsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole) {
        return QVariant();
    }

    return static_cast<TagWrapper*>(index.internalPointer())->tag.name;
}

QVariant TagsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return "Tags";
    }

    return QVariant();
}
