#include "command_add_directory.h"

AddDirectory::AddDirectory(QString const& path, DirectoriesModel& model, QUndoCommand* parent) :
    path(path), model(model), QUndoCommand(parent)
{
    // setText(QObject::tr("Add Directory"));
    setText(QObject::tr("Add Directory") + " " + path);
}

void AddDirectory::redo()
{
    model.addDirectory(path);
}

void AddDirectory::undo()
{
    model.removeDirectory(path);
}
