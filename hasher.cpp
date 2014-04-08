#include <QCryptographicHash>
#include <QFileInfo>

#include "hasher.h"

ErrorCode Hasher::getFileHash(QFile& file, QByteArray& hash)
{
    // Open the file if necessary
    if (!file.isOpen()) {
        file.open(QFile::ReadOnly);
    }

    QCryptographicHash hasher(QCryptographicHash::Md5);
    if (!hasher.addData(&file)) {
        return ErrorCode(ErrorCode::ReadError, QFileInfo(file).absolutePath());
    }
    hash = hasher.result();
    return ErrorCode(ErrorCode::Success);
}
