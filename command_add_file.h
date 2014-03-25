#ifndef COMMAND_ADDFILE_H
#define COMMAND_ADDFILE_H

#include <QUndoCommand>
#include "directories_model.h"

class AddFile : public QUndoCommand
{
public:
    AddFile(QString const& path, DirectoriesModel& model, QUndoCommand* parent = nullptr);
    virtual void redo();
    virtual void undo();

private:
    QString path;
    DirectoriesModel& model;
};

#endif // COMMAND_ADDFILE_H
