#include <QDir>

#include "sample.h"

Sample::Sample(int id, int dir_id, QString const& name, QString const& filename, QString const& path) :
    id(id),
    dir_id(dir_id),
    name(name),
    filename(filename),
    path(path)
{
}

QString Sample::fullPath() const
{
    return QDir(path).filePath(filename);
}

