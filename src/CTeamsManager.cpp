#include "CTeamsManager.h"

using namespace novaengine;

CTeamsManager::CTeamsManager(INovaEngine* p_engine):
    Engine(p_engine)
{
    last_team_id = 1;
}

CTeamsManager::~CTeamsManager()
{
    //dtor
}

bool CTeamsManager::load_team(const char* team_name)
{
    STeam newTeam;
    //----------------------------------------------------LoadTexture
    char texture_path[256];
    sprintf(texture_path,"teams\\%s\\texture.png",team_name);
    renderer::ITexture* Team_texture = Engine->getTextureManager()->loadTexture(texture_path);

    newTeam.TeamSprite = Engine->getSceneManager()->createSceneSprite(Team_texture);
    newTeam.TeamSprite->UnRegisterNode();
    Team_texture->release();

    //Fix sprite to 32x32 size
    f32 mult_w = 32 / Team_texture->getTextureDimension().width;
    f32 mult_h = 32 / Team_texture->getTextureDimension().height;
    newTeam.TeamSprite->setScale(core::vector3df(mult_w,mult_h,0));
    //----------------------------------------------------LoadScript
    char script_path[256];
    sprintf(script_path,"teams\\%s\\script.lua",team_name);
    newTeam.TeamScript = script_path;

    //----------------------------------------------------Assign ID
    newTeam.TeamID = request_new_unit_id();

    //----------------------------------------------------Setup units
    SUnit sample_unit;
    sample_unit.Health  = 100;
    sample_unit.Arrmor  = 10;
    sample_unit.Attack  = 30;
    sample_unit.LifeTime= 999999999;
    sample_unit.TeamID  = newTeam.TeamID;
    newTeam.sample_unit = sample_unit;
    //----------------------------------------------------Finish
    Teams.push_back(newTeam);

    //---------------------------------------------------spam
    std::cout << "Registred new team with id:" << newTeam.TeamID << std::endl;
}
u32     CTeamsManager::getTeamsCount()
{
    return Teams.size();
}
STeam*  CTeamsManager::getTeamsList()
{
    return &Teams.front();
}
STeam* CTeamsManager::get_team_by_id(s32 ID)
{
    u32 size = Teams.size();
    for(u32 i = 0 ; i < size ; i++)
    {
        if(Teams[i].TeamID == ID)
        {
            return &Teams[i];
        }
    }
    return NULL;
}
u32 CTeamsManager::request_new_unit_id()
{
    return last_team_id++;
}
