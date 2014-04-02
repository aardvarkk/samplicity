#ifndef LINE_EDIT_DIALOG_H
#define LINE_EDIT_DIALOG_H

#include <QDialog>

#include "database.h"

namespace Ui {
class LineEditDialog;
}

class LineEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LineEditDialog(Database& db, QWidget *parent = 0);
    ~LineEditDialog();

    void setText(QString const& text);
    QString getText();

private:
    Database& db;
    Ui::LineEditDialog* ui;
};

#endif // LINE_EDIT_DIALOG_H
