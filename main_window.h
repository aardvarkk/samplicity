#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSortFilterProxyModel>
#include <QUndoStack>

#include "audio_player.h"
#include "database.h"
#include "directories_model.h"
#include "samples_model.h"
#include "tags_model.h"
#include "settings.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setApplyTagSelections(Sample const& sample);
private slots:
    void on_actionExit_triggered();

    void on_actionAddDirectory_triggered();

    void on_actionAddFile_triggered();

    void on_dirsTreeViewSelectionChanged(QItemSelection const& selected, QItemSelection const& deselected);

    void sampleSelectionChanged(QModelIndex const& selected, QModelIndex const& deselected);

    void on_actionLoop_Playback_toggled(bool arg1);

    void on_actionTags_triggered();

    void on_actionOpen_sample_triggered();

    void tagModeToggled(bool checked);

    void tagSelectionChanged(QItemSelection const& selected, QItemSelection const& deselected);

private:
    void filterSamples();

    QSettings* settings;
    Ui::MainWindow* ui;
    Database* db;
    QUndoStack* undoStack;
    DirectoriesModel* directoriesModel;

    QSortFilterProxyModel tagsProxyModel;
    TagsModel* tagsModel;

    SamplesModel* samplesModel;
    AudioPlayer* audioPlayer;

    bool writeSampleTags;
};

#endif // MAINWINDOW_H
