#include "minecraftversionsloader.h"

MinecraftVersionsLoader::MinecraftVersionsLoader(QString j)
{
    Json = j;
    reader.parse(Json, jso);
    Value vers = jso["versions"];
    Value o;
    for(int i = 0; i < vers.size(); i++)
    {
        MinecraftVersion v;
        o = vers[i];
        v.id = o["id"].asString();
        v.time = o["time"].asString();
        v.releaseTime = o["releaseTime"].asString();
        v.mtype = o["type"].asString();
        versions.append(v);
    }
    o = jso["lastest"];
    snapshot = o["snapshot"].asString();
    release = o["release"].asString();
}
