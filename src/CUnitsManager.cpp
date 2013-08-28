#include "CUnitsManager.h"

CUnitsManager::CUnitsManager()
{
    last_unit_id    = 1;
    last_get_unit   = 0;
}

CUnitsManager::~CUnitsManager()
{
    //dtor
}

void CUnitsManager::addUnit(SUnit unit)
{
    unit.UnitID = request_new_unit_id();
    Units.push_back(unit);
}

void CUnitsManager::remUnit(u32   ID)
{
    u32 size = Units.size();
    for(u32 i = 0 ; i < size ; i++)
    {
        if(Units[i].UnitID == ID)
        {
            Units.erase(Units.begin()+i);
            return;
        }
    }
}

u32 CUnitsManager::getUnitsCount()
{
    return Units.size();
}

SUnit*  CUnitsManager::getUnitsList()
{
    return &Units.front();
}

SUnit*  CUnitsManager::getNextUnit()
{
    if(Units.size() == 0)
        return NULL;

    if(last_get_unit >= Units.size())
        last_get_unit = 0;

    SUnit* unit = &Units[last_get_unit];
    last_get_unit++;

    return unit;
}

void    CUnitsManager::shake_units()
{
        srand(time(NULL));

        u32 size = Units.size();

        for(u32 i = 0; i < size;i++)
        {
            u32 s = rand()%size;
            u32 d = rand()%size;

            SUnit tmpUnit = Units[s];
            Units[s] = Units[d];
            Units[d] = tmpUnit;
        }
}

SUnit* CUnitsManager::get_unit_by_id(u32 id)
{
    u32 size = Units.size();
    for(u32 i = 0 ; i < size ; i++)
    {
        if(Units[i].UnitID == id)
        {
            return &Units[i];
        }
    }
    return NULL;
}

SUnit* CUnitsManager::get_unit_by_pos(s32 x,s32 y)
{
    u32 size = Units.size();
    for(u32 i = 0 ; i < size ; i++)
    {
        if(Units[i].Position.x == x && Units[i].Position.y == y&& Units[i].Health > 0 && Units[i].LifeTime > 0)
        {
            return &Units[i];
        }
    }
    return NULL;
}
void CUnitsManager::update()
{


    u32 size = Units.size();

    for(u32 i = 0 ; i < size ; i++)
    {
        Units[i].LastPosition = Units[i].Position;
        if(Units[i].Health <= 0 || Units[i].LifeTime <= 0)
        {
            std::cout <<  "unitID " << Units[i].UnitID << " from teamID " << (u32)Units[i].TeamID << " removed... i=" << i << std::endl;
            Units.erase(Units.begin()+i);
            i--;
            size--;
        }
    }
}
u32 CUnitsManager::request_new_unit_id()
{
  return last_unit_id++;
}

