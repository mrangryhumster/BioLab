#include "CGame.h"

#include "NovaEngine.h"
using namespace novaengine;

CGame::CGame(int p_argc,char** p_argv)
{
    argc = p_argc;
    argv = p_argv;
}

CGame::~CGame()
{
    //dtor
}

bool CGame::init_engine()
{
    SEngineParams params;
    params.WindowSize = core::dimension2du(800,600);
    params.Bits = 16;
    params.FullScreen = false;
    params.VSync = false;
    Engine = createEngineEx(params);

    Engine->getWindow()->setWindowCaption("BioLab");
    //Configuration texture loading
    /* PIXELS 0.0 */
    renderer::STextureGenParams texgen;
    texgen.Texture_mag_filter = renderer::ETBF_NEAREST;
    texgen.Texture_min_filter = renderer::ETBF_NEAREST;
    Engine->getRenderer()->setTextureGenParams(texgen);
    return true;
}

bool CGame::init_scene()
{
    scene::ISceneCamera* cam = Engine->getSceneManager()->createSceneCamera();
    cam->RegisterNode();
    cam->setActive();
    return true;
}
bool CGame::init_game()
{

    Session = new CGameSession(Engine,2500,false);

    if(Session->init() == false)
        return false;

    for(int i = 1; i < argc;i++)
    {
        Session->add_player(argv[i]);
    }
    return true;
}
bool CGame::run()
{

    Session->start();
    f32 fps = 0;
    fps_list.push_back(0);
    while(Engine->run())
    {
        //Update Session
        if(Session->update(Engine->getRenderer()->getMPF()) == false)
        {
            std::cout << "Script engine error..." << std::endl;
            return false;
        }

        //Render
        Engine->getRenderer()->begin_frame(true,core::color4f(0,0,0,0));

        Engine->getSceneManager()->renderScene();
        Session->render_units();

        Engine->getRenderer()->end_frame();

        //FPS
        if(fps != Engine->getRenderer()->getFPS())
        {
            fps = Engine->getRenderer()->getFPS();
            char buf[64];
            sprintf(buf,"BioLab fps:%.0f a:%f primitives:%u vertex:%u",fps,Engine->getRenderer()->getMPF(),Engine->getRenderer()->getPPD(),Engine->getRenderer()->getVPD());
            Engine->getWindow()->setWindowCaption(buf);

            fps_list.push_back(Engine->getRenderer()->getMPF()*25);
        }


    }
    Engine->registerEventHandler(NULL);

    Engine->release();
    return true;
}

void CGame::draw_fps()
{
    s32 x_stride = - (Engine->getWindow()->getRealWindowSize().width / 2);
    s32 y_stride = Engine->getWindow()->getRealWindowSize().height - Engine->getWindow()->getRealWindowSize().height/2 - 200;

    Engine->getSceneManager()->useProjectionMatrix(scene::EPM_ORTHO);
    Engine->getSceneManager()->useViewMatrix(scene::EVM_2D);

    Engine->getRenderer()->setTransform(Engine->getSceneManager()->getActiveScene()->getActiveCamera()->getAbsoluteTransformation(),renderer::EMT_MODEL);
    renderer::IMaterial* material = new renderer::CMaterial();

    material->setMaterialColor(core::color4u(0,0,64,128));
    Engine->getRenderer()->setActiveMaterial(material);

    Engine->getRenderer()->draw2dRectangle(core::vector2df(x_stride,y_stride),core::vector2df(x_stride+200,y_stride+200));

    f32 fps_mid = 0;

    if(fps_list.size() > 21)
        fps_list.erase(fps_list.begin());

    if(fps_list.size() <= 20)
    {
        for(u32 i = 0; i < 20; i++)
            fps_list.push_back(0);
    }

    s32 list_size = fps_list.size();

    for(s32 i = 1; i < list_size; i++)
    {
        if(i >= 21)
            break;

        if(fps_list[i]!=0)
        {
            material->setMaterialColor(core::color4u(255,0,0,255));
            Engine->getRenderer()->setActiveMaterial(material);
            Engine->getRenderer()->draw2dLine(core::vector2df((x_stride - 10) + (i*10),fps_list[i]+y_stride),core::vector2df((x_stride - 10) + (i*10)+10,fps_list[i+1]+y_stride));

            material->setMaterialColor(core::color4u(255,255,0,255));
            Engine->getRenderer()->setActiveMaterial(material);
            Engine->getRenderer()->draw2dPoint(core::vector2df(x_stride-10 + (i*10),fps_list[i]+y_stride),4);
            fps_mid += fps_list[i];

        }
    }

    fps_mid /= 20;

    material->setMaterialColor(core::color4u(0,255,0,128));
    Engine->getRenderer()->setActiveMaterial(material);
    Engine->getRenderer()->draw2dLine(core::vector2df(x_stride,y_stride + fps_mid),core::vector2df(x_stride+200,y_stride + fps_mid));




}

