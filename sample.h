#ifndef SAMPLE_H
#define SAMPLE_H

#include <QString>

struct Sample
{
public:
    Sample(int id, int dir_id, QString const& name, QString const& filename, QString const& path);
    int id;
    int dir_id;
    QString name;
    QString filename;
    QString path;

    QString fullPath() const;
};

#endif // SAMPLE_H
