#ifndef REPARENT_TAG_DIALOG_H
#define REPARENT_TAG_DIALOG_H

#include <QDialog>

#include "tags_model.h"
#include "tag.h"

namespace Ui {
class ReparentTagDialog;
}

class ReparentTagDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReparentTagDialog(TagsModel* model, QWidget *parent = 0);
    ~ReparentTagDialog();
    Tag getSelectedTag();

private:
    Ui::ReparentTagDialog *ui;
};

#endif // REPARENT_TAG_DIALOG_H
