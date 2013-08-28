#ifndef SEFFECT_H
#define SEFFECT_H

#include "NovaEngine.h"
using namespace novaengine;

struct SEffect
{
    scene::ISceneSprite* EffectSprite;

    f32 LifeTime;

    f32 fadeout_modif;
    f32 scale_x_modif;
    f32 scale_y_modif;
    f32 pos_x_modif;
    f32 pos_y_modif;

    //private
    f32 fadeout_accum;
};

#endif // SEFFECT_H
