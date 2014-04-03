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

bool Database::createTables()
{
    auto success = true;
    QSqlQuery query;
    success &= query.exec("CREATE TABLE IF NOT EXISTS dirs (id INTEGER PRIMARY KEY, parent_id INTEGER, path TEXT UNIQUE)");
    success &= query.exec("CREATE TABLE IF NOT EXISTS samples (id INTEGER PRIMARY KEY, dir_id INTEGER, name TEXT, filename TEXT, UNIQUE (dir_id, name))");
    success &= query.exec("CREATE TABLE IF NOT EXISTS tags (id INTEGER PRIMARY KEY, parent_id INTEGER, name TEXT, UNIQUE (parent_id, name))");
    success &= query.exec("CREATE TABLE IF NOT EXISTS sample_tags (id INTEGER PRIMARY KEY, sample_id INTEGER, tag_id INTEGER, UNIQUE (sample_id, tag_id))");
    return success;
}

Sample Database::getSample(QFile const& file)
{
    Sample sample;

    auto fileInfo = QFileInfo(file);

    QSqlQuery query;
    query.prepare("SELECT samples.id, samples.dir_id, samples.name, samples.filename, dirs.path FROM samples JOIN dirs ON samples.dir_id = dirs.id WHERE samples.filename = ? AND dirs.path = ?");
    query.addBindValue(fileInfo.fileName());
    query.addBindValue(fileInfo.dir().absolutePath());
    query.exec();
    while (query.next()) {
        sample = Sample(
                    query.value(0).toInt(),
                    query.value(1).toInt(),
                    query.value(2).toString(),
                    query.value(3).toString(),
                    query.value(4).toString()
                    );
    }

    return sample;
}

bool Database::addSampleTag(Sample const& sample, Tag const& tag)
{
    qDebug() << __FUNCSIG__;
    qDebug() << "Add tag" << tag.id << "to sample" << sample.id;

    QSqlQuery query;
    query.prepare("INSERT OR IGNORE INTO sample_tags (sample_id, tag_id) VALUES (?,?)");
    query.addBindValue(sample.id);
    query.addBindValue(tag.id);
    return query.exec();
}

bool Database::removeSampleTag(Sample const& sample, Tag const& tag)
{
    qDebug() << __FUNCSIG__;
    qDebug() << "Remove tag" << tag.id << "from sample" << sample.id;

    QSqlQuery query;
    query.prepare("DELETE FROM sample_tags WHERE sample_id = ? AND tag_id = ?");
    query.addBindValue(sample.id);
    query.addBindValue(tag.id);
    return query.exec();
}

QList<Tag> Database::getSampleTags(Sample const& sample)
{
    QList<Tag> tags;
    QSqlQuery query;
    query.prepare("SELECT tags.id, tags.parent_id, tags.name FROM (samples JOIN sample_tags ON samples.id = sample_tags.sample_id) JOIN tags ON sample_tags.tag_id = tags.id  WHERE samples.id = ?");
    query.addBindValue(sample.id);
    query.exec();
    while (query.next()) {
        tags << Tag(
                    query.value(0).toInt(),
                    query.value(1).toInt(),
                    query.value(2).toString()
                    );
    }

    return tags;
}

