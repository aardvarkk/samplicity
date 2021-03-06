#ifndef TAGS_MODEL_H
#define TAGS_MODEL_H

#include <QAbstractItemModel>

#include "database.h"
#include "tag.h"

struct TagWrapper
{
    TagWrapper() : parent(nullptr), row(-1) {}
    Tag tag;
    TagWrapper* parent;
    QVector<TagWrapper> children;
    int row;
};

class TagsModel : public QAbstractItemModel
{
    // A data role used to retrieve IDs
    enum
    {
        IDRole = Qt::UserRole
    };

public:
    TagsModel(Database& db);
    ~TagsModel();
    bool addTag(QString const& name, int parent_id = 0);
    bool removeTag(Tag const& tag);
    bool renameTag(Tag& tag, QString const& newName);
    bool reparentTag(Tag& tag, int parent_id);
    bool addSampleTag(Sample const& sample, Tag const& tag);
    bool removeSampleTag(Sample const& sample, Tag const& tag);
    void refresh();
    QModelIndex modelIndex(Tag const& tag);

    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &index) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    void addTagWrappers(QList<Tag> const& tags, TagWrapper* parent);

    Database& db;
    TagWrapper* tags;
};

#endif // TAGS_MODEL_H
