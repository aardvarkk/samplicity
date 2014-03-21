#include "filesystem.h"

bool Filesystem::findFiles(QDir const& dir, QStringList* extensions)
{
    auto myDir(dir);

    myDir.setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
    auto subDirs = myDir.entryInfoList();
    for (auto subDir : subDirs) {
        findFiles(subDir.filePath(), extensions);
    }

    myDir.setFilter(QDir::Files);
    if (extensions) {
        myDir.setNameFilters(*extensions);
    }
    auto files = myDir.entryInfoList();
    for (auto file : files) {
        emit foundFile(QFile(file.absoluteFilePath()));
    }

    return true;
}
