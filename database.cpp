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
    success &= query.exec("CREATE TABLE IF NOT EXISTS samples (id INTEGER PRIMARY KEY, name TEXT, file TEXT, dir TEXT)");
    return success;
}

void Database::addFile(QFile const& file)
{
    qDebug() << __FUNCSIG__;
    qDebug() << file.fileName();
}

void Database::removeFile(QFile const& file)
{
    qDebug() << __FUNCSIG__;
    qDebug() << file.fileName();
}

class AddDirectoryHelper : public DirectoryIteratorHelper
{
public:
    AddDirectoryHelper(Database& db) : db(db) {}

protected:
    virtual void foundFile(QString const& path)
    {
        db.addFile(path);
    }
private:
    Database& db;
};

class RemoveDirectoryHelper : public DirectoryIteratorHelper
{
public:
    RemoveDirectoryHelper(Database& db) : db(db) {}

protected:
    virtual void foundFile(QString const& path)
    {
        db.removeFile(path);
    }
private:
    Database& db;
};

void Database::addDirectory(QDir const& dir)
{
    qDebug() << __FUNCSIG__;
    qDebug() << dir;
    Filesystem::findFiles(dir, AddDirectoryHelper(*this));
}

void Database::removeDirectory(const QDir &dir)
{
    qDebug() << __FUNCSIG__;
    qDebug() << dir;
    Filesystem::findFiles(dir, RemoveDirectoryHelper(*this));
}
