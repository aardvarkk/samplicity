#include <QDir>

#include "sample.h"

Sample::Sample() : id(0), dir_id(0), name(""), filename(""), path(""), rating(QVariant(QVariant::Int))
{
}

Sample::Sample(int id, int dir_id, QString const& name, QString const& filename, QString const& path, QVariant const& rating) :
    id(id),
    dir_id(dir_id),
    name(name),
    filename(filename),
    path(path),
    rating(rating)
{
}

QString Sample::fullPath() const
{
    return QDir(path).filePath(filename);
}

