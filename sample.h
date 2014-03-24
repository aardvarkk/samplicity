#ifndef SAMPLE_H
#define SAMPLE_H

#include <QString>

struct Sample
{
public:
    Sample(QString const& name, QString const& path);
    QString name;
    QString path;
};

#endif // SAMPLE_H
