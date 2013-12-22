#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

void refreshMinecraftVersion(QString mp, QComboBox *cb, QString last)
{
    QStringList sl = FileUtilities::findAllDir(FileUtilities::addSeparator(mp) + "versions");
    cb->clear();
    int j = -1;
    for(int i = 0; i < sl.length(); i++)
    {
        cb->addItem(sl[i]);
        if(sl[i] == last)
            j = i;
    }
    if(j != -1) cb->setCurrentIndex(j);
}

QVector<MinecraftUnpackingLibrary> CheckUnpackingLibraries(MinecraftLoader l)
{
    QVector<MinecraftUnpackingLibrary> result;
    for(int i = 0; i < l.unPackingLibraries.count(); i++)
    {
        bool flag = false;
        if(l.unPackingLibraries[i].rules.count() == 0)
          flag = true;
        for(int j = 0; j < l.unPackingLibraries[i].rules.count(); j++)
            if(l.unPackingLibraries[i].rules[j].action == "disallow")
            {
                if(l.unPackingLibraries[i].rules[j].osName == "" ||
                        l.unPackingLibraries[i].rules[j].osName.toLower().trimmed() == "linux")
                {
                    flag = false;
                    break;
                }
            }
            else
            {
                if(l.unPackingLibraries[i].rules[j].osName == "" ||
                        l.unPackingLibraries[i].rules[j].osName.toLower().trimmed() == "linux")
                {
                    flag = true;
                    break;
                }
            }
        if(flag)
        {
            qDebug() << FileUtilities::addSeparator(minecraftPath) + "libraries/" + l.unPackingLibraries[i].formatted;
            QFile fi(FileUtilities::addSeparator(minecraftPath) + "libraries/" + l.unPackingLibraries[i].formatted);
            if(fi.exists())
                result.append(l.unPackingLibraries[i]);
        }
    }
    return result;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    SettingsManager::load();
    ProfilesManager::load();
    ui->txtJavaPath->setText(javaPath);
    ui->txtMinecraftPath->setText(minecraftPath);
    ui->txtMaxMemory->setText(QString::number(maxMemory));
    ui->txtPlayerName->setText(playerName);
    qDebug() << launchMode;
    if(launchMode.toLower() == "64bit")
    {
        ui->rdoNormal->setChecked(false);
        ui->rdo32Bit->setChecked(false);
        ui->rdo64Bit->setChecked(true);
    }
    else if(launchMode.toLower() == "32bit")
    {
        ui->rdoNormal->setChecked(false);
        ui->rdo32Bit->setChecked(true);
        ui->rdo64Bit->setChecked(false);
    }
    else
    {
        ui->rdoNormal->setChecked(true);
        ui->rdo32Bit->setChecked(false);
        ui->rdo64Bit->setChecked(false);
    }

    refreshMinecraftVersion(minecraftPath, ui->cboVersion, last);
}

MainWindow::~MainWindow()
{
    delete ui;
    SettingsManager::save();
}

QString MainWindow::generateLaunchString()
{
    QString str = ui->cboVersion->currentText();
    if(str.length() == 0)
    {
        QMessageBox::information(this, "Error", "Not selected any minecraft version.", QMessageBox::Ok);
        return "";
    }
    QFile file(FileUtilities::addSeparator(minecraftPath).append("versions/").append(str).append("/").append(str).append(".json"));
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    MinecraftLoader ml(FileUtilities::addSeparator(minecraftPath),QString(file.readAll()),launchMode,javaPath,playerName,QString("no"),512,
                       ProfilesManager::find(str));

    QVector<MinecraftUnpackingLibrary> lst = CheckUnpackingLibraries(ml);

    QString realpa = FileUtilities::addSeparator(minecraftPath);

    QString directory = realpa + "versions/" + str + "/" + str + "-natives";
    FileUtilities::deleteDir(directory);
    QDir dir(directory);
    if(!dir.exists())
        dir.mkdir(dir.path());
    for(int i = 0; i < lst.count(); i++)
    {
        QProcess::startDetached(QString("unzip -o -d ").append(directory).append(" ").append(realpa + "libraries/" + lst[i].formatted));
    }

    QDir dmods(realpa + "mods");
    QDir dvermods(realpa + "versions/" + str + "/mods");
    if(dmods.exists())
    {
        FileUtilities::deleteDir(dmods.path());
        FileUtilities::copyDir(dvermods, dmods, true);
    }
    QDir dcoremods(realpa + "coremods");
    QDir dvercoremods(realpa + "versions/" + str + "/coremods");
    if(dcoremods.exists())
    {
        FileUtilities::deleteDir(dcoremods.path());
        FileUtilities::copyDir(dvercoremods, dcoremods, true);
    }
    QDir dconfig(realpa + "config");
    QDir dverconfig(realpa + "versions/" + str + "/config");
    if(dmods.exists())
    {
        FileUtilities::deleteDir(dconfig.path());
        FileUtilities::copyDir(dverconfig, dconfig, true);
    }

    last = str;
    SettingsManager::save();

    loader->selectedProfile = str;
    ProfilesManager::save();

    return ml.generateLaunchString();
}

