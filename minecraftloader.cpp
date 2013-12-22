#include "minecraftloader.h"
#include <QDebug>

MinecraftLibrary::MinecraftLibrary(QString name, Json::Value rules)
{
    this->oldText = name;
    QStringList s = name.split(':');
    QString str = s[0];
    for(int j = 0; j < str.length(); j++)
        if(str[j] == '.')
            str[j] = '/';
    str = str + "/" + s[1] + "/" + s[2] + "/" + s[1] + "-" + s[2] + ".jar";
    this->formatted = str;

    if(!rules.isNull())
    {
        for(int i = 0; i < rules.size(); i++)
        {
            Json::Value rule = rules[i];
            Rules r;
            Json::Value action = rule.get("action", Json::Value());
            if(!action.isNull())
            {
                r.action = QString(rule["action"].asString().c_str());
                r.osName = QString(rule["os"].asString().c_str());
                r.osVersion = QString(rule["version"].asString().c_str());
                this->rules.append(r);
            }
        }
    }
}

MinecraftUnpackingLibrary::MinecraftUnpackingLibrary(QString name, Json::Value rules, Json::Value natives)
{
    this->oldText = name;
    QStringList s = name.split(':');
    QString str = s[0];
    for(int j = 0; j < str.length(); j++)
        if(str[j] == '.')
            str[j] = '/';

    if(!natives.isNull())
    {
        this->native.Windows = QString(natives.get("windows", Json::Value("natives-windows")).asString().c_str());
        this->native.Linux = QString(natives.get("linux", Json::Value("natives-linux")).asString().c_str());
        this->native.OSX = QString(natives.get("osx", Json::Value("natives-osx")).asString().c_str());
    }

    str = str + "/" + s[1] + "/" + s[2] + "/" + s[1] + "-" + s[2] + "-" + this->native.Linux + ".jar";
    this->formatted = str;

    if(!rules.isNull())
    {
        for(int i = 0; i < rules.size(); i++)
        {
            Json::Value rule = rules[i];
            Rules r;
            Json::Value action = rule.get("action", Json::Value());
            if(!action.isNull())
            {
                r.action = QString(rule["action"].asString().c_str());
                r.osName = QString(rule["os"].asString().c_str());
                r.osVersion = QString(rule["version"].asString().c_str());
                this->rules.append(r);
            }
        }
    }
}

MinecraftLoader::MinecraftLoader(QString mcp, QString mvt, QString lm, QString jp,
                                 QString pn, QString ss, int mm, Profile *pro)
{
    this->MinecraftPath = mcp;
    this->GameDir = mcp;
    this->GameAssets = FileUtilities::addSeparator(mcp) + "assets";
    this->JavaPath = jp;
    this->MinecraftVersionText = mvt;
    if(lm == "64Bit") this->LaunchMode = "-d64";
    else if(lm == "32Bit") this->LaunchMode = "-d32";
    else this->LaunchMode = "";
    this->PlayerName = pn;
    this->Session = ss;
    this->MaxMemory = mm;

    reader.parse(mvt.toStdString(), this->MinecraftVersionJson);
    this->ID = QString(MinecraftVersionJson["id"].asString().c_str());
    this->VersionName = ID;
    this->Time = QString(MinecraftVersionJson["time"].asString().c_str());
    this->ReleaseTime = QString(MinecraftVersionJson["releaseTime"].asString().c_str());
    this->Type = QString(MinecraftVersionJson["type"].asString().c_str());
    this->MinecraftArguments = QString(MinecraftVersionJson["minecraftArguments"].asString().c_str());
    this->MainClass = QString(MinecraftVersionJson["mainClass"].asString().c_str());

    Json::Value lib = MinecraftVersionJson["libraries"];
    for(Json::Value::iterator i = lib.begin();
        i != lib.end(); i++)
    {
            string s = (*i)["name"].asString();
            if(!(*i).get("natives", Json::Value()).isNull())
            {
                unPackingLibraries.append(MinecraftUnpackingLibrary(QString(s.c_str()),
                                                                    (*i).get("rules", Json::Value()), (*i).get("natives", Json::Value())));
            }
            if(s == "") break;
            libraries.append(MinecraftLibrary(QString(s.c_str()), (*i).get("rules", Json::Value())));
    }

    if(pro != NULL)
    {
        this->profile = *pro;
        if(pro->hasGameDir)
            GameDir = pro->gameDir;
        if(pro->hasJavaDir)
            JavaPath = pro->javaDir;
    }
}

QString MinecraftLoader::generateLaunchString()
{
    QString res("\"");
    if(JavaPath.length() == 0)
        res += "java";
    else
        res += FileUtilities::addSeparator(JavaPath) + "java";

    res += "\" ";
    if(profile.hasJavaArgs && profile.javaArgs.indexOf("-Xincgc") == 0)
        res += "-Xincgc ";
    if(profile.hasJavaArgs && profile.javaArgs.indexOf("-d") == 0)
        res += LaunchMode + " ";
    if(profile.hasJavaArgs && profile.javaArgs.indexOf("-Xmx") == 0)
        res += "-Xmx" + QString::number(MaxMemory) + "m ";
    res += "-Dfml.ignoreInvalidMinecraftCertificates=true -Dfml.ignorePatchDiscrepancies=true ";
    res += "-Djava.library.path=\"";
    res += FileUtilities::addSeparator(MinecraftPath) + "versions/" + ID + "/" + ID + "-natives\"";
    res += " -cp ";
    for(int i = 0; i < libraries.count(); i++)
    {
        bool flag = false;
        if(libraries[i].rules.isEmpty())
            flag = true;
        else for(int j = 0; j < libraries[i].rules.count(); j++)
        {
            if(libraries[i].rules[j].action == "disallow")
            {
                if(libraries[i].rules[j].osName == "" ||
                   libraries[i].rules[j].osName.toLower().trimmed() == "linux")
                {
                    flag = false;
                    break;
                }
            }
            else
            {
                if(libraries[i].rules[j].osName == "" ||
                        libraries[i].rules[j].osName == "linux")
                {
                    flag = true;
                    break;
                }
            }
        }
        if(flag)
            res += "\"" + FileUtilities::addSeparator(MinecraftPath) + "libraries/" +
                    libraries[i].formatted + "\":";
    }
    res += "\"" + FileUtilities::addSeparator(MinecraftPath) + "versions/" + ID + "/" + ID + ".jar\" ";
    res += MainClass + " ";
    QString t = MinecraftArguments;
    t = t.replace("${auth_player_name}", PlayerName);
    t = t.replace("${auth_session}", Session);
    t = t.replace("${version_name}", VersionName);
    t = t.replace("${game_directory}", "\"" + GameDir + "\"");
    t = t.replace("${game_assets}", "\"" + GameAssets + "\"");
    if(profile.hasFullScreen && profile.fullscreen)
        t += " --fullscreen";
    if(profile.hasResolution)
        t += QString(" --height ") + QString::number(profile.r.height)
                + " --width " + QString::number(profile.r.width) + " ";
    res += t;
    return res;
}
