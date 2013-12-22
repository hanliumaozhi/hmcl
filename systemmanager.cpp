#include "systemmanager.h"
#include <exception>
using namespace std;

struct sysinfo *SystemManager::getMemoryInfo()
{
    struct sysinfo *info;
    int error = sysinfo(info);
    if(error != 0) return NULL;
    return info;
}

int SystemManager::getMemorySize()
{
    return 2048;
}
