#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "command_adddirectory.h"
#include "command_addfile.h"
#include <QFileDialog>
#include <QLayout>
#include <QUndoView>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    db(new Database("samplicity.db"))
{
    ui->setupUi(this);

    undoStack = new QUndoStack(this);
    // auto undoView = new QUndoView(undoStack);
    // undoView->show();

    ui->menuEdit->addAction(undoStack->createUndoAction(this));
    ui->menuEdit->addAction(undoStack->createRedoAction(this));
}

MainWindow::~MainWindow()
{
    delete undoStack;
    delete db;
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_actionAddDirectory_triggered()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::DirectoryOnly);
    dialog.exec();
    undoStack->push(new AddDirectory(dialog.selectedFiles().front(), *db));
}

void MainWindow::on_actionAddFile_triggered()
{
    QString file = QFileDialog::getOpenFileName(this);
    undoStack->push(new AddFile(file, *db));
}
