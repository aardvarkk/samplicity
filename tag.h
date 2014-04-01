#ifndef TAG_H
#define TAG_H

#include <QString>

struct Tag
{
public:
    Tag();
    Tag(int id, int parent_id, QString const& name);
    int id;
    int parent_id;
    QString name;

    bool valid();
};

#endif // TAG_H
