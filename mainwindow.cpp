#include <QFileDialog>
#include <QLayout>
#include <QUndoView>

#include "command_add_directory.h"
#include "command_add_file.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "edit_tags_dialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    db(new Database("samplicity.db")),
    directoriesModel(new DirectoriesModel(*db)),
    samplesModel(new SamplesModel(*db)),
    tagsModel(new TagsModel(*db)),
    audioPlayer(new AudioPlayer),
    settings(Settings::getSettings())
{
    ui->setupUi(this);

    // Default settings
    ui->actionLoop_Playback->setChecked(settings->value("loopPlayback", false).toBool());

    undoStack = new QUndoStack(this);
    // auto undoView = new QUndoView(undoStack);
    // undoView->show();

    ui->menuEdit->insertAction(ui->actionTags, undoStack->createUndoAction(this));
    ui->menuEdit->insertAction(ui->actionTags, undoStack->createRedoAction(this));

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

    ui->tagsTreeView->setModel(tagsModel);
}

void MainWindow::on_samplesTreeViewSelectionChanged(QModelIndex const& selected, QModelIndex const& deselected)
{
    auto sample = samplesModel->getSample(selected);
    audioPlayer->stop();
    audioPlayer->play(sample->fullPath());
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

void MainWindow::on_actionLoop_Playback_toggled(bool checked)
{
    settings->setValue("loopPlayback", checked);
    audioPlayer->setLoop(checked);
}

void MainWindow::on_actionTags_triggered()
{
    qDebug() << __FUNCSIG__;

    EditTagsDialog dialog(*db, this);
    dialog.exec();
}
