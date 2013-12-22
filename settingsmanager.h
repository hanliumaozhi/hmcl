#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <string>
#include <QDir>
#include <QTextStream>
#include <json/reader.h>
#include <json/writer.h>
#include "systemmanager.h"
#include "FileUtilities.h"
#include <fstream>
using namespace std;

extern QString playerName, minecraftPath, javaPath, launchMode, last;
extern int maxMemory;
extern bool loaded, debug;

class SettingsManager
{
public:
    static QDir d;
    static void load();
    static void save();
};

#endif // SETTINGSMANAGER_H
