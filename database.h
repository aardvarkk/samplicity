#ifndef DATABASE_H
#define DATABASE_H

#include <QString>
#include <QtSql>
#include "sample.h"
#include "tag.h"

class Database : public QObject
{
    Q_OBJECT

public:
    Database();
    Database(QString const& filename);

    bool addDirectory(QDir const& dir);
    bool removeDirectory(QDir const& dir);
    QList<QDir> getDirectoryChildren(QDir const* dir) const;

    QList<Sample> getFilteredSamples(
            QList<QDir> const& filterDirs,
            QList<Tag> const& filterTags
            ) const;

    Sample getSample(QFile const& file);
    bool addSampleTag(Sample const& sample, Tag const& tag);
    bool removeSampleTag(Sample const& sample, Tag const& tag);
    QList<Tag> getSampleTags(Sample const& sample);

    Tag addTag(QString const& name, int parent_id = 0);
    // We need a parent ID because it's the combination of name and parent that defines a tag
    // Can have the same named tag sitting under different parents ("high" under "lead" and "high" under "pad")
    Tag getTag(QString const& name, int parent_id = 0);
    Tag getTag(int id);
    bool renameTag(Tag& tag, QString const& newName);
    QList<Tag> getTags() const;
    QList<Tag> getTagChildren(int parent_id = 0);
    QList<Tag> getTagDescendants(Tag const& parent);
    bool reparentTag(Tag& tag, int parent_id);
    QList<Tag> getTagAncestors(Tag const& tag);
    bool removeTag(Tag const& tag);
    Tag getTagChild(Tag const& parent, int index);
    int getTagChildIndex(Tag const& child);

    // Remove any orphaned dirs (dirs with no samples in them)
    void cleanup();

public slots:
    bool addFile(QFile const& file);
    bool removeFile(QFile const& file);

protected:
    bool createTables();

    QSqlDatabase db;
};

#endif // DATABASE_H
