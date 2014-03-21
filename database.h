#ifndef DATABASE_H
#define DATABASE_H

#include <QString>
#include <QtSql>

class Database : public QObject
{
    Q_OBJECT

public:
    Database();
    Database(QString const& filename);

    bool addDirectory(QDir const& dir);
    bool removeDirectory(QDir const& dir);

public slots:
    bool addFile(QFile const& file);
    bool removeFile(QFile const& file);

protected:
    bool createTables();

    QSqlDatabase db;
};

#endif // DATABASE_H
