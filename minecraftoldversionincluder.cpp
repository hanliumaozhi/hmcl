#include "minecraftoldversionincluder.h"
#include <QDebug>

MinecraftOldVersionIncluder::MinecraftOldVersionIncluder(QString path, QString name)
{
    this->path = path;
    this->name = name;
}

void MinecraftOldVersionIncluder::include()
{
    QString inc = FileUtilities::addSeparator(minecraftPath);
    QString newname = name;
    int i;
    for(i = 0; i < newname.length(); i++)
        if(newname[i] == '.')
            newname[i] = '/';
    QDir d(QString(inc).append("versions/").append(name));
    d.mkpath(d.path());
    QString pa = FileUtilities::addSeparator(path);
    QFile file(QString(pa) + "bin/minecraft.jar");
    file.copy(QString(inc).append("versions/").append(name).append("/").append(name).append(".jar"));

    qDebug() << "H:" << QString(inc).append("libraries/").append(newname).append("/natives/HML/");
    QDir dnative(QString(inc).append("libraries/").append(newname).append("/natives/HML/"));
    if(!dnative.exists())
        dnative.mkpath(dnative.path());

    qDebug() << QString("zip ").append(inc).append("libraries/").append(newname).append("/natives/HML/natives-HML-natives-linux.jar").append(" ").append(QString(pa).append("bin/natives"));
    qDebug() << "Working: " << QString(pa).append("bin/natives/");
    //pro.setWorkingDirectory(QString(pa).append("bin/natives/"));
    //pro.start(QString("zip ").append(inc).append("libraries/").append(newname).append("/natives/HML/natives-HML-natives-linux.jar").append(" ").append("*.*"));
//    QProcess::startDetached(QString("zip ").append(inc).append("libraries/").append(newname).append("/natives/HML/natives-HML-natives-linux.jar").append(" ").append(QString(pa).append("bin/natives")));
//    pro.start(QString("zip -r /home/jack/.minecraft/libraries/2/natives/HML/natives-HML-natives-linux.jar *.*"));
//    QStringList args;
//    args.append(QString(inc).append("libraries/").append(newname).append("/natives/HML/natives-HML-natives-linux.jar"));
//    args.append(QString(pa).append("bin/natives/*"));
    //args.append("*.*");

//    QProcess::startDetached(QString("zip"), args, QString(pa).append("bin/natives"));
    //QProcess::startDetached(QString("zip ").append(inc).append("libraries/").append(newname).append("/natives/HML/natives-HML-natives-linux.jar ").append("*.*"), QStringList(), QString(pa).append("bin/natives/"));

    QStringList sl = FileUtilities::findAllFileWithPath(QString(pa).append("bin/natives"));
    QString jars = "";
    foreach(QString s, sl)
    {
        jars += s + " ";
    }

    QProcess::startDetached(QString("zip -D -j ").append(inc).append("libraries/").append(newname).append("/natives/HML/natives-HML-natives-linux.jar ").append(jars));
    QDir dmods(QString(pa).append("mods"));
    QDir dcoremods(QString(pa).append("coremods"));
    QDir dconfig(QString(pa).append("config"));
    qDebug() << pa;

    if(dmods.exists())
        FileUtilities::copyDir(dmods, QString(inc).append("versions/").append(name).append("/mods"), true);
    if(dcoremods.exists())
        FileUtilities::copyDir(dcoremods, QString(inc).append("versions/").append(name).append("/coremods"), true);
    if(dconfig.exists())
        FileUtilities::copyDir(dconfig, QString(inc).append("versions/").append(name).append("/config"), true);

    Json::Value jso;
    jso["id"] = name.toStdString();
    jso["time"] = "";
    jso["releaseTime"] = "";
    jso["minecraftArguments"] = "${auth_player_name} ${auth_session}";
    jso["minimumLauncherVersion"] = 0;
    jso["mainClass"] = "net.minecraft.client.Minecraft";
    QStringList s = FileUtilities::findAllFile(QString(pa).append("bin"));
    Json::Value libs, lib, native;
    for(i = 0; i < s.length(); i++)
    {
        QFileInfo info(s[i]);
        QString f = info.fileName();
        QString na = f.mid(0, f.length() - 4);
        if(f != "minecraft.jar")
        {
            qDebug() << "dlib: " << QString(inc).append("libraries/").append(newname).append("/").append(na).append("/HML");
            QDir dlib(QString(inc).append("libraries/").append(newname).append("/").append(na).append("/HML"));
            dlib.mkpath(dlib.path());
            qDebug() << "from: " << QString(pa).append("bin/").append(s[i]);
            qDebug() << "to: " << QString(inc).append("libraries/").append(newname).append("/").append(na).append("/HML/").append(na).append("-HML.jar");
            QFileInfo from(QString(pa).append("bin/").append(s[i]));
            QFileInfo to(QString(inc).append("libraries/").append(newname).append("/").append(na).append("/HML/").append(na).append("-HML.jar"));
            QFile::copy(from.filePath(), QString(inc).append("libraries/").append(newname).append("/").append(na).append("/HML/").append(na).append("-HML.jar"));
            lib = Json::Value();
            lib["name"] = QString(name).append(":").append(na).append(":HML").toStdString();
            libs.append(lib);
        }
    }
    lib = Json::Value();
    lib["name"] = QString(name).append(":").append("natives:HML").toStdString();
    native["linux"] = "natives-linux";
    lib["natives"] = native;
    libs.append(lib);

    jso["libraries"] = libs;
    qDebug() << inc;
    qDebug() << QString(inc).append("versions/").append(name).append("/").append(name).append(".json").toStdString().c_str();
    ofstream fout(QString(inc).append("versions/").append(name).append("/").append(name).append(".json").toStdString().c_str());
    fout<<jso.toStyledString();
    fout.close();
}
