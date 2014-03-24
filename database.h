#ifndef DATABASE_H
#define DATABASE_H

#include <QString>
#include <QtSql>
#include "sample.h"

class Database : public QObject
{
    Q_OBJECT

public:
    Database();
    Database(QString const& filename);

    bool addDirectory(QDir const& dir);
    bool removeDirectory(QDir const& dir);
    QList<QDir> getChildren(QDir const* dir) const;
    QList<Sample> getSamples(QList<QDir> const* filterDirs = nullptr) const;

public slots:
    bool addFile(QFile const& file);
    bool removeFile(QFile const& file);

protected:
    bool createTables();

    QSqlDatabase db;
};

#endif // DATABASE_H
