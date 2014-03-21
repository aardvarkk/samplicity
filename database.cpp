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

    auto success = true;
    QSqlQuery query;

    // Create all parents, from base to tip
    QDir dir = QFileInfo(file).absoluteDir();
    QStringList parents;
    do {
        parents << dir.absolutePath();
    } while (dir.cdUp());
    auto it = parents.constEnd();
    while (it != parents.constBegin()) {
        --it;
        query.prepare("INSERT OR IGNORE INTO dirs (path) VALUES (?)");
        query.addBindValue(*it);
        success &= query.exec();
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
    Filesystem fs;
    QObject::connect(&fs, SIGNAL(foundFile(QFile)), this, SLOT(addFile(QFile)));
    return fs.findFiles(dir);
}

bool Database::removeDirectory(const QDir &dir)
{
    qDebug() << __FUNCSIG__;
    qDebug() << dir;
    Filesystem fs;
    QObject::connect(&fs, SIGNAL(foundFile(QFile)), this, SLOT(removeFile(QFile)));
    return fs.findFiles(dir);
}
