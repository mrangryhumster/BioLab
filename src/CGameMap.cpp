#include "CGameMap.h"

using namespace novaengine;

CGameMap::CGameMap(INovaEngine* p_engine,core::dimension2du p_map_size):
    Engine(p_engine),
    map_size(p_map_size)
{
    renderer::ITileMap* TileMap_1 = Engine->getGeometryManager()->buildTileMap(core::dimension2df(32,32),core::dimension2du(map_size.width,map_size.height),NULL);
    Lay_1 = Engine->getSceneManager()->createSceneTileMap(TileMap_1);
    Lay_1->RegisterNode();

    renderer::ITileMap* TileMap_2 = Engine->getGeometryManager()->buildTileMap(core::dimension2df(32,32),core::dimension2du(map_size.width,map_size.height),NULL);
    Lay_2 = Engine->getSceneManager()->createSceneTileMap(TileMap_2);
    Lay_2->RegisterNode();

    //Lay_1->getMaterial()->setMaterialWireframe(true);
    //Lay_2->getMaterial()->setMaterialWireframe(true);

    renderer::STile tile1,tile2;
    tile1.Tile_Color = core::color4u(128,128,128,255);
    tile2.Tile_Color = core::color4u(0,0,0,0);

    TileMap_1->setAllTiles(tile1);
    TileMap_2->setAllTiles(tile2);

    map_info = new u32[map_size.width*map_size.height];

    for(u32 i = 0; i < map_size.width*map_size.height;i++)
        map_info[i] = GMEE_ROAD;

    TileMap_1->release();
    TileMap_2->release();
    Lay_1->release();
    Lay_2->release();
}

CGameMap::~CGameMap()
{
    delete map_info;
}

u32 CGameMap::get_block_info(core::vector2ds coord)
{
    u32 index = coord.x + (coord.y * map_size.width);
    return map_info[index];
}

u32 CGameMap::set_block_info(core::vector2ds coord,u32 info)
{
    u32 index = coord.x + (coord.y * map_size.width);
    map_info[index] = info;
    need_rebuild_lay = true;
    return 0;
}
bool CGameMap::canPass(core::vector2ds coord)
{
   if(coord.x < 0 || coord.y < 0 || coord.x >= map_size.width || coord.y >= map_size.height)
        return false;

    u32 index = coord.x + (coord.y * map_size.width);
    return map_info[index] == GMEE_ROAD;
}
void CGameMap::update()
{
    if(need_rebuild_lay)
    {
        rebuild_map();
        need_rebuild_lay = false;
    }
}
void CGameMap::rebuild_map()
{

}
