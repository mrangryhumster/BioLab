#ifndef STEAM_H
#define STEAM_H

#include "NovaEngine.h"
using namespace novaengine;
#include <string>

#include "SUnit.h"

struct STeam
{
    scene::ISceneSprite* TeamSprite;
    std::string TeamScript;

    SUnit sample_unit;

    s32 TeamID;
};

#endif // STEAM_H
