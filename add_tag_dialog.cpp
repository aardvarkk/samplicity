#include "add_tag_dialog.h"
#include "ui_add_tag_dialog.h"

AddTagDialog::AddTagDialog(Database& db, QWidget *parent) :
    QDialog(parent),
    db(db),
    ui(new Ui::AddTagDialog)
{
    ui->setupUi(this);

    ui->lineEdit->setFocus();
}

AddTagDialog::~AddTagDialog()
{
    delete ui;
}

QString AddTagDialog::tagName()
{
    return ui->lineEdit->text();
}
