#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QLineEdit>
#include <QCheckBox>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QDialog>
#include <string>
#include <QVector>
using namespace std;
#include "minecraftloader.h"
#include "FileUtilities.h"
#include "settingsmanager.h"
#include "minecraftoldversionincluder.h"
#include "launcherprofileloader.h"
#include "versionmanagerdialog.h"

namespace Ui {
class MainWindow;
}

void refreshMinecraftVersion(QString mp, QComboBox *cb, QString last);


extern QString playerName, minecraftPath, launchMode, javaPath, last;
extern int maxMemory;
extern bool loaded, debug;

extern bool lpLoaded;
extern LauncherProfileLoader *loader;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString generateLaunchString();
    
private slots:
    void on_btnRun_clicked();

    void on_btnSelHome_clicked();

    void on_btnSelGoodMem_clicked();

    void on_btnSelMCPath_clicked();

    void on_txtMinecraftPath_editingFinished();

    void on_rdo64Bit_clicked();

    void on_rdo32Bit_clicked();

    void on_rdoNormal_clicked();

    void on_btnSelJavaPath_clicked();

    void on_txtJavaPath_editingFinished();

    void on_txtMaxMemory_editingFinished();

    void on_txtPlayerName_editingFinished();

    void on_cboVersion_currentIndexChanged(const QString &arg1);

    void on_btnMakeSh_clicked();

    void on_btnInclude_clicked();

    void on_btnManageVersion_clicked();

private:
    Ui::MainWindow *ui;

private:
    QProcess *pro;
    QLineEdit *txtMinecraftPath, *txtJavaPath, *txtPlayerName;
    QCheckBox *chkUseJava, *chk64Bit;
};

#endif // MAINWINDOW_H
