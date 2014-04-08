#include <QCryptographicHash>
#include <QFileInfo>

#include "hasher.h"

ErrorCode Hasher::getFileHash(QFile& file, QByteArray& hash)
{
    QCryptographicHash hasher(QCryptographicHash::Md5);
    if (!hasher.addData(&file)) {
        return ErrorCode(ErrorCode::ReadError, QFileInfo(file).absolutePath());
    }
    hash = hasher.result();
    return ErrorCode(ErrorCode::Success);
}
