#include "utils.h"

QStringList Utils::getSupportedExtensions()
{
    return QStringList() << "wav";
}

QStringList Utils::getSupportedNameFilters()
{
    QStringList nameFilters;
    for (auto e : Utils::getSupportedExtensions())
    {
        nameFilters << "*." << e;
    }
    return nameFilters;
}

bool Utils::isSupportedExtension(QString const& extension)
{
    for (auto s : Utils::getSupportedExtensions()) {
        if (!s.compare(extension)) {
            return true;
        }
    }
    return false;
}
