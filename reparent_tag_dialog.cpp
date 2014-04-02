#include "reparent_tag_dialog.h"
#include "ui_reparent_tag_dialog.h"

ReparentTagDialog::ReparentTagDialog(TagsModel* model, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReparentTagDialog)
{
    ui->setupUi(this);
    ui->treeView->setModel(model);
}

ReparentTagDialog::~ReparentTagDialog()
{
    delete ui;
}

Tag ReparentTagDialog::getSelectedTag()
{
    auto selected = ui->treeView->selectionModel()->selectedIndexes();
    return selected.empty() ? Tag() : static_cast<TagWrapper*>(selected.last().internalPointer())->tag;
}
