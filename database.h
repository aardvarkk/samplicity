#ifndef DATABASE_H
#define DATABASE_H

#include <QString>
#include <QtSql>

class Database
{
public:
    Database();
    Database(QString const& filename);

    void addFile(QFile const& file);
    void addDirectory(QDir const& dir);

protected:
    bool createTables();

    QSqlDatabase db;
};

#endif // DATABASE_H
