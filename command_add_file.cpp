#include "command_add_file.h"

AddFile::AddFile(QString const& path, DirectoriesModel& model, QUndoCommand* parent) :
    path(path), model(model), QUndoCommand(parent)
{
    // setText(QObject::tr("Add File"));
    setText(QObject::tr("Add File") + " " + path);
}

void AddFile::redo()
{
    model.addFile(path);
}

void AddFile::undo()
{
    model.removeFile(path);
}
