#include "line_edit_dialog.h"
#include "ui_line_edit_dialog.h"

LineEditDialog::LineEditDialog(Database& db, QWidget* parent) :
    QDialog(parent),
    db(db),
    ui(new Ui::LineEditDialog)
{
    ui->setupUi(this);

    ui->lineEdit->setFocus();
}

LineEditDialog::~LineEditDialog()
{
    delete ui;
}

void LineEditDialog::setText(QString const& text)
{
    ui->lineEdit->setText(text);
}

QString LineEditDialog::getText()
{
    return ui->lineEdit->text();
}
