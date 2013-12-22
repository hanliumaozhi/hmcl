#ifndef LAUNCHERPROFILELOADER_H
#define LAUNCHERPROFILELOADER_H

#include <QString>
#include <string>
#include <vector>
#include <json/reader.h>
#include <json/writer.h>
#include <json/value.h>
#include "settingsmanager.h"
#include "FileUtilities.h"
using namespace Json;
using namespace std;

extern QString playerName, minecraftPath, javaPath, launchMode, last;
extern int maxMemory;
extern bool loaded, debug;

class LauncherProfileLoader;

extern bool lpLoaded;
extern LauncherProfileLoader *loader;

class Authentication
{
public:
    QString username, accessToken, uuid, displayName;
};

class Resolution
{
public:
    int height, width;
    Resolution(){}
    Resolution(int w, int h):height(h), width(w){}
};

class Profile
{
public:
    QString name, gameDir, javaDir, javaArgs, lastVersionId,playerUUID;
    bool hasGameDir, hasJavaDir, hasJavaArgs;
    bool hasResolution, hasAllowedReleaseTypes;
    bool hasLastVersionId, hasPlayerUUID, hasFullScreen;
    bool fullscreen;
    Resolution r;

    Profile(){}
    Profile(QString n):name(n){
        hasAllowedReleaseTypes = hasGameDir = hasJavaArgs =
                hasJavaDir = hasLastVersionId = hasPlayerUUID =
                hasResolution = hasFullScreen = false;
    }
};

class LauncherProfileLoader
{
public:
    QString selectedProfile, clientToken;
    vector<Profile> profiles;
    vector<Authentication> authenticationDatabase;

    LauncherProfileLoader(QString lp);
    QString generateString();
};

class ProfilesManager
{
public:
    static void load();
    static void save();
    static void add(Profile p);
    static Profile *find(QString name);
};

#endif // LAUNCHERPROFILELOADER_H
