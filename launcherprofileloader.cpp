#include "launcherprofileloader.h"

class LauncherProfileLoader;

bool lpLoaded;
LauncherProfileLoader *loader;

void try_get(const Value v, string key, string def, string &res, bool &has)
{
    Value v2 = v.get(key, Value());
    if(v2.isNull())
    {
        res = def;
        has = false;
    }
    else
    {
        res = v2.asString();
        has = true;
    }
}

void try_get(const Value v, string key, bool def, bool &res, bool &has)
{
    Value v2 = v.get(key, Value());
    if(v2.isNull())
    {
        res = def;
        has = false;
    }
    else
    {
        res = v2.asBool();
        has = true;
    }
}

void try_get(const Value v, QString key, QString def, QString &res, bool &has)
{
    string tmp;
    try_get(v, key.toStdString(), def.toStdString(), tmp, has);
    res = QString(tmp.c_str());
}

void try_get(const Value v, string key, Resolution &res, bool &has)
{
    Value v2 = v.get(key, Value());
    if(v2.isNull())
    {
        res.height = 0;
        res.width = 0;
        has = false;
    }
    else
    {
        res.height = v2["width"].asInt();
        res.width = v2["width"].asInt();
        has = true;
    }
}

void try_get(const Value v, string key, bool &has)
{
    Value v2 = v.get(key, Value());
    if(v2.isNull())
    {
        has = false;
    }
    else
    {
        has = true;
    }
}

LauncherProfileLoader::LauncherProfileLoader(QString lp)
{
    Reader r;
    Value jso;
    if(lp.length() == 0)
    {
        selectedProfile = "";
        clientToken = "";
    }
    else
    {
        r.parse(lp.toStdString(), jso);
        Json::Value res = jso.get("authenticatiobDatabase", Json::Value());
        if(!res.isNull())
        {
            for(Json::Value::iterator i = res.begin(); i != res.end(); i++)
            {
                Authentication a;
                a.accessToken = QString((*i)["accessToken"].asString().c_str());
                a.displayName = QString((*i)["displayName"].asString().c_str());
                a.username = QString((*i)["username"].asString().c_str());
                a.uuid = QString((*i)["uuid"].asString().c_str());
            }
        }
        Value pf = jso.get("profiles", Value());
        if(!pf.isNull())
        {
            for(Value::iterator i = pf.begin(); i != pf.end(); i++)
            {
                Value pp = *i;
                Profile p(QString(pp["name"].asString().c_str()));
                try_get(pp, "gameDir", "", p.gameDir, p.hasGameDir);
                try_get(pp, "javaDir", "", p.javaDir, p.hasJavaDir);
                try_get(pp, "javaArgs", "", p.javaArgs, p.hasJavaArgs);
                try_get(pp, "lastVersionId", "", p.lastVersionId, p.hasLastVersionId);
                try_get(pp, "playerUUID", "", p.playerUUID, p.hasPlayerUUID);
                try_get(pp, "fullscreen", false, p.fullscreen, p.hasFullScreen);
                try_get(pp, "resolution", p.r, p.hasResolution);
                profiles.push_back(p);
            }
        }
    }
}

QString LauncherProfileLoader::generateString()
{
    Value jso, pro, procontent, resln, auth;
    Value lst;
    QString s;
    int i;
    jso["selectedProfile"] = selectedProfile.toStdString();
    jso["clientToken"] = selectedProfile.toStdString();
    for(i = 0; i < authenticationDatabase.size(); i++)
    {
        resln["username"] = authenticationDatabase[i].username.toStdString();
        resln["accessToken"] = authenticationDatabase[i].accessToken.toStdString();
        resln["uuid"] = authenticationDatabase[i].uuid.toStdString();
        resln["displayName"] = authenticationDatabase[i].displayName.toStdString();
        auth[authenticationDatabase[i].uuid.toStdString()] = resln;
    }
    jso["authenticationDatabase"] = auth;

    for(i = 0; i < profiles.size(); i++)
    {
        procontent = Value();
        procontent["name"] = profiles[i].name.toStdString();
        if(profiles[i].hasGameDir)
            procontent["gameDir"] = profiles[i].gameDir.toStdString();
        if(profiles[i].hasJavaArgs)
            procontent["javaArgs"] = profiles[i].javaArgs.toStdString();
        if(profiles[i].hasJavaDir)
            procontent["javaDir"] = profiles[i].javaDir.toStdString();
        if(profiles[i].hasPlayerUUID)
            procontent["playerUUID"] = profiles[i].playerUUID.toStdString();
        if(profiles[i].hasFullScreen)
            procontent["fullscreen"] = profiles[i].fullscreen;
        if(profiles[i].hasResolution)
        {
            resln = Value();
            resln["width"] = profiles[i].r.width;
            resln["height"] = profiles[i].r.height;
            procontent["resolution"] = resln;
        }
        if(profiles[i].hasAllowedReleaseTypes)
        {
            lst = Value();
            lst.append(Value("snapshot"));
            lst.append(Value("release"));
        }
        pro[profiles[i].name.toStdString()] = procontent;
    }
    jso["profiles"] = pro;
    s = QString(jso.toStyledString().c_str());
    return s;
}

void ProfilesManager::load()
{
    QString j = QString(FileUtilities::readToEnd((FileUtilities::addSeparator(minecraftPath) + "launcher_profiles.json").toStdString().c_str()).c_str());
    loader = new LauncherProfileLoader(j);
    lpLoaded = true;
}

void ProfilesManager::save()
{
    if(lpLoaded)
    {
        FileUtilities::writeToFile((FileUtilities::addSeparator(minecraftPath)
                                   + "launcher_profiles.json").toStdString().c_str(), loader->generateString().toStdString());
    }
}

Profile *ProfilesManager::find(QString name)
{
    int i;
    if(!lpLoaded) return NULL;
    for(i = 0; i < loader->profiles.size(); i++)
    {
        if(loader->profiles[i].name == name)
            return &loader->profiles[i];
    }
    return NULL;
}

void ProfilesManager::add(Profile p)
{
    Profile *p2 = ProfilesManager::find(p.name);
    if(p2 == NULL)
    {
        loader->profiles.push_back(p);
    }
    else
    {
        *p2 = p;
    }
}
