#include <QFileDialog>
#include <QLayout>
#include <QDesktopServices>
#include <QUndoView>

#include "command_add_directory.h"
#include "command_add_file.h"
#include "main_window.h"
#include "ui_main_window.h"
#include "edit_tags_dialog.h"

#include "modeltest.h"

struct TagMode
{
    enum
    {
        Filter,
        Apply
    };
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    db(new Database("samplicity.db")),
    directoriesModel(new DirectoriesModel(*db)),
    samplesModel(new SamplesModel(*db)),
    tagsModel(new TagsModel(*db)),
    audioPlayer(new AudioPlayer),
    settings(Settings::getSettings()),
    writeSampleTags(true)
{
    ui->setupUi(this);

    // Default settings
    ui->actionLoop_Playback->setChecked(settings->value("loopPlayback", false).toBool());

    auto tagMode = settings->value("tagMode", TagMode::Filter).toInt();
    ui->filterRadioButton->setChecked(tagMode == TagMode::Filter);
    ui->applyRadioButton->setChecked(tagMode == TagMode::Apply);
    ui->tagMode->setId(ui->filterRadioButton, TagMode::Filter);
    ui->tagMode->setId(ui->applyRadioButton, TagMode::Apply);

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
        SLOT(directorySelectionChanged(QItemSelection const&, QItemSelection const&))
        );

    ui->samplesTreeView->setModel(samplesModel);
    // auto modelTest = new ModelTest(samplesModel, this);

    // Make it such that when any directories/files are added or changed,
    // the samples view is refreshed
    QObject::connect(
        directoriesModel,
        SIGNAL(modelReset()),
        samplesModel,
        SLOT(reset()));

    QObject::connect(
        ui->samplesTreeView->selectionModel(),
        SIGNAL(currentChanged(QModelIndex,QModelIndex)),
        this,
        SLOT(sampleSelectionChanged(QModelIndex, QModelIndex))
        );

    ui->tagsTreeView->setModel(tagsModel);
//    tagsProxyModel.setSourceModel(tagsModel);
//    tagsProxyModel.setSortCaseSensitivity(Qt::CaseInsensitive);
//    ui->tagsTreeView->setModel(&tagsProxyModel);
//    ui->tagsTreeView->setSortingEnabled(true);
//    tagsProxyModel.sort(0, Qt::AscendingOrder);
    // auto modelTest = new ModelTest(tagsModel, this);

    QObject::connect(
                ui->filterRadioButton,
                SIGNAL(toggled(bool)),
                this,
                SLOT(tagModeToggled(bool))
                );
    QObject::connect(
                ui->applyRadioButton,
                SIGNAL(toggled(bool)),
                this,
                SLOT(tagModeToggled(bool))
                );

    // When tags selections are changed, we need to apply/filter
    QObject::connect(
                ui->tagsTreeView->selectionModel(),
                SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                this,
                SLOT(tagSelectionChanged(QItemSelection,QItemSelection))
                );
}

void MainWindow::tagSelectionChanged(QItemSelection const& selected, QItemSelection const& deselected)
{
    qDebug() << __FUNCSIG__;
//    qDebug() << sender();

    switch (settings->value("tagMode").toInt()) {
        case TagMode::Apply:
        {
            // In case the selection changed, but not via the user (i.e. sample was switched)
            if (!writeSampleTags) {
                return;
            }

            auto selected_sample = ui->samplesTreeView->selectionModel()->selectedIndexes();
            if (selected_sample.empty()) {
                return;
            }
            auto sample = static_cast<Sample*>(selected_sample.last().internalPointer());
            for (auto selRng : selected) {
                for (auto idx : selRng.indexes()) {
                    tagsModel->addSampleTag(*sample, static_cast<TagWrapper*>(idx.internalPointer())->tag);
                }
            }
            for (auto selRng : deselected) {
                for (auto idx : selRng.indexes()) {
                    tagsModel->removeSampleTag(*sample, static_cast<TagWrapper*>(idx.internalPointer())->tag);
                }
            }
        }
        break;
        case TagMode::Filter:
            filterSamples();
        break;
    }
}

void MainWindow::setApplyTagSelections(Sample const& sample)
{
    writeSampleTags = false;

    ui->tagsTreeView->selectionModel()->clearSelection();
    auto tags = db->getSampleTags(sample);
    for (auto t : tags) {
        ui->tagsTreeView->selectionModel()->select(tagsModel->modelIndex(t), QItemSelectionModel::Select);
    }

    writeSampleTags = true;
}

void MainWindow::sampleSelectionChanged(QModelIndex const& selected, QModelIndex const& deselected)
{
    qDebug() << __FUNCSIG__;

    if (!selected.isValid()) {
        return;
    }

    // If we're in "Apply" tag mode, we want to show the relevant tags for this sample
    if (settings->value("tagMode").toInt() == TagMode::Apply) {
        // Disable (then later re-enable) actually writing sample tags
        // If it's disabled, we won't try to change the tags when the tag selections change
        setApplyTagSelections(*static_cast<Sample*>(selected.internalPointer()));
    }

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
    // Get all selected directories
    QList<QDir> selectedDirs;
    for (auto selectionRange : ui->dirsTreeView->selectionModel()->selection()) {
        for (auto modelIdx : selectionRange.indexes()) {
            selectedDirs << QDir(directoriesModel->data(modelIdx, Qt::EditRole).toString());
        }
    }

    // Get all selected tags
    QList<Tag> selectedTags;
    for (auto selectionRange : ui->tagsTreeView->selectionModel()->selection()) {
        for (auto modelIdx : selectionRange.indexes()) {
            selectedTags << static_cast<TagWrapper*>(modelIdx.internalPointer())->tag;
        }
    }

    // Run the filter on the samples model
    samplesModel->setFilter(selectedDirs, selectedTags);
}

void MainWindow::directorySelectionChanged(QItemSelection const& selected, QItemSelection const& deselected)
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
    EditTagsDialog dialog(*db, this);
    dialog.exec();
    tagsModel->refresh();
}

void MainWindow::on_actionOpen_sample_triggered()
{
    // Get last selected sample
    auto selected = ui->samplesTreeView->selectionModel()->selectedIndexes();
    if (selected.empty()) {
        return;
    }

    auto s = static_cast<Sample*>(selected.last().internalPointer());

    #ifdef Q_OS_WIN
    QProcess::startDetached(
                "explorer",
                QStringList() << "/select," << QDir::toNativeSeparators(s->fullPath())
                );
    #endif
}

void MainWindow::tagModeToggled(bool checked)
{
    qDebug() << __FUNCSIG__;
    settings->setValue("tagMode", ui->tagMode->checkedId());

    switch (settings->value("tagMode").toInt()) {
    // If switch to apply, show tags for selected sample
    case TagMode::Apply:
        // Nothing selected...
        if (ui->samplesTreeView->selectionModel()->selection().empty()) {
            return;
        }

        setApplyTagSelections(
                    *static_cast<Sample*>(
                        ui->samplesTreeView->selectionModel()->currentIndex().internalPointer()));
        break;
    // If switch to filter, probably clear the tag selection
    case TagMode::Filter:
        ui->tagsTreeView->selectionModel()->clearSelection();
        break;
    default:
        return;
    }
}
