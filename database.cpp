#include "database.h"
#include "filesystem.h"

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

    db.transaction();

    // Create all parents, from base to tip
    auto fileInfo = QFileInfo(file);
    auto dir = fileInfo.absoluteDir();
    QStringList parents;
    do {
        parents << dir.absolutePath();
    } while (dir.cdUp());
    auto success = true;
    QSqlQuery query;
    auto it = parents.constEnd();
    while (it != parents.constBegin()) {
        --it;
        query.prepare("INSERT OR IGNORE INTO dirs (path) VALUES (?)");
        query.addBindValue(*it);
        success &= query.exec();
    }

    // Get its parent id
    query.prepare("SELECT id FROM dirs WHERE path = ? LIMIT 1");
    query.addBindValue(fileInfo.absolutePath());
    success &= query.exec();
    if (!query.next()) {
        return false;
    }
    int parent_id = query.value(0).toInt();

    // Add a new sample record
    query.prepare("INSERT INTO samples (dir_id, name, filename) VALUES (?,?,?)");
    query.addBindValue(parent_id);
    query.addBindValue(fileInfo.fileName());
    query.addBindValue(fileInfo.fileName());
    success &= query.exec();

    success ? db.commit() : db.rollback();
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
    auto success = fs.findFiles(dir) ? db.commit() : db.rollback();
    return success;
}

bool Database::removeDirectory(const QDir &dir)
{
    qDebug() << __FUNCSIG__;
    qDebug() << dir;
    Filesystem fs;
    QObject::connect(&fs, SIGNAL(foundFile(QFile)), this, SLOT(removeFile(QFile)));
    return fs.findFiles(dir);
}
