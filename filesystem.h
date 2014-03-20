#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <QDir>
#include <QString>

class DirectoryIteratorHelper
{
public:
    virtual void foundFile(QString const& path) = 0;
};

class Filesystem
{
public:
    Filesystem();

    static void findFiles(QDir const& dir, DirectoryIteratorHelper& helper);
};

#endif // FILESYSTEM_H
