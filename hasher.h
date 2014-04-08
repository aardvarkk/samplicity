#ifndef HASHER_H
#define HASHER_H

#include <QFile>

#include "error_code.h"

class Hasher
{
public:
    static ErrorCode getFileHash(QFile& file, QByteArray& hash);
};

#endif // HASHER_H
