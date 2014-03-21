#ifndef COMMAND_ADDFILE_H
#define COMMAND_ADDFILE_H

#include "database.h"
#include <QUndoCommand>

class AddFile : public QUndoCommand
{
public:
    AddFile(QString const& path, Database& db, QUndoCommand* parent = nullptr);
    virtual void redo();
    virtual void undo();

private:
    QString path;
    Database& db;
};

#endif // COMMAND_ADDFILE_H