Tag Database::addTag(QString const& name, int parent_id)
{
    Tag added;

    QSqlQuery query;
    query.prepare("INSERT OR IGNORE INTO tags (name, parent_id) VALUES (?,?)");
    query.addBindValue(name);
    query.addBindValue(parent_id > 0 ? parent_id : QVariant(QVariant::Int));
    if (query.exec()) {
        return added = getTag(name, parent_id);
    }

    return added;
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

Tag Database::getTag(int id)
{
    Tag tag;

    QSqlQuery query;
    query.prepare("SELECT parent_id, name FROM tags WHERE id = ?");
    query.addBindValue(id);
    query.exec();

    while (query.next()) {
        tag = Tag(
            id,
            query.value(0).toInt(),
            query.value(1).toString()
            );
    }
    return tag;
}

bool Database::renameTag(Tag& tag, QString const& newName)
{
    QSqlQuery query;
    query.prepare("UPDATE tags SET name = ? WHERE id = ?");
    query.addBindValue(newName);
    query.addBindValue(tag.id);
    if (query.exec()) {
        tag.name = newName;
        return true;
    }
    return false;
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

QList<Tag> Database::getTagChildren(int parent_id)
{
    QList<Tag> tags;
    QSqlQuery query;
    if (parent_id > 0) {
        query.prepare("SELECT id, parent_id, name FROM tags WHERE parent_id = ?");
        query.addBindValue(parent_id);
    } else {
        query.prepare("SELECT id, parent_id, name FROM tags WHERE parent_id IS NULL");
    }
    query.exec();

    while (query.next()) {
        tags << Tag(
            query.value(0).toInt(),
            query.value(1).toInt(),
            query.value(2).toString()
                    );
    }

    return tags;
}

QList<Tag> Database::getTagDescendants(Tag const& parent)
{
    QList<Tag> descendants;
    descendants << getTagChildren(parent.id);
    for (auto c : getTagChildren(parent.id)) {
        descendants << getTagDescendants(c);
    }
    return descendants;
}

QList<Tag> Database::getTagAncestors(Tag const& tag)
{
    QList<Tag> ancestors;

    // Keep walking up the parent list and adding to ancestors
    Tag check(tag);
    while (check.parent_id > 0) {
        check = getTag(check.parent_id);
        ancestors << check;
    }

    return ancestors;
}

bool Database::removeTag(Tag const& tag)
{
    qDebug() << __FUNCSIG__;

    bool success = true;

    // Remove our child tags first, then ourselves
    for (auto c : getTagChildren(tag.id)) {
//        qDebug() << "Remove child tag " << tag.id;
        success &= removeTag(c);
    }

    QSqlQuery query;

    // Remove ourselves
    // Untag everything relevant
    query.prepare("DELETE FROM sample_tags WHERE tag_id = ?");
    query.addBindValue(tag.id);
    success &= query.exec();

//    qDebug() << "Remove tag " << tag.id;

    // Remove the actual tag itself
    query.prepare("DELETE FROM tags WHERE id = ?");
    query.addBindValue(tag.id);
    success &= query.exec();

    return success;
}

Tag Database::getTagChild(Tag const& parent, int index)
{
    Tag tag;

    QSqlQuery query;
    query.prepare("SELECT id, parent_id, name FROM tags WHERE parent_id = ?");
    query.addBindValue(parent.id);
    query.exec();
    while (query.next()) {
        if (index-- == 0) {
            tag = Tag(
                        query.value(0).toInt(),
                        query.value(1).toInt(),
                        query.value(2).toString());
        }
    }

    return tag;
}

int Database::getTagChildIndex(Tag const& child)
{
    QSqlQuery query;
    query.prepare("SELECT id FROM tags WHERE parent_id = ?");
    query.addBindValue(child.parent_id);
    query.exec();
    int index = 0;
    while (query.next()) {
        if (query.value(0).toInt() == child.id) {
            break;
        }
        ++index;
    }
    return index;
}

bool Database::reparentTag(Tag& tag, int parent_id)
{
    qDebug() << __FUNCSIG__;

    // Can't reparent to self
    if (tag.id == parent_id) {
        return false;
    }

    // Check if the tag given is a parent of the tag with the given parent_id
    // If so, this is invalid as it will create a circular reference
    auto parent = getTag(parent_id);
    auto ancestors = getTagAncestors(parent);

    // Make sure our tag is not in the ancestors list of the supposed new parent
    for (auto a : ancestors) {
        if (a.id == tag.id) {
            return false;
        }
    }

    // Update it!
    // If parent_id is 0, we want to set a null parent
    QSqlQuery query;
    query.prepare("UPDATE tags SET parent_id = ? WHERE id = ?");
    query.addBindValue(parent_id > 0 ? parent_id : QVariant(QVariant::Int));
    query.addBindValue(tag.id);
    if (query.exec()) {
        tag.parent_id = parent_id;
        return true;
    }

    return false;
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
        query.addBindValue(parent_id > 0 ? parent_id : QVariant(QVariant::Int));
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

void addDirectoryChildren(int dirID, QStringList& dirIDs) {
    // Add ourselves
    dirIDs << QString::number(dirID);

    // Add all of our children (items who have us as a parent)
    QSqlQuery query;
    query.prepare("SELECT id FROM dirs WHERE parent_id = ?");
    query.addBindValue(dirID);
    query.exec();
    while (query.next()) {
        addDirectoryChildren(query.value(0).toInt(), dirIDs);
    }
}

void addTagChildren(int tagID, QStringList& sampleIDs) {
    // Add ourselves
    sampleIDs << QString::number(tagID);

    // Add all of our children (items who have us as a parent)
    QSqlQuery query;
    query.prepare("SELECT id FROM tags WHERE parent_id = ?");
    query.addBindValue(tagID);
    query.exec();
    while (query.next()) {
        addTagChildren(query.value(0).toInt(), sampleIDs);
    }
}

QList<Sample> Database::getFilteredSamples(
        QList<QDir> const& filterDirs,
        QList<Tag> const& filterTags
        ) const
{
    QList<Sample> samples;
    QSqlQuery query;

    // Create a list of all IDs that would be valid parents for samples based on given dirs
    QStringList dirIDs;
    for (auto filterDir : filterDirs) {
        QSqlQuery getID;
        getID.prepare("SELECT id FROM dirs WHERE path = ?");
        getID.addBindValue(filterDir.absolutePath());
        getID.exec();
        while (getID.next()) {
            addDirectoryChildren(getID.value(0).toInt(), dirIDs);
        }
    }

    // Create a list of all sample IDs that contain a tag (or any children of those tags)
    QStringList sampleIDs;
    for (auto filterTag : filterTags) {
        addTagChildren(filterTag.id, sampleIDs);
    }

    // No filtering (select NULL so all columns match)
    if (filterDirs.empty() && filterTags.empty()) {
        query.exec("SELECT NULL, samples.id, dir_id, name, filename, path FROM samples JOIN dirs ON samples.dir_id = dirs.id");
    }
    // Only directory filtering (select NULL so all columns match)
    else if (!filterDirs.empty() && filterTags.empty()) {
        query.exec("SELECT NULL, samples.id, dir_id, name, filename, path FROM samples JOIN dirs ON samples.dir_id = dirs.id WHERE dir_id IN (" + dirIDs.join(",") + ")");
    }
    // Only tag filtering
    else if (filterDirs.empty() && !filterTags.empty()) {
        QString queryStr;
        queryStr += "SELECT COUNT(samples.id), samples.id, dir_id, name, filename, path FROM ";
        queryStr += "sample_tags JOIN samples ON sample_tags.sample_id = samples.id ";
        queryStr += "JOIN dirs ON samples.dir_id = dirs.id ";
        queryStr += "WHERE sample_tags.tag_id IN (" + sampleIDs.join(",") + ") ";
        queryStr += "GROUP BY samples.id ";
        queryStr += "HAVING COUNT(samples.id) = " + QString::number(filterTags.size());
        query.exec(queryStr);
    }
    // Both directory and tag filtering
    else {

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

