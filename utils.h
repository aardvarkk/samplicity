#ifndef UTILS_H
#define UTILS_H

#include <QStringList>

class Utils
{
public:
    static bool isSupportedExtension(QString const& extension);
    static QStringList getSupportedExtensions();
    static QStringList getSupportedNameFilters();
};

#endif // UTILS_H
