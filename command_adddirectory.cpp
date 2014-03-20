#include "command_adddirectory.h"

AddDirectory::AddDirectory(QString const& path, Database& db, QUndoCommand* parent) :
    path(path), db(db), QUndoCommand(parent)
{
    // setText(QObject::tr("Add Directory"));
    setText(QObject::tr("Add Directory") + " " + path);
}

void AddDirectory::redo()
{
    db.addDirectory(path);
}

void AddDirectory::undo()
{
    db.removeDirectory(path);
}
