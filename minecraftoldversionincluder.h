#ifndef MINECRAFTOLDVERSIONINCLUDER_H
#define MINECRAFTOLDVERSIONINCLUDER_H

#include <json/writer.h>
#include <json/value.h>
#include "FileUtilities.h"
#include <json/writer.h>
#include <json/value.h>
#include <string>
#include <fstream>
using namespace std;
#include <QProcess>
#include "settingsmanager.h"
#include <zlib-1.2.8/zlib.h>

extern QString playerName, minecraftPath, launchMode, javaPath, last;
extern int maxMemory;
extern bool loaded, debug;

class MinecraftOldVersionIncluder
{
private:
    QString path, name;
public:
    MinecraftOldVersionIncluder(QString path, QString name);
    void include();
};

#endif // MINECRAFTOLDVERSIONINCLUDER_H
