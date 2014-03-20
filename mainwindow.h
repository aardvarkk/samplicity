#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "database.h"

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
    QUndoStack* undoStack;
    Ui::MainWindow *ui;
    Database* db;
};

#endif // MAINWINDOW_H
