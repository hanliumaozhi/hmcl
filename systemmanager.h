#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H
#include <sys/sysinfo.h>
#include <stdlib.h>
#include <QFile>

class SystemManager
{
public:
    static struct sysinfo *getMemoryInfo();
    static int getMemorySize();
};

#endif // SYSTEMMANAGER_H
