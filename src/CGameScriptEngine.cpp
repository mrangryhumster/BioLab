#include "CGameScriptEngine.h"



CGameScriptEngine* lb_ScriptEngine = NULL;
//!***************************************************************************
//! Utils

void fix_pos(s32 PosSize,s32& x,s32& y)
{

}
void pos_to_1(s32& x,s32& y)
{
    if(x != 0)
        x = x / abs(x);
    if(y != 0)
        y = y / abs(y);
}

void pos_to_2(s32& x,s32& y)
{
    if(x > 2)
        x =  2;
    if(x < -2)
        x = -2;
    if(y > 2)
        y =  2;
    if(y < -2)
        y = -2;
}

void absolute_pos_to_1(s32& x,s32& y)
{
    pos_to_1(x,y);
    x+= lb_ScriptEngine->getCurrentUnit()->Position.x;
    y+= lb_ScriptEngine->getCurrentUnit()->Position.y;
}

void absolute_pos_to_2(s32& x,s32& y)
{
    pos_to_2(x,y);
    x+= lb_ScriptEngine->getCurrentUnit()->Position.x;
    y+= lb_ScriptEngine->getCurrentUnit()->Position.y;
}

void relative_to_global_pos(s32& x,s32& y,SUnit* unit)
{
    x = -(((int)lb_ScriptEngine->getGameMap()->get_map_size().width *16)) + unit->Position.x * 32 + 16;
    y = -(((int)lb_ScriptEngine->getGameMap()->get_map_size().height*16)) + unit->Position.y * 32 + 16;
}
//!***************************************************************************
//!************************LUA*BINDING*FUNCTIONS******************************
//!***************************************************************************


int lua_getRandom(lua_State* Lua)
{
    int start = 0;
    int end   = 10;

    int argc = lua_gettop(Lua);

    if(argc == 1 && lua_isnumber(Lua,1))
    {
        end = lua_tonumber(Lua,1);
    }
    if(argc == 2 && lua_isnumber(Lua,1) && lua_isnumber(Lua,2))
    {
        start   = lua_tonumber(Lua,1);
        end     = lua_tonumber(Lua,2);
    }
    int rnd = rand() % (end - start);
    rnd += start;

    lua_pushnumber(Lua,rnd);
    return 1;
}

int lua_getFullStatus(lua_State* Lua)
{
    SUnit* unit = lb_ScriptEngine->getCurrentUnit();
    lua_pushnumber(Lua,unit->UnitID);
    lua_pushnumber(Lua,unit->Position.x);
    lua_pushnumber(Lua,unit->Position.y);
    lua_pushnumber(Lua,unit->Health);
    lua_pushnumber(Lua,unit->Actions);
    lua_pushnumber(Lua,unit->TeamID);
    return 6;
}

int lua_isPassably(lua_State* Lua)
{
    int argc = lua_gettop(Lua);

    if(argc < 2 || (!lua_isnumber(Lua,1) || !lua_isnumber(Lua,2)))
    {
        lua_pushboolean(Lua,0);
        return 1;
    }

    int     x       = lua_tonumber(Lua,1);
    int     y       = lua_tonumber(Lua,2);

    absolute_pos_to_2(x,y);

    if(lb_ScriptEngine->isCollision(x,y))
        lua_pushboolean(Lua,1);
    else
        lua_pushboolean(Lua,0);

    return 1;
}

int lua_inspect(lua_State* Lua)
{
    int argc = lua_gettop(Lua);
    if(argc < 2 || (!lua_isnumber(Lua,1) || !lua_isnumber(Lua,2)))
        return 0;


    int     x           = lua_tonumber(Lua,1);
    int     y           = lua_tonumber(Lua,2);

    absolute_pos_to_2(x,y);

    SUnit*  unit        = lb_ScriptEngine->getUnitsManager()->get_unit_by_pos(x,y);

    if(unit == NULL)
        return 0;

    lua_pushnumber(Lua,unit->UnitID);
    lua_pushnumber(Lua,unit->Health);
    lua_pushnumber(Lua,unit->TeamID);

    return 3;
}

