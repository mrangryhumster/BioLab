#ifndef CGAMESCRIPTENGINE_H
#define CGAMESCRIPTENGINE_H

#include "SUnit.h"
#include "CGameMap.h"
#include "CUnitsManager.h"
#include "CTeamsManager.h"
#include "CGameEffects.h"

#include <stdlib.h>
#include <time.h>

extern "C"
{
  #include <lua.h>
  #include <lualib.h>
  #include <lauxlib.h>
}

class CGameScriptEngine
{
    public:
        CGameScriptEngine(CGameMap* p_game_map,CUnitsManager* p_unit_control,CTeamsManager* p_team_manager,CGameEffects* p_effects);
        virtual ~CGameScriptEngine();

        bool init();
        bool execute();
        bool execute_next();

        CGameMap*        getGameMap();
        CUnitsManager*   getUnitsManager();
        CGameEffects*    getEffectManager();
        SUnit*           getCurrentUnit();

        //!--------------------------------
        bool isCollision(u32 x,u32 y);

    protected:
    private:
        CGameMap*       GameMap;
        CUnitsManager*   UnitsManager;
        CTeamsManager*   TeamManager;
        CGameEffects*    Effects;
        lua_State*      Lua;

        u32 current_execute;

        SUnit* CurrentUnit;
};

#endif // CGAMESCRIPTENGINE_H
