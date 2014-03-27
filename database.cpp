#include "database.h"
#include "filesystem.h"
#include "utils.h"

Database::Database(QString const& filename)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("localhost");
    db.setDatabaseName(filename);
    if (!db.open())
    {
        throw std::exception("Unable to create database");
    }

    if (!createTables()) {
        throw std::exception("Unable to create database tables");
    }

    // Could remove this -- it just sanitizes/checks the database upon opening it up
    // Not strictly necessary...
    // cleanup();
}

void Database::addTag(QString const& name, int* parent_id)
{
    QSqlQuery query;
    query.prepare("INSERT OR IGNORE INTO tags (name, parent_id) VALUES (?,?)");
    query.addBindValue(name);
    query.addBindValue(parent_id ? *parent_id : QVariant(QVariant::Int));
    query.exec();
}

Tag Database::getTag(QString const& name, int parent_id)
{
    Tag tag;

    QSqlQuery query;
    if (parent_id > 0) {
        query.prepare("SELECT id, parent_id, name FROM tags WHERE name = ? AND parent_id = ?");
        query.addBindValue(name);
        query.addBindValue(parent_id);
    } else {
        query.prepare("SELECT id, parent_id, name FROM tags WHERE name = ? AND parent_id IS NULL");
        query.addBindValue(name);
    }
    query.exec();

    while (query.next()) {
        tag = Tag(
            query.value(0).toInt(),
            query.value(1).toInt(),
            query.value(2).toString()
            );
    }
    return tag;
}

QList<Tag> Database::getTags() const
{
    QList<Tag> tags;
    QSqlQuery query;
    query.exec("SELECT id, parent_id, name FROM tags");

    while (query.next()) {
        tags << Tag(
            query.value(0).toInt(),
            query.value(1).toInt(),
            query.value(2).toString()
                    );
    }

    return tags;
}

bool Database::createTables()
{
    auto success = true;
    QSqlQuery query;
    success &= query.exec("CREATE TABLE IF NOT EXISTS dirs (id INTEGER PRIMARY KEY, parent_id INTEGER, path TEXT UNIQUE)");
    success &= query.exec("CREATE TABLE IF NOT EXISTS samples (id INTEGER PRIMARY KEY, dir_id INTEGER, name TEXT, filename TEXT, UNIQUE (dir_id, name))");
    success &= query.exec("CREATE TABLE IF NOT EXISTS tags (id INTEGER PRIMARY KEY, parent_id INTEGER, name TEXT, UNIQUE (parent_id, name))");
    return success;
}

bool Database::addFile(QFile const& file)
{
    qDebug() << __FUNCSIG__;
    qDebug() << file.fileName();

    auto fileInfo = QFileInfo(file);

    // Transaction-based if we're only adding a file
    // If we're adding a directory, the directory makes the transaction
    if (QObject::sender() == nullptr) {
        if (!Utils::isSupportedExtension(fileInfo.suffix())) {
            return false;
        }
        db.transaction();
    }

    // Create all parents, from base to tip
    auto dir = fileInfo.absoluteDir();
    QStringList parents;
    do {
        parents << dir.absolutePath();
    } while (dir.cdUp());

    QSqlQuery query;
    auto it = parents.constEnd();
    int parent_id = -1;
    while (it != parents.constBegin()) {
        --it;
        query.prepare("INSERT OR IGNORE INTO dirs (parent_id, path) VALUES (?,?)");
        query.addBindValue(parent_id >= 0 ? parent_id : QVariant(QVariant::Int));
        query.addBindValue(*it);
        query.exec();

        // Get the relevant parent for this directory
        query.prepare("SELECT id FROM dirs WHERE path = ?");
        query.addBindValue(*it);
        query.exec();
        while (query.next()) {
            parent_id = query.value(0).toInt();
        }
    }

    // Get the relevant parent for this file
    query.prepare("SELECT id FROM dirs WHERE path = ?");
    query.addBindValue(fileInfo.dir().absolutePath());
    query.exec();
    int dir_id;
    while (query.next()) {
        dir_id = query.value(0).toInt();
    }

    // Add a new sample record
    query.prepare("INSERT INTO samples (dir_id, name, filename) VALUES (?,?,?)");
    query.addBindValue(dir_id);
    query.addBindValue(fileInfo.fileName());
    query.addBindValue(fileInfo.fileName());
    auto success = query.exec();

    if (QObject::sender() == nullptr) {
        success ? db.commit() : db.rollback();
    }

    return true;
}

