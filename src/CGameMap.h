#ifndef CGAMEMAP_H
#define CGAMEMAP_H

#include "NovaEngine.h"
using namespace novaengine;

#include <stdlib.h>

enum G_MAP_ENVIROMENT_ELEMENT
{
    GMEE_NONE  = 0x0,
    GMEE_ROAD  = 0x1,
    GMEE_BLOCK = 0x2,
    GMEE_EAT   = 0x3,
};

class CGameMap
{
public:
    CGameMap(INovaEngine* p_engine,core::dimension2du p_map_size);
    virtual ~CGameMap();


    core::dimension2du get_map_size()
    {
        return map_size;
    };

    u32 get_block_info(core::vector2ds coord);
    u32 set_block_info(core::vector2ds coord,u32 info);
    bool canPass(core::vector2ds coord);

    void update();

protected:
private:

    void rebuild_map();


    INovaEngine* Engine;
    scene::ISceneTileMap* Lay_1;
    scene::ISceneTileMap* Lay_2;

    core::dimension2du map_size;
    u32* map_info;

    bool need_rebuild_lay;

};

#endif // CGAMEMAP_H
