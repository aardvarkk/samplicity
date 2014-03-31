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

    // Create our tree structure from the database
    addTagWrappers(db.getTagChildren(), this->tags);
}

TagsModel::~TagsModel()
{
    delete tags;
}

QModelIndex TagsModel::index(int row, int column, const QModelIndex &parent) const
{
    qDebug() << __FUNCSIG__;
    qDebug() << row << column << parent;

    if (parent.isValid()) {
        auto w = static_cast<TagWrapper*>(parent.internalPointer());
        return createIndex(row, column, &w->children[row]);
    }

    if (row < tags->children.count()) {
        qDebug() << &tags->children[row];
        qDebug() << tags->children[row].tag.name;
        auto ptr = &tags->children[row];
        return createIndex(row, column, ptr);
    }

    return QModelIndex();
}

QModelIndex TagsModel::parent(const QModelIndex &index) const
{
    qDebug() << __FUNCSIG__;
    qDebug() << index;

    if (!index.isValid()) {
        qDebug() << QModelIndex();
        return QModelIndex();
    }

    auto w = static_cast<TagWrapper*>(index.internalPointer());
    auto parent = w->parent;

    if (parent->row >= 0) {
        qDebug() << createIndex(parent->row, 0, parent);
        return createIndex(parent->row, 0, parent);
    } else {
        qDebug() << QModelIndex();
        return QModelIndex();
    }
}

int TagsModel::rowCount(const QModelIndex &parent) const
{
    qDebug() << __FUNCSIG__;
    qDebug() << parent;

    if (!parent.isValid()) {
        qDebug() << tags->children.count();
        return tags->children.count();
    } else {
        auto w = static_cast<TagWrapper*>(parent.internalPointer());
        qDebug() << w->children.count();
        return w->children.count();
    }
}

int TagsModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant TagsModel::data(const QModelIndex &index, int role) const
{
    qDebug() << __FUNCSIG__;

    if (!index.isValid()) {
        qDebug() << QVariant();
        return QVariant();
    }

    auto w = static_cast<TagWrapper*>(index.internalPointer());

    qDebug() << w->tag.name;
    return w->tag.name;
}

QVariant TagsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return "Tags";
    }

    return QVariant();
}


