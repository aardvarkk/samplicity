#ifndef COMMAND_ADDDIRECTORY_H
#define COMMAND_ADDDIRECTORY_H

#include <QUndoCommand>
#include "directories_model.h"

class AddDirectory : public QUndoCommand
{
public:
    AddDirectory(QString const& path, DirectoriesModel& model, QUndoCommand* parent = nullptr);
    virtual void redo();
    virtual void undo();

private:
    QString path;
    DirectoriesModel& model;
};

#endif // COMMAND_ADDDIRECTORY_H
