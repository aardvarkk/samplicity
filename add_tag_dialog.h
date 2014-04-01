#ifndef ADD_TAG_DIALOG_H
#define ADD_TAG_DIALOG_H

#include <QDialog>

#include "database.h"

namespace Ui {
class AddTagDialog;
}

class AddTagDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddTagDialog(Database& db, QWidget *parent = 0);
    ~AddTagDialog();
    QString tagName();

private:
    Database& db;
    Ui::AddTagDialog *ui;
};

#endif // ADD_TAG_DIALOG_H
