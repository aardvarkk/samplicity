#ifndef EDIT_TAGS_DIALOG_H
#define EDIT_TAGS_DIALOG_H

#include <QDialog>

#include "database.h"
#include "tags_model.h"

namespace Ui {
class EditTagsDialog;
}

class EditTagsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditTagsDialog(Database& db, QWidget *parent = 0);
    ~EditTagsDialog();

private:
    Ui::EditTagsDialog *ui;
    TagsModel* tagsModel;
};

#endif // EDIT_TAGS_DIALOG_H
