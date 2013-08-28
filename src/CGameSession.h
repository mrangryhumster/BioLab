#ifndef CGAMESESSION_H
#define CGAMESESSION_H

#include <iostream>
#include <vector>

#include <stdlib.h>
#include <time.h>

#include "NovaEngine.h"
using namespace novaengine;

#include "CGameScriptEngine.h"
#include "CGameMap.h"
#include "CUnitsManager.h"
#include "CTeamsManager.h"

#include "CGameEffects.h"



class CGameSession : public IEventHandler
{
public:
    CGameSession(INovaEngine* p_engine,u32 p_simspeed,bool p_execute_all);
    virtual ~CGameSession();

    bool init();
    bool add_player(const char* player_name);
    bool start(u32 UnitsCount);

    bool OnEvent(SEvent event);
    bool update(f32 ms);
    bool render_units();

    SUnit* get_unit(s32 x,s32 y);

protected:
private:

    INovaEngine*    Engine;

    //--------------------------
    bool            camera_moving;
    f32             camera_zoom;
    core::vector3df camera_pos;
    core::vector2ds last_mouse_pos;
    core::vector2ds curr_mouse_pos;
    //--------------------------

    bool inited;
    CGameMap*          GameMap;
    CUnitsManager*      UnitsManager;
    CTeamsManager*      TeamsManager;
    CGameEffects*       GameEffects;
    CGameScriptEngine* ScriptEngine;


    //Logic data
    bool ExecuteAll;

    f32 SimSpeed;
    f32 realSimSpeed;

    f32 Accum;

};

#endif // CGAMESESSION_H
