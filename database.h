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

    void addDirectory(QDir const& dir);
    void removeDirectory(QDir const& dir);

public slots:
    void addFile(QFile const& file);
    void removeFile(QFile const& file);

protected:
    bool createTables();

    QSqlDatabase db;
};

#endif // DATABASE_H
