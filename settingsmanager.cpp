#include "settingsmanager.h"
#include <QDebug>

QString playerName, minecraftPath, launchMode, javaPath, last;
int maxMemory;
bool loaded, debug;

void SettingsManager::load()
{
    QFile f(SettingsManager::d.currentPath() + "/settings.json");
    if(f.open(QFile::ReadOnly))
    {
        QTextStream ts(&f);
        QString str, line;
        while(!ts.atEnd()) {
            line = ts.readLine();
            str += line;
        }
        Json::Reader reader;
        Json::Value root;
        std::string stdstring = str.toStdString();
        reader.parse(stdstring,root);
        playerName = QString(root["PlayerName"].asString().c_str());
        minecraftPath = QString(root["MinecraftPath"].asString().c_str());
        javaPath = QString(root["JavaPath"].asString().c_str());
        launchMode = QString(root["LaunchMode"].asString().c_str());
        last = QString(root["Last"].asString().c_str());
        maxMemory = root["MaxMemory"].asUInt();
        debug = root["Debug"].asBool();
        loaded = true;
    }
    else
    {
        playerName = "Player007";
        minecraftPath = FileUtilities::qhome().append("/.minecraft");
        maxMemory = SystemManager::getMemorySize() / 2;
        debug = false;
        last = "";
        launchMode = "Normal";
        javaPath = "";
        loaded = true;
        save();
    }
}

void SettingsManager::save()
{
    if(!loaded) return;
    QString path = d.currentPath();
    QFile file(path + "/settings.json");
    Json::FastWriter writer;
    Json::Value root;
    root["PlayerName"] = playerName.toStdString();
    root["MinecraftPath"] = minecraftPath.toStdString();
    root["JavaPath"] = javaPath.toStdString();
    root["Last"] = last.toStdString();
    root["MaxMemory"] = Json::Value(maxMemory);
    root["LaunchMode"] = launchMode.toStdString();
    root["Debug"] = debug;

    const char *c = (path + "/settings.json").toStdString().c_str();

    ofstream fout(c);
    fout<<root.toStyledString();
    fout.close();
}
