#include <iostream>
#include "CGame.h"

using namespace std;

int main(int argc,char** argv)
{
    CGame game(argc,argv);
    game.init_engine();
    game.init_scene();
    game.init_game();
    game.run();
    return 0;
}
