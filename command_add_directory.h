#ifndef COMMAND_ADDDIRECTORY_H
#define COMMAND_ADDDIRECTORY_H

#include "database.h"
#include <QUndoCommand>

class AddDirectory : public QUndoCommand
{
public:
    AddDirectory(QString const& path, Database& db, QUndoCommand* parent = nullptr);
    virtual void redo();
    virtual void undo();

private:
    QString path;
    Database& db;
};

#endif // COMMAND_ADDDIRECTORY_H
