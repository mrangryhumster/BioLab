#ifndef CGAMEEFFECTS_H
#define CGAMEEFFECTS_H

#include "NovaEngine.h"
using namespace novaengine;

#include <vector>

#include <stdlib.h>
#include <time.h>

#include "SEffect.h"

enum G_EFFECTS
{
    GE_SPAWN        = 0x1,
    GE_ATTACK       = 0x2,
    GE_BLOCK        = 0x3,
    GE_DEATH        = 0x4,
    GE_HEAL_SMALL   = 0x5,
    GE_HEAL_BIG     = 0x6,

};
class CGameEffects
{
    public:
        CGameEffects(INovaEngine* p_engine);
        virtual ~CGameEffects();

        bool init();

        void create_effect(u32 effect_image,core::vector2ds pos);

        void update(f32 ms);
        void draw_effects();

    protected:
    private:
        INovaEngine* Engine;

        renderer::ITexture* EffectsTileset;

        std::vector<SEffect> Effects;

};

#endif // CGAMEEFFECTS_H
