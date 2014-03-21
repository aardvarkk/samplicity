#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <QDir>
#include <QString>

class Filesystem : public QObject
{
    Q_OBJECT

public:
    void findFiles(QDir const& dir);
signals:
    void foundFile(QFile const& file);
};

#endif // FILESYSTEM_H
