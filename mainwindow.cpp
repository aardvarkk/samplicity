#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "command_add_directory.h"
#include "command_add_file.h"
#include <QFileDialog>
#include <QLayout>
#include <QUndoView>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    db(new Database("samplicity.db")),
    directoriesModel(new DirectoriesModel(*db))
{
    ui->setupUi(this);

    undoStack = new QUndoStack(this);
    // auto undoView = new QUndoView(undoStack);
    // undoView->show();

    ui->menuEdit->addAction(undoStack->createUndoAction(this));
    ui->menuEdit->addAction(undoStack->createRedoAction(this));

    ui->dirsTreeView->setModel(directoriesModel);
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
    for (auto d : dialog.selectedFiles()) {
        undoStack->push(new AddDirectory(d, *db));
    }
}

void MainWindow::on_actionAddFile_triggered()
{
    QStringList files = QFileDialog::getOpenFileNames(this);
    for (auto f : files) {
        undoStack->push(new AddFile(f, *db));
    }
}
