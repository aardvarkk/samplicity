#ifndef SAMPLE_H
#define SAMPLE_H

#include <QString>
#include <QVariant>

struct Sample
{
public:
    Sample();
    Sample(int id, int dir_id, QString const& name, QString const& filename, QString const& path, QVariant const& rating);
    int id;
    int dir_id;
    QString name;
    QString filename;
    QString path;
    QVariant rating;

    QString fullPath() const;
};

#endif // SAMPLE_H
