#include <QMessageBox>

#include "add_tag_dialog.h"
#include "edit_tags_dialog.h"
#include "ui_edit_tags_dialog.h"
#include "tags_model.h"

EditTagsDialog::EditTagsDialog(Database& db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditTagsDialog),
    db(db),
    tagsModel(new TagsModel(db))
{
    ui->setupUi(this);

    ui->treeView->setModel(tagsModel);

    buttonAddTag = ui->buttonBox->addButton(tr("&Add"), QDialogButtonBox::ActionRole);
    buttonRemoveTag = ui->buttonBox->addButton(tr("Remove"), QDialogButtonBox::ActionRole);
    buttonRenameTag = ui->buttonBox->addButton(tr("Rename"), QDialogButtonBox::ActionRole);
    buttonReparentTag = ui->buttonBox->addButton(tr("Reparent"), QDialogButtonBox::ActionRole);

    QObject::connect(
                buttonAddTag,
                SIGNAL(clicked(bool)),
                this,
                SLOT(addTag(bool))
                );
}

EditTagsDialog::~EditTagsDialog()
{
    delete tagsModel;
    delete ui;
}

void EditTagsDialog::addTag(bool checked)
{
    int parent_id = 0;
    auto selected = ui->treeView->selectionModel()->selectedIndexes();
    if (!selected.empty()) {
        parent_id = static_cast<TagWrapper*>(selected.last().internalPointer())->tag.id;
    }

    AddTagDialog dialog(db, this);
    while (dialog.exec() == QDialog::Accepted) {
        if (tagsModel->addTag(dialog.tagName(), parent_id)) {
            return;
        }
        QMessageBox::warning(this, tr("Warning"), tr("Error adding tag"));
    }
}

