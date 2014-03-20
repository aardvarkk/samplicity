#include "command_addfile.h"

AddFile::AddFile(QString const& path, Database& db, QUndoCommand* parent) :
    path(path), db(db), QUndoCommand(parent)
{
    // setText(QObject::tr("Add File"));
    setText(QObject::tr("Add File") + " " + path);
}

void AddFile::redo()
{
    db.addFile(path);
}

void AddFile::undo()
{
    db.removeFile(path);
}