void MainWindow::on_btnRun_clicked()
{
    QProcess::startDetached(generateLaunchString());
}

void MainWindow::on_btnMakeSh_clicked()
{
    QString str = QFileDialog::getSaveFileName(this,"","","SH File(*.sh)");
    QFile file(str);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.write(generateLaunchString().toStdString().c_str());
    file.close();
    QProcess::startDetached(QString("chmod 777 ").append(str));
}

void MainWindow::on_btnSelHome_clicked()
{
    ui->txtMinecraftPath->setText(FileUtilities::qhome().append("/.minecraft"));
    minecraftPath = FileUtilities::qhome().append("/.minecraft");
    refreshMinecraftVersion(minecraftPath, ui->cboVersion, "");
}

void MainWindow::on_btnSelGoodMem_clicked()
{
    long ram = SystemManager::getMemorySize() / 2;
    QString s = QString::number(ram);
    ui->txtMaxMemory->setText(s);
    maxMemory = ram;
    SettingsManager::save();
}

void MainWindow::on_btnSelMCPath_clicked()
{
    QString str = QFileDialog::getExistingDirectory(this);
    minecraftPath = str;
    ui->txtMinecraftPath->setText(str);
    SettingsManager::save();
}

void MainWindow::on_txtMinecraftPath_editingFinished()
{
    minecraftPath = ui->txtMinecraftPath->text();
    SettingsManager::save();
    refreshMinecraftVersion(minecraftPath, ui->cboVersion, "");
}

void MainWindow::on_txtJavaPath_editingFinished()
{
    javaPath = ui->txtJavaPath->text();
    SettingsManager::save();
}

void MainWindow::on_txtMaxMemory_editingFinished()
{
    maxMemory = ui->txtMaxMemory->text().toInt();
    SettingsManager::save();
}

void MainWindow::on_txtPlayerName_editingFinished()
{
    playerName = ui->txtPlayerName->text();
    SettingsManager::save();
}

void MainWindow::on_rdo64Bit_clicked()
{
    launchMode = "64Bit";
    SettingsManager::save();
}

void MainWindow::on_rdo32Bit_clicked()
{
    launchMode = "32Bit";
    SettingsManager::save();
}

void MainWindow::on_rdoNormal_clicked()
{
    launchMode = "Normal";
    SettingsManager::save();
}

void MainWindow::on_btnSelJavaPath_clicked()
{
    QString str = QFileDialog::getExistingDirectory(this);
    javaPath = str;
    ui->txtJavaPath->setText(str);
    SettingsManager::save();
}

void MainWindow::on_cboVersion_currentIndexChanged(const QString &arg1)
{
    last = arg1;
    SettingsManager::save();
}

void MainWindow::on_btnInclude_clicked()
{
    QString mcp = QFileDialog::getExistingDirectory();
    QFile file(FileUtilities::addSeparator(mcp) + "bin/minecraft.jar");
    if(!file.exists())
    {
        QMessageBox::information(this, "Error", "Not found minecraft.jar", QMessageBox::Ok);
        return;
    }
    QString name = QInputDialog::getText(this, "Include Old Minecraft", "请输入名字");
    QDir dir(FileUtilities::addSeparator(minecraftPath).append("versions/").append(name));
    if(dir.exists())
    {
        QMessageBox::information(this, "Error", "找到同名Minecraft版本", QMessageBox::Ok);
        return;
    }

    MinecraftOldVersionIncluder l(mcp, name);
    l.include();
    SettingsManager::save();
    refreshMinecraftVersion(minecraftPath, ui->cboVersion, last);
}

void MainWindow::on_btnManageVersion_clicked()
{
    QString name = ui->cboVersion->currentText();
    if(name == "")
    {
        QMessageBox::information(this, "Error", "Not selected true minecraft version.", QMessageBox::Ok);
        return;
    }

    Profile *p = ProfilesManager::find(name);
    if(p == NULL)
    {
        p = new Profile(name);
    }

    VersionManagerDialog dlg(name, *p, this);
    dlg.exec();
}
