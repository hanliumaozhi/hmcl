#ifndef VERSIONMANAGERDIALOG_H
#define VERSIONMANAGERDIALOG_H

#include <QFileDialog>
#include <QDialog>
#include <QString>
#include "launcherprofileloader.h"

extern bool lpLoaded;
extern LauncherProfileLoader *loader;

namespace Ui {
class VersionManagerDialog;
}

class VersionManagerDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit VersionManagerDialog(QString ver, Profile pro, QWidget *parent = 0);
    ~VersionManagerDialog();
    void refresh();
    void gameDirChanged();
    void javaDirChanged();
    void javaArgsChanged();
    void resolutionChanged();
    void fullscreenChanged();
    
private slots:
    void on_btnSelGameDir_clicked();

    void on_btnSelJavaDir_clicked();

    void on_chkFullscreen_clicked();

    void on_chkGameDir_clicked();

    void on_chkJavaDir_clicked();

    void on_chkJavaArgs_clicked();

    void on_chkResolution_clicked();

    void on_buttonBox_accepted();

private:
    Ui::VersionManagerDialog *ui;
    Profile pro;
};

#endif // VERSIONMANAGERDIALOG_H
