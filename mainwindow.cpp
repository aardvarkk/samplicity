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
    directoriesModel(new DirectoriesModel(*db)),
    samplesModel(new SamplesModel(*db)),
    audioPlayer(new AudioPlayer)
{
    ui->setupUi(this);

    undoStack = new QUndoStack(this);
    // auto undoView = new QUndoView(undoStack);
    // undoView->show();

    ui->menuEdit->addAction(undoStack->createUndoAction(this));
    ui->menuEdit->addAction(undoStack->createRedoAction(this));

    ui->dirsTreeView->setModel(directoriesModel);

    // When you change the directory selection, refilter the samples
    QObject::connect(
        ui->dirsTreeView->selectionModel(),
        SIGNAL(selectionChanged(QItemSelection const&, QItemSelection const&)),
        this,
        SLOT(on_dirsTreeViewSelectionChanged(QItemSelection const&, QItemSelection const&))
        );

    ui->samplesTreeView->setModel(samplesModel);

    // Make it such that when any directories/files are added or changed,
    // the samples view is refreshed
    QObject::connect(
        directoriesModel,
        SIGNAL(modelReset()),
        samplesModel,
        SLOT(refresh()));

    QObject::connect(
        ui->samplesTreeView->selectionModel(),
        SIGNAL(currentChanged(QModelIndex,QModelIndex)),
        this,
        SLOT(on_samplesTreeViewSelectionChanged(QModelIndex, QModelIndex))
        );
}

void MainWindow::on_samplesTreeViewSelectionChanged(QModelIndex const& selected, QModelIndex const& deselected)
{
    auto sample = samplesModel->getSample(selected);
    audioPlayer->stop();
    audioPlayer->play(sample->path);
}

MainWindow::~MainWindow()
{
    delete audioPlayer;
    delete samplesModel;
    delete directoriesModel;
    delete undoStack;
    delete db;
    delete ui;
}

void MainWindow::filterSamples()
{
    QList<QDir> selectedDirs;

    // Get all selected directories
    auto selection = ui->dirsTreeView->selectionModel()->selection();
    for (auto selectionRange : selection) {
        for (auto modelIdx : selectionRange.indexes()) {
            selectedDirs << QDir(directoriesModel->data(modelIdx, Qt::EditRole).toString());
        }
    }

    qDebug() << __FUNCSIG__;
    qDebug() << selectedDirs;

    samplesModel->setFilterDirs(selectedDirs);
}

void MainWindow::on_dirsTreeViewSelectionChanged(QItemSelection const& selected, QItemSelection const& deselected)
{
    filterSamples();
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
        undoStack->push(new AddDirectory(d, *directoriesModel));
    }
}

void MainWindow::on_actionAddFile_triggered()
{
    QStringList files = QFileDialog::getOpenFileNames(this);
    for (auto f : files) {
        undoStack->push(new AddFile(f, *directoriesModel));
    }
}
