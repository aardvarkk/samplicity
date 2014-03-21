#include "filesystem.h"

void Filesystem::findFiles(QDir const& dir)
{
    auto myDir(dir);

    myDir.setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
    auto subDirs = myDir.entryInfoList();
    for (auto subDir : subDirs) {
        findFiles(subDir.filePath());
    }

    myDir.setFilter(QDir::Files);
    auto files = myDir.entryInfoList();
    for (auto file : files) {
        emit foundFile(QFile(file.absoluteFilePath()));
    }
}
