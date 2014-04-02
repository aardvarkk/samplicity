#include <QMessageBox>

#include "line_edit_dialog.h"
#include "edit_tags_dialog.h"
#include "reparent_tag_dialog.h"
#include "tags_model.h"
#include "ui_edit_tags_dialog.h"

EditTagsDialog::EditTagsDialog(Database& db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditTagsDialog),
    db(db),
    tagsModel(new TagsModel(db))
{
    ui->setupUi(this);

    ui->treeView->setModel(tagsModel);

    buttonAddTag = ui->buttonBox->addButton(tr("&Add"), QDialogButtonBox::ActionRole);
    buttonRemoveTag = ui->buttonBox->addButton(tr("Re&move"), QDialogButtonBox::ActionRole);
    buttonRenameTag = ui->buttonBox->addButton(tr("Re&name"), QDialogButtonBox::ActionRole);
    buttonReparentTag = ui->buttonBox->addButton(tr("Re&parent"), QDialogButtonBox::ActionRole);

    QObject::connect(
                buttonAddTag,
                SIGNAL(clicked(bool)),
                this,
                SLOT(addTag(bool))
                );

    QObject::connect(
                buttonRemoveTag,
                SIGNAL(clicked(bool)),
                this,
                SLOT(removeTag(bool))
                );

    QObject::connect(
                buttonRenameTag,
                SIGNAL(clicked(bool)),
                this,
                SLOT(renameTag(bool))
                );

    QObject::connect(
                buttonReparentTag,
                SIGNAL(clicked(bool)),
                this,
                SLOT(reparentTag(bool))
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

    LineEditDialog dialog(db, this);
    dialog.setWindowTitle(tr("Add Tag"));
    while (dialog.exec() == QDialog::Accepted) {
        if (tagsModel->addTag(dialog.getText(), parent_id)) {
            return;
        }
        QMessageBox::warning(this, tr("Warning"), tr("Error adding tag"));
    }
}

void EditTagsDialog::removeTag(bool checked)
{
    qDebug() << __FUNCSIG__;

    // Get the set of tags to remove
    auto selected = ui->treeView->selectionModel()->selectedIndexes();
    QList<Tag> toRemove;
    for (auto s : selected) {
        auto w = static_cast<TagWrapper*>(s.internalPointer());
        toRemove << w->tag;
    }

    for (auto t : toRemove) {
//        qDebug() << "Remove tag " << t.id;
        if (!tagsModel->removeTag(t)) {
            return;
        }
    }
}

void EditTagsDialog::renameTag(bool checked)
{
    auto selected = ui->treeView->selectionModel()->selectedIndexes();
    if (selected.empty()) {
        return;
    }

    auto w = static_cast<TagWrapper*>(selected.last().internalPointer());

    LineEditDialog dialog(db, this);
    dialog.setWindowTitle(tr("Rename Tag"));
    dialog.setText(w->tag.name);
    while (dialog.exec() == QDialog::Accepted) {
        if (tagsModel->renameTag(w->tag, dialog.getText())) {
            return;
        }
        QMessageBox::warning(this, tr("Warning"), tr("Error adding tag"));
    }
}

void EditTagsDialog::reparentTag(bool checked)
{
    qDebug() << __FUNCSIG__;

    auto selected = ui->treeView->selectionModel()->selectedIndexes();
    if (selected.empty()) {
        return;
    }

    auto w = static_cast<TagWrapper*>(selected.last().internalPointer());

    ReparentTagDialog dialog(tagsModel);
    while (dialog.exec() == QDialog::Accepted) {
        auto parent = dialog.getSelectedTag();
//        qDebug() << "Reparent" << w->tag.id << "from" << w->tag.parent_id << "to" << parent.id;
        if (tagsModel->reparentTag(w->tag, parent.id)) {
            return;
        }
        QMessageBox::warning(this, tr("Warning"), tr("Error reparenting tag"));
    }
}

