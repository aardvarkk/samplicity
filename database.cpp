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
}

bool Database::createTables()
{
    auto success = true;
    QSqlQuery query;
    success &= query.exec("CREATE TABLE IF NOT EXISTS dirs (id INTEGER PRIMARY KEY, parent_id INTEGER, path TEXT UNIQUE)");
    success &= query.exec("CREATE TABLE IF NOT EXISTS samples (id INTEGER PRIMARY KEY, dir_id INTEGER, name TEXT, filename TEXT)");
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
    auto success = true;
    QSqlQuery query;
    auto it = parents.constEnd();
    int id = -1;
    while (it != parents.constBegin()) {
        --it;
        query.prepare("INSERT OR IGNORE INTO dirs (parent_id, path) VALUES (?,?)");
        query.addBindValue(id >= 0 ? id : QVariant(QVariant::Int));
        query.addBindValue(*it);
        success &= query.exec();
        id = query.lastInsertId().toInt();
    }

    // Add a new sample record
    query.prepare("INSERT INTO samples (dir_id, name, filename) VALUES (?,?,?)");
    query.addBindValue(id);
    query.addBindValue(fileInfo.fileName());
    query.addBindValue(fileInfo.fileName());
    success &= query.exec();

    if (QObject::sender() == nullptr) {
        success ? db.commit() : db.rollback();
    }

    return success;
}

bool Database::removeFile(QFile const& file)
{
    qDebug() << __FUNCSIG__;
    qDebug() << file.fileName();
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
