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
    QList<Sample> getSamples(QList<QDir> const* filterDirs = nullptr) const;

    void addTag(QString const& name, int parent_id = 0);

    // We need a parent ID because it's the combination of name and parent that defines a tag
    // Can have the same named tag sitting under different parents ("high" under "lead" and "high" under "pad")
    Tag getTag(QString const& name, int parent_id = 0);
    Tag getTag(int id);

    void renameTag(Tag& tag, QString const& newName);

    QList<Tag> getTags() const;

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
