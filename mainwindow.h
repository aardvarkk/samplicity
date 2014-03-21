#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "database.h"
#include "directories_model.h"

#include <QMainWindow>
#include <QUndoStack>

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

private:
    Ui::MainWindow *ui;
    Database* db;
    QUndoStack* undoStack;
    DirectoriesModel* directoriesModel;
};

#endif // MAINWINDOW_H
