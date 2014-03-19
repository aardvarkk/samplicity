#include "database.h"

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
    qDebug() << file.fileName();
}

void Database::addDirectory(QDir const& dir)
{
    qDebug() << dir;

    auto myDir(dir);

    myDir.setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
    auto subDirs = myDir.entryInfoList();
    for (auto subDir : subDirs) {
        addDirectory(subDir.filePath());
    }

    myDir.setFilter(QDir::Files);
    auto files = myDir.entryInfoList();
    for (auto file : files) {
        addFile(QFile(file.filePath()));
    }
}

