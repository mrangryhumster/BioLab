#ifndef SUNIT_H
#define SUNIT_H

#include "NovaEngine.h"
using namespace novaengine;

struct SUnit
{
    core::vector2ds LastPosition;
    core::vector2ds Position;

    s32             Attack;
    s32             Health;
    s32             Arrmor;

    s32             LifeTime;
    s32             Actions;

    //Private
    s32             UnitID; //! setted by UnitsManager
    s32             TeamID; //! setted by TeamControl
};

#endif // SUNIT_H
