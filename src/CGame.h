#ifndef CGAME_H
#define CGAME_H

#include "NovaEngine.h"
using namespace novaengine;

#include <vector>

#include "CGameSession.h"

class CGame
{
    public:
        CGame(int argc,char** argv);
        virtual ~CGame();

        bool init_engine();
        bool init_scene();
        bool init_game();
        bool run();

        void draw_fps();
    protected:
    private:
    int argc;
    char** argv;
    INovaEngine* Engine;
    CGameSession* Session;
    std::vector<f32> fps_list;
    u32 UnitsCount;
};

#endif // CGAME_H