bool Database::removeFile(QFile const& file)
{
    qDebug() << __FUNCSIG__;
    qDebug() << file.fileName();

    // Remove based on filename AND location
    auto fileInfo = QFileInfo(file);

    QSqlQuery query;
    query.prepare("DELETE FROM samples WHERE filename = ? AND dir_id IN (SELECT id FROM dirs WHERE path = ?)");
    query.addBindValue(fileInfo.fileName());
    query.addBindValue(fileInfo.dir().absolutePath());
    query.exec();

    qDebug() << query.executedQuery();

    return true;
}

bool Database::addDirectory(QDir const& dir)
{
    qDebug() << __FUNCSIG__;
    qDebug() << dir;

    db.transaction();
    Filesystem fs;
    QObject::connect(&fs, SIGNAL(foundFile(QFile)), this, SLOT(addFile(QFile)));
    auto success = fs.findFiles(dir, &Utils::getSupportedNameFilters()) ? db.commit() : db.rollback();
    return success;
}

bool Database::removeDirectory(const QDir &dir)
{
    qDebug() << __FUNCSIG__;
    qDebug() << dir;

    db.transaction();
    Filesystem fs;
    QObject::connect(&fs, SIGNAL(foundFile(QFile)), this, SLOT(removeFile(QFile)));
    auto success = fs.findFiles(dir, &Utils::getSupportedNameFilters()) ? db.commit() : db.rollback();
    return success;
}

QList<QDir> Database::getDirectoryChildren(QDir const* dir) const
{
    QList<QDir> children;

    QSqlQuery query;
    if (dir) {
        QSqlQuery getID;
        getID.prepare("SELECT id FROM dirs WHERE path = ?");
        getID.addBindValue(dir->absolutePath());
        getID.exec();
        if (getID.next()) {
            query.prepare("SELECT path FROM dirs WHERE parent_id = ?");
            query.addBindValue(getID.value(0));
            query.exec();
        }
    } else {
        query.exec("SELECT path FROM dirs WHERE parent_id IS NULL");
    }

    while (query.next()) {
        children << QDir(query.value(0).toString());
    }

    return children;
}

void addChildren(int dir_id, QStringList& ids) {
    // Add ourselves
    ids << QString::number(dir_id);

    // Add all of our children (items who have us as a parent)
    QSqlQuery query;
    query.prepare("SELECT id FROM dirs WHERE parent_id = ?");
    query.addBindValue(dir_id);
    query.exec();
    while (query.next()) {
        addChildren(query.value(0).toInt(), ids);
    }
}

QList<Sample> Database::getSamples(QList<QDir> const* filterDirs) const
{
    QList<Sample> samples;
    QSqlQuery query;

    if (filterDirs) {
        // Create a list of all IDs that would be valid parents for samples based on given dirs
        QStringList ids;
        for (auto filterDir : *filterDirs) {
            QSqlQuery getID;
            getID.prepare("SELECT id FROM dirs WHERE path = ?");
            getID.addBindValue(filterDir.absolutePath());
            getID.exec();
            while (getID.next()) {
                addChildren(getID.value(0).toInt(), ids);
            }
        }

        // Get all matching samples
        query.exec("SELECT id, dir_id, name, filename, path FROM samples JOIN dirs ON samples.dir_id = dirs.id WHERE dir_id IN (" + ids.join(",") + ")");
    } else {
        query.exec("SELECT id, dir_id, name, filename, path FROM samples JOIN dirs ON samples.dir_id = dirs.id");
    }

    while (query.next()) {
        samples << Sample(
            query.value(0).toInt(),
            query.value(1).toInt(),
            query.value(2).toString(),
            query.value(3).toString(),
            query.value(4).toString()
            );
    }

    return samples;
}

// Remove any orphaned dirs (dirs with no samples in them)
void Database::cleanup()
{
    QSqlQuery query;

    // We'll break out when there's no longer any empty leaf directories
    for (;;) {

        // Leaf directories (they are not a parent to anybody/they have no children)
        QSet<int> leaves;

        query.exec("SELECT dirs_left.id FROM dirs AS dirs_left LEFT JOIN dirs AS dirs_right ON dirs_left.id = dirs_right.parent_id WHERE dirs_right.id IS NULL GROUP BY dirs_left.id");
        while (query.next()) {
            leaves << query.value(0).toInt();
        }
        qDebug() << leaves;

        // Directories with no samples
        QSet<int> empties;
        query.exec("SELECT dirs.id FROM dirs LEFT JOIN samples ON samples.dir_id = dirs.id WHERE samples.id IS NULL");
        while (query.next()) {
            empties << query.value(0).toInt();
        }
        qDebug() << empties;

        QSet<int> toDelete = leaves.intersect(empties);
        qDebug() << toDelete;

        if (toDelete.empty()) {
            break;
        }

        for (auto deleteId : toDelete) {
            query.prepare("DELETE FROM dirs WHERE id = ?");
            query.addBindValue(deleteId);
            query.exec();
        }
    }
}

