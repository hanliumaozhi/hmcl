#ifndef MINECRAFTLOADER_H
#define MINECRAFTLOADER_H

#include <QString>
#include <json/reader.h>
#include <json/value.h>
#include <QStringList>
#include <QVector>
#include <QComboBox>
#include "FileUtilities.h"
#include "launcherprofileloader.h"

extern bool lpLoaded;
extern LauncherProfileLoader *loader;

struct Rules
{
    QString action, osName, osVersion;
};

struct MinecraftLibrary
{
    MinecraftLibrary(){}
    MinecraftLibrary(QString name, Json::Value rules);
    QString oldText, formatted;
    QVector<Rules> rules;
};

struct Native
{
    QString Linux, Windows, OSX;
};

struct MinecraftUnpackingLibrary : MinecraftLibrary
{
    Native native;
    QVector<QString> excludes;
    MinecraftUnpackingLibrary(){}
    MinecraftUnpackingLibrary(QString name, Json::Value rules, Json::Value natives);
};

class MinecraftLoader
{
private:
//    void formatLibrary(QString name, MinecraftLibrary &res, QJSON rules);
    QString MinecraftPath, MinecraftVersionText, LaunchMode, PlayerName,
        Session, AuthSession, GameDir, GameAssets, JavaPath;
    QString ID, Time, ReleaseTime, Type, ProcessArguments, MinecraftArguments,
        MainClass, VersionName;
    int MaxMemory;
    Json::Value MinecraftVersionJson;
    Json::Reader reader;

    Profile profile;
public:
    MinecraftLoader(QString mcp, QString mvt, QString lm, QString jp,
                    QString pn, QString ss, int mm, Profile *pro);
    QString generateLaunchString();
    QVector<MinecraftUnpackingLibrary> unPackingLibraries;
    QVector<MinecraftLibrary> libraries;
};

#endif // MINECRAFTLOADER_H
