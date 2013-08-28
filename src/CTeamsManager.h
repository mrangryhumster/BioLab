#ifndef CTEAMSMANAGER_H
#define CTEAMSMANAGER_H

#include "NovaEngine.h"
using namespace novaengine;

#include "STeam.h"

#include <stdlib.h>
#include <vector>

class CTeamsManager
{
public:
    CTeamsManager(INovaEngine* p_engine);
    virtual ~CTeamsManager();

    bool load_team(const char* team_name);

    u32     getTeamsCount();
    STeam*  getTeamsList();

    STeam* get_team_by_id(s32 ID);

    u32 request_new_unit_id();
protected:
private:
    INovaEngine* Engine;
    u32 last_team_id;
    std::vector<STeam> Teams;

};

#endif // CTEAMSMANAGER_H
