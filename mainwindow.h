#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUndoStack>
#include "database.h"
#include "directories_model.h"
#include "samples_model.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionExit_triggered();

    void on_actionAddDirectory_triggered();

    void on_actionAddFile_triggered();

    void on_dirsTreeViewSelectionChanged(QItemSelection const& selected, QItemSelection const& deselected);

private:
    void filterSamples();

    Ui::MainWindow *ui;
    Database* db;
    QUndoStack* undoStack;
    DirectoriesModel* directoriesModel;
    SamplesModel* samplesModel;
};

#endif // MAINWINDOW_H
