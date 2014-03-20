#include "filesystem.h"

Filesystem::Filesystem()
{
}

void Filesystem::findFiles(QDir const& dir, DirectoryIteratorHelper& helper)
{
    auto myDir(dir);

    myDir.setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
    auto subDirs = myDir.entryInfoList();
    for (auto subDir : subDirs) {
        Filesystem::findFiles(subDir.filePath(), helper);
    }

    myDir.setFilter(QDir::Files);
    auto files = myDir.entryInfoList();
    for (auto file : files) {
        helper.foundFile(file.filePath());
    }
}
