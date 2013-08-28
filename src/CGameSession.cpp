#include "CGameSession.h"
using namespace novaengine;


CGameSession::CGameSession(INovaEngine* p_engine,u32 p_simspeed,bool p_execute_all):
    Engine(p_engine),
    inited(false)
{
    Accum = 0;
    SimSpeed = p_simspeed;
    ExecuteAll = p_execute_all;
}

CGameSession::~CGameSession()
{
    if(inited)
    {
        delete GameMap;

    }
}

bool CGameSession::init()
{
    Engine->registerEventHandler(this);
    //Load map
    GameMap     = new CGameMap(Engine,core::dimension2du(16,16));

    UnitsManager = new CUnitsManager();
    TeamsManager = new CTeamsManager(Engine);

    GameEffects = new CGameEffects(Engine);

    //Load script engine
    ScriptEngine = new CGameScriptEngine(GameMap,UnitsManager,TeamsManager,GameEffects);

    GameEffects->init();

    if(ScriptEngine->init() == false)
    {
        std::cout << "Cannot init script engine..." << std::endl;
        return false;
    }

    inited = true;
    return true;
}
bool  CGameSession::add_player(const char* player_name)
{
    return TeamsManager->load_team(player_name);
}
bool CGameSession::start(u32 UnitsCount)
{
    //------------------------------------------------
    u32 size = TeamsManager->getTeamsCount();
    STeam* TeamsList = TeamsManager->getTeamsList();
    for(u32 i = 0; i < size; i++)
    {
        SUnit unit = TeamsList[i].sample_unit;
        unit.Position = core::vector2ds(rand()%16,rand()%16);

        core::vector2ds SpawnGlobPos;
        SpawnGlobPos.x = -(((int)GameMap->get_map_size().width *16)) + unit.Position.x * 32 + 16;
        SpawnGlobPos.y = -(((int)GameMap->get_map_size().height*16)) + unit.Position.y * 32 + 16;
        GameEffects->create_effect(GE_SPAWN,SpawnGlobPos);

        for(u32 i = 0; i < UnitsCount; i++)
            UnitsManager->addUnit(unit);

    }
    UnitsManager->shake_units();
    //---------------------------------------------------
}
bool CGameSession::OnEvent(SEvent event)
{
    if(event.event_type == EET_MOUSE)
    {
        if(event.mouse.event_type == EETM_BUTTON)
        {
            camera_moving = event.mouse.key_state;

            last_mouse_pos.x = event.mouse.x;
            last_mouse_pos.y = event.mouse.y;

        }
        else
        {
            if(camera_moving)
            {
                curr_mouse_pos.x = event.mouse.x;
                curr_mouse_pos.y = event.mouse.y;

                camera_pos.x += last_mouse_pos.x - curr_mouse_pos.x;
                camera_pos.y += curr_mouse_pos.y - last_mouse_pos.y;

                last_mouse_pos = curr_mouse_pos;

                Engine->getSceneManager()->getActiveScene()->getActiveCamera()->setPosition(camera_pos);
            }
        }
    }
}
bool CGameSession::update(f32 ms)
{
    Accum+=ms;

    GameEffects->update(ms);

    if(ExecuteAll != true)
    {
    f32 cell_count = UnitsManager->getUnitsCount();
    realSimSpeed = SimSpeed / cell_count;
    }
    else
    {
        realSimSpeed = SimSpeed;
    }

    while(Accum >= realSimSpeed)
    {
        Accum-=realSimSpeed;

        UnitsManager->update();

        if(ExecuteAll)
            return ScriptEngine->execute();
        else
            return ScriptEngine->execute_next();
    }

    return true;
}

bool CGameSession::render_units()
{
    u32 size            = UnitsManager->getUnitsCount();
    SUnit* UnitsList    = UnitsManager->getUnitsList();

    for(u32 i = 0 ; i < size ; i++)
    {
        core::vector3df Position;
        Position.x = -(((int)GameMap->get_map_size().width *16)) + UnitsList[i].Position.x * 32 + 16;
        Position.y = -(((int)GameMap->get_map_size().height*16)) + UnitsList[i].Position.y * 32 + 16;

        core::vector3df LastPosition;

        LastPosition.x = -(((int)GameMap->get_map_size().width *16)) + UnitsList[i].LastPosition.x * 32 + 16;
        LastPosition.y = -(((int)GameMap->get_map_size().height*16)) + UnitsList[i].LastPosition.y * 32 + 16;

        core::vector2df SurfPos;
        SurfPos.x = ((f32)(Position.x - LastPosition.x) / realSimSpeed)*Accum;
        SurfPos.y = ((f32)(Position.y - LastPosition.y) / realSimSpeed)*Accum;

        LastPosition.x += SurfPos.x;
        LastPosition.y += SurfPos.y;

        STeam* team = TeamsManager->get_team_by_id(UnitsList[i].TeamID);
        if(team != NULL)
        {
            scene::ISceneSprite* sprite = team->TeamSprite;
            sprite->setPosition(LastPosition);
            sprite->getMaterial()->setMaterialColor(core::color4u(255,255,255,255));
            sprite->render();
        }
    }

    GameEffects->draw_effects();
    return true;
}