int lua_action(lua_State* Lua)
{
    if(lb_ScriptEngine->getCurrentUnit()->Actions == 0)
        return 1;


    int argc = lua_gettop(Lua);

    if(argc < 3 || ( !lua_isstring(Lua,1) || !lua_isnumber(Lua,2) || !lua_isnumber(Lua,3)))
        return 1;


    SUnit* unit             = lb_ScriptEngine->getCurrentUnit();
    const char* action      = lua_tostring(Lua,1);
    int         x           = lua_tonumber(Lua,2);
    int         y           = lua_tonumber(Lua,3);

    if(strcmp(action,"move") == 0)
    {
        absolute_pos_to_1(x,y);
        if(lb_ScriptEngine->isCollision(x,y))
        {
            lua_pushboolean(Lua,1);

            unit->LastPosition = unit->Position;

            unit->Position.x = x;
            unit->Position.y = y;

            unit->Actions--;
        }
        else
        {
            lua_pushboolean(Lua,0);
        }
    }

    if(strcmp(action,"attack") == 0)
    {
        absolute_pos_to_1(x,y);
        SUnit* target = lb_ScriptEngine->getUnitsManager()->get_unit_by_pos(x,y);
        if(target != NULL && target->TeamID != unit->TeamID)
        {
            s32 Damage = unit->Attack - target->Arrmor;
            s32 ArrmorPenetration = rand() % unit->Attack;

            if(Damage <= 0)
                Damage = 1;


            target->Health-=Damage;
            target->Arrmor-=ArrmorPenetration;

            std::cout << unit->UnitID << "(" << unit->TeamID << ") id hit "<< target->UnitID << "(" << target->TeamID << ")  id on " << Damage << " Damage trgt:hp = " << target->Health << "\\100 arrmor penetration = " << ArrmorPenetration << "(" << target->Arrmor << ")" << std::endl;


            //--------------------------------------------------------------------------------------------
            s32 ex,ey;
            relative_to_global_pos(ex,ey,target);
            if(target->Health > 0)
            {
                lb_ScriptEngine->getEffectManager()->create_effect(GE_ATTACK,novaengine::core::vector2ds(ex,ey));
            }
            else
            {
                lb_ScriptEngine->getEffectManager()->create_effect(GE_DEATH,novaengine::core::vector2ds(ex,ey));
            }

            if(target->Arrmor > 0)
            {
                lb_ScriptEngine->getEffectManager()->create_effect(GE_BLOCK,novaengine::core::vector2ds(ex,ey));
            }
            //--------------------------------------------------------------------------------------------

            lua_pushboolean(Lua,1);

            unit->Actions--;
        }
        else
        {
            lua_pushboolean(Lua,0);
        }
    }

    return 1;
}

int lua_rest(lua_State* Lua)
{
    if(lb_ScriptEngine->getCurrentUnit()->Actions == 0)
        return 0;

    SUnit* unit             = lb_ScriptEngine->getCurrentUnit();

    u32 Heal = (100 - unit->Health)/25;
    unit->Health+=Heal;



    if(Heal != 0)
    {
        std::cout << unit->UnitID << "(" << unit->TeamID << ")  id healed on " << Heal << " points hp now " << unit->Health << "\\100" << std::endl;
        s32 x,y;
        relative_to_global_pos(x,y,unit);
        lb_ScriptEngine->getEffectManager()->create_effect(GE_HEAL_SMALL,novaengine::core::vector2ds(x,y));
    }
    unit->Actions = 0;
    return 0;
}
//!***************************************************************************
//!***************************************************************************
//!***************************************************************************

CGameScriptEngine::CGameScriptEngine(CGameMap* p_game_map,CUnitsManager* p_unit_control,CTeamsManager* p_team_manager,CGameEffects* p_effects):
    GameMap(p_game_map),
    UnitsManager(p_unit_control),
    TeamManager(p_team_manager),
    Effects(p_effects),
    Lua(NULL)
{
    lb_ScriptEngine = this;
    current_execute = 0;

    srand(time(NULL));

    //freopen ("log.txt","wt",stdout);
}

CGameScriptEngine::~CGameScriptEngine()
{
    if(Lua)
        lua_close(Lua);
}

bool CGameScriptEngine::init()
{
    Lua = luaL_newstate();
    if(Lua == NULL)
    {
        std::cout << "Cannot init lua engine..." << std::endl;
        return false;
    }
    luaL_openlibs(Lua);

    //register lua api
    lua_register(Lua,"getRandom",lua_getRandom);
    lua_register(Lua,"getFullStatus",lua_getFullStatus);
    lua_register(Lua,"inspect",lua_inspect);
    lua_register(Lua,"isPassably",lua_isPassably);
    lua_register(Lua,"action",lua_action);
    lua_register(Lua,"rest",lua_rest);

    return true;
}
bool CGameScriptEngine::execute()
{
    u32 size  = UnitsManager->getUnitsCount();
    for(u32 i = 0 ; i < size ; i++)
    {
        if(execute_next() == false)
            return false;
    }
    return true;
}

bool CGameScriptEngine::execute_next()
{
    int error = 0;
    CurrentUnit = UnitsManager->getNextUnit();

    if(CurrentUnit == NULL)
    {
        std::cout << "Unit lost..." << std::endl;
        return false;
    }
    CurrentUnit->Actions += 1;

    if(CurrentUnit->Actions > 1)
        CurrentUnit->Actions = 1;

    CurrentUnit->LifeTime--;

    if(CurrentUnit->Health > 0 && CurrentUnit->LifeTime > 0 && CurrentUnit->Actions > 0)
    {
        STeam* team = TeamManager->get_team_by_id(CurrentUnit->TeamID);
        if(team != NULL)
            error = luaL_dofile(Lua,team->TeamScript.c_str());
        else
        {
            std::cout << "Team lost..." << std::endl;
            return false;
        }
    }

    if(error)
    {
        std::cout << "Lua err : " << lua_tostring(Lua,-1) << std::endl;
        return false;
    }

    return true;
}

CGameMap* CGameScriptEngine::getGameMap()
{
    return GameMap;
}
CUnitsManager* CGameScriptEngine::getUnitsManager()
{
    return UnitsManager;
}
CGameEffects*   CGameScriptEngine::getEffectManager()
{
    return Effects;
}

SUnit* CGameScriptEngine::getCurrentUnit()
{
    return CurrentUnit;
}

//------------------------------------------------------------------------------
bool CGameScriptEngine::isCollision(u32 x,u32 y)
{
    if(GameMap->canPass(novaengine::core::vector2ds(x,y)) && UnitsManager->get_unit_by_pos(x,y)==NULL)
        return true;
    else
        return false;
}
