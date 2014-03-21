#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <QDir>
#include <QFile>
#include <QStringList>

class Filesystem : public QObject
{
    Q_OBJECT

public:
    bool findFiles(QDir const& dir, QStringList* extensions = nullptr);

signals:
    void foundFile(QFile const& file);
};

#endif // FILESYSTEM_H
