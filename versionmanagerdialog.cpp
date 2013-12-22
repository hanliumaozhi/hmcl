#include "versionmanagerdialog.h"
#include "ui_versionmanagerdialog.h"

void VersionManagerDialog::gameDirChanged()
{
    ui->txtGameDir->setEnabled(ui->chkGameDir->isChecked());
    ui->btnSelGameDir->setEnabled(ui->chkGameDir->isChecked());
}

void VersionManagerDialog::javaDirChanged()
{
    ui->txtJavaDir->setEnabled(ui->chkJavaDir->isChecked());
    ui->btnSelJavaDir->setEnabled(ui->chkJavaDir->isChecked());
}

void VersionManagerDialog::javaArgsChanged()
{
    ui->txtJavaArgs->setEnabled(ui->chkJavaArgs->isChecked());
}

void VersionManagerDialog::resolutionChanged()
{
    ui->label->setEnabled(ui->chkResolution->isChecked());
    ui->txtWidth->setEnabled(ui->chkResolution->isChecked());
    ui->txtHeight->setEnabled(ui->chkResolution->isChecked());
}

void VersionManagerDialog::fullscreenChanged()
{
    pro.fullscreen = ui->chkFullscreen->isChecked();
    pro.hasFullScreen = true;
}

VersionManagerDialog::VersionManagerDialog(QString ver, Profile pro, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VersionManagerDialog)
{
    ui->setupUi(this);

    this->pro = Profile(pro);

    this->setWindowTitle("Minecraft Version: " + ver);
    ui->chkGameDir->setChecked(pro.hasGameDir);
    if(pro.hasGameDir)
        ui->txtGameDir->setText(pro.gameDir);
    ui->chkJavaDir->setChecked(pro.hasJavaDir);
    if(pro.hasJavaDir)
        ui->txtJavaDir->setText(pro.javaDir);
    ui->chkJavaArgs->setChecked(pro.hasJavaArgs);
    if(pro.hasJavaArgs)
        ui->txtJavaArgs->setText(pro.javaArgs);
    ui->chkResolution->setChecked(pro.hasResolution);
    if(pro.hasResolution)
    {
        ui->txtHeight->setText(QString::number(pro.r.height));
        ui->txtWidth->setText(QString::number(pro.r.width));
    }
    ui->chkFullscreen->setChecked(pro.hasFullScreen);

    gameDirChanged();
    javaArgsChanged();
    javaDirChanged();
    resolutionChanged();
    fullscreenChanged();
}

VersionManagerDialog::~VersionManagerDialog()
{
    delete ui;
}

void VersionManagerDialog::on_btnSelGameDir_clicked()
{
    pro.gameDir = QFileDialog::getExistingDirectory();
    ui->txtGameDir->setText(pro.gameDir);
}

void VersionManagerDialog::on_btnSelJavaDir_clicked()
{
    pro.javaDir = QFileDialog::getExistingDirectory();
    ui->txtJavaDir->setText(pro.javaDir);
}

void VersionManagerDialog::on_chkFullscreen_clicked()
{
    fullscreenChanged();
}

void VersionManagerDialog::on_chkGameDir_clicked()
{
    gameDirChanged();
}

void VersionManagerDialog::on_chkJavaDir_clicked()
{
    javaDirChanged();
}

void VersionManagerDialog::on_chkJavaArgs_clicked()
{
    javaArgsChanged();
}

void VersionManagerDialog::on_chkResolution_clicked()
{
    resolutionChanged();
}

void VersionManagerDialog::on_buttonBox_accepted()
{
    pro.hasGameDir = ui->chkGameDir->isChecked();
    pro.gameDir = ui->txtGameDir->text();
    pro.hasJavaDir = ui->chkJavaDir->isChecked();
    pro.javaDir = ui->txtJavaDir->text();
    pro.hasJavaArgs = ui->chkJavaArgs->isChecked();
    pro.javaArgs = ui->chkJavaArgs->text();
    pro.hasResolution = ui->chkResolution->isChecked();
    pro.r.width = ui->txtWidth->text().toInt();
    pro.r.height = ui->txtHeight->text().toInt();
    pro.hasFullScreen = pro.fullscreen = ui->chkFullscreen->isChecked();

    ProfilesManager::add(pro);
    ProfilesManager::save();
}
