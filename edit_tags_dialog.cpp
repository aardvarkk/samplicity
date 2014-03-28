#include "edit_tags_dialog.h"
#include "ui_edit_tags_dialog.h"
#include "tags_model.h"

EditTagsDialog::EditTagsDialog(Database& db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditTagsDialog),
    tagsModel(new TagsModel(db))
{
    qDebug() << __FUNCSIG__;

    ui->setupUi(this);

    ui->treeView->setModel(tagsModel);
}

EditTagsDialog::~EditTagsDialog()
{
    delete tagsModel;
    delete ui;
}
