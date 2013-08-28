#include "CGameEffects.h"

using namespace novaengine;

CGameEffects::CGameEffects(INovaEngine* p_engine):
    Engine(p_engine)
{
    srand(time(NULL));
}

CGameEffects::~CGameEffects()
{
    //dtor
}

bool CGameEffects::init()
{
    EffectsTileset = Engine->getTextureManager()->loadTexture("res\\effect_test.png");
}

void CGameEffects::create_effect(u32 effect_image,core::vector2ds pos)
{
    SEffect newEffect;
    newEffect.EffectSprite = Engine->getSceneManager()->createSceneSprite(EffectsTileset);
    newEffect.EffectSprite->setScale(core::vector3df(2,2,1));
    newEffect.EffectSprite->setPosition(core::vector3df(pos.x,pos.y,0));
    newEffect.EffectSprite->UnRegisterNode();
    //----------------------------------------------------------------------------------------
    if(effect_image == GE_SPAWN)
    {
    newEffect.EffectSprite->setTextureSourceRect(core::rectf(0.75f,0,1,0.25f));

    newEffect.LifeTime = 3000;
    newEffect.fadeout_modif = 0.080f;

    newEffect.pos_x_modif = 0;
    newEffect.pos_y_modif = 0;

    newEffect.scale_x_modif = 0.005f;
    newEffect.scale_y_modif = 0.005f;

    newEffect.fadeout_accum = 0;
    }
    //----------------------------------------------------------------------------------------
    if(effect_image == GE_ATTACK)
    {
    int e = rand() % 3;
    if(e == 0)
        newEffect.EffectSprite->setTextureSourceRect(core::rectf(0,0,0.25f,0.25f));
    if(e == 1)
        newEffect.EffectSprite->setTextureSourceRect(core::rectf(0,0.25f,0.25f,0.50f));
    if(e == 2)
        newEffect.EffectSprite->setTextureSourceRect(core::rectf(0,0.50f,0.25f,0.75f));

    newEffect.LifeTime = 1500;
    newEffect.fadeout_modif = 0.1f;

    newEffect.pos_x_modif = 0;
    newEffect.pos_y_modif = 0;

    newEffect.scale_x_modif = 0.001f;
    newEffect.scale_y_modif = 0.001f;

    newEffect.fadeout_accum = 0;
    }
    //----------------------------------------------------------------------------------------
    if(effect_image == GE_BLOCK)
    {
    newEffect.EffectSprite->setTextureSourceRect(core::rectf(0,0.75f,0.25f,1));

    newEffect.LifeTime = 1500;
    newEffect.fadeout_modif = 0.1f;

    newEffect.pos_x_modif = 0;
    newEffect.pos_y_modif = 0;

    newEffect.scale_x_modif = 0.001f;
    newEffect.scale_y_modif = 0.001f;

    newEffect.fadeout_accum = 0;
    }
    //----------------------------------------------------------------------------------------
    if(effect_image == GE_DEATH)
    {
    newEffect.EffectSprite->setTextureSourceRect(core::rectf(0.25f,0,0.50f,0.25f));

    newEffect.LifeTime = 5000;
    newEffect.fadeout_modif = 0.051f;

    newEffect.pos_x_modif = 0;
    newEffect.pos_y_modif = 0.01f;

    newEffect.scale_x_modif = 0;
    newEffect.scale_y_modif = 0;

    newEffect.fadeout_accum = 0;

    }
    //----------------------------------------------------------------------------------------
    if(effect_image == GE_HEAL_SMALL)
    {
    newEffect.EffectSprite->setTextureSourceRect(core::rectf(0.50f,0,0.75f,0.25f));

    newEffect.LifeTime = 3000;
    newEffect.fadeout_modif = 0.085f;

    newEffect.pos_x_modif = 0;
    newEffect.pos_y_modif = 0.02f;

    newEffect.scale_x_modif = 0;
    newEffect.scale_y_modif = 0;

    newEffect.fadeout_accum = 0;
    }
    //----------------------------------------------------------------------------------------
    Effects.push_back(newEffect);
}

void CGameEffects::update(f32 ms)
{
    u32 size = Effects.size();
    for(u32 i = 0; i < size; i ++)
    {
        if(Effects[i].LifeTime <= 0)
        {
            Effects[i].EffectSprite->release();
            Effects.erase(Effects.begin() + i);
            i--;
            size--;
        }
        else
        {
        Effects[i].LifeTime-=ms;

        core::color4u   Color           = Effects[i].EffectSprite->getMaterial()->getMaterialColor();
        core::vector3df Scale           = Effects[i].EffectSprite->getScale();
        core::vector3df Position        = Effects[i].EffectSprite->getPosition();

        Effects[i].fadeout_accum += Effects[i].fadeout_modif * ms;
        Color.Alpha  = 255 - Effects[i].fadeout_accum;
        Scale.x     += Effects[i].scale_x_modif * ms;
        Scale.y     += Effects[i].scale_y_modif * ms;
        Position.x  += Effects[i].pos_x_modif   * ms;
        Position.y  += Effects[i].pos_y_modif   * ms;

        Effects[i].EffectSprite->getMaterial()->setMaterialColor(Color);
        Effects[i].EffectSprite->setScale(Scale);
        Effects[i].EffectSprite->setPosition(Position);
        }
    }
}

void CGameEffects::draw_effects()
{
    u32 size = Effects.size();
    for(u32 i = 0; i < size; i ++)
    {
        Effects[i].EffectSprite->render();
    }
}
