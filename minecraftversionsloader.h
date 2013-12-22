#ifndef MINECRAFTVERSIONSLOADER_H
#define MINECRAFTVERSIONSLOADER_H

#include <QString>
#include <QVector>
#include <json/reader.h>
using namespace Json;

struct MinecraftVersion
{
    QString id, time, releaseTime, mtype;
};

class MinecraftVersionsLoader
{
    QString json;
    Reader reader;
    Value jso;

public:
    MinecraftVersionsLoader(QString j);

    QString snapshot, release;
    QVector<MinecraftVersion> versions;
};

#endif // MINECRAFTVERSIONSLOADER_H
