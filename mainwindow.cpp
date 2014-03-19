#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    db(new Database("samplicity.db"))
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
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
    QDir dir = dialog.directory();
    db->addDirectory(dir);
}

void MainWindow::on_actionAddFile_triggered()
{
    QString file = QFileDialog::getOpenFileName(this);
    db->addFile(file);
}
