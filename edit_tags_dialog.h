#ifndef EDIT_TAGS_DIALOG_H
#define EDIT_TAGS_DIALOG_H

#include <QDialog>
#include <QPushButton>

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

private slots:
    void addTag(bool checked = false);
    void removeTag(bool checked = false);
    void renameTag(bool checked = false);

private:
    Database& db;
    Ui::EditTagsDialog *ui;
    TagsModel* tagsModel;

    QPushButton* buttonAddTag;
    QPushButton* buttonRemoveTag;
    QPushButton* buttonRenameTag;
    QPushButton* buttonReparentTag;
};

#endif // EDIT_TAGS_DIALOG_H
