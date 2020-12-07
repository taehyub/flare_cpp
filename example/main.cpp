#include "flare_thorvg/thorvg_actor_artboard.hpp"
#include "flare_thorvg/thorvg_actor.hpp"
#include "flare/animation/actor_animation.hpp"
#include "flare/exceptions/overflow_exception.hpp"
#include "flare/exceptions/unsupported_version_exception.hpp"
#include "flare_math/aabb.hpp"

#include <iostream>
#include <thread>
#include <Evas_GL.h>
#include <Elementary.h>
#include <thorvg.h>

#define WIDTH 720
#define HEIGHT 1280

#define AssetsPath "/home/flare/"

char AssetPathBuf[100];

using namespace std;

Eo *gView;
flare::TvgActorArtboard* artboard;
flare::ActorAnimation* animation;
flare::ActorAnimation* animation2;
flare::TvgActor* actor;
Ecore_Animator *animator;

double animationTime = 0.0;
double elapsed = 0.0;
double lastTime = 0.0;
double currentTime = 0.0;
bool gAnim = true;
int gViewX = 0;
int gViewY = 0;
static uint32_t buffer[WIDTH * HEIGHT];

void tvgDrawCmds(tvg::Canvas* canvas)
{
    if (!canvas) return;
}

Eina_Bool animGlCb(void* data)
{
    if (!gAnim) return ECORE_CALLBACK_CANCEL;
    tvg::Canvas *canvas = (tvg::Canvas*)data;

    currentTime = ecore_time_get();
    double elapsed = currentTime - lastTime;
    lastTime = currentTime;
    animationTime += elapsed;
    animation->apply(std::fmod(animationTime, animation->duration()), artboard, 1.0);
    if (animation2)
      animation2->apply(std::fmod(animationTime, animation2->duration()), artboard, 1.0);
    artboard->advance(elapsed);
    artboard->draw(canvas);

    if (animationTime > animation->duration()) {
      animator = NULL;
      return ECORE_CALLBACK_CANCEL;
    }

    auto img = (Eo*) gView;
    evas_object_image_pixels_dirty_set(img, EINA_TRUE);
    evas_object_image_data_update_add(img, 0, 0, WIDTH, HEIGHT);

    return ECORE_CALLBACK_RENEW;
}

static unique_ptr<tvg::SwCanvas> swCanvas;

void tvgSwTest(uint32_t* buffer, const char* fileName, const char* animationName, const char* animationName2)
{
    gAnim = true;
    //Create a Canvas
    swCanvas = tvg::SwCanvas::gen();
    swCanvas->target(buffer, WIDTH, WIDTH, HEIGHT, tvg::SwCanvas::ARGB8888);

    tvg::Canvas *canvas = swCanvas.get();

    tvgDrawCmds(canvas);

    actor = new flare::TvgActor();
    actor->load(fileName);
    artboard = actor->artboard<flare::TvgActorArtboard>();
    artboard->initializeGraphics();
    animation = artboard->animation(animationName);
    if (animationName2)
      animation2 = artboard->animation(animationName2);

    lastTime = ecore_time_get();
    ecore_animator_frametime_set(1. / 60);
    animator = ecore_animator_add(animGlCb, canvas);
}

void drawSwView(void* data, Eo* obj)
{
    if (swCanvas && swCanvas->draw() == tvg::Result::Success) {
        swCanvas->sync();
    }
}


static unique_ptr<tvg::GlCanvas> glCanvas;

void initGLview(Evas_Object *obj)
{
    static constexpr auto BPP = 4;

    //Create a Canvas
    glCanvas = tvg::GlCanvas::gen();
    glCanvas->target(nullptr, WIDTH * BPP, WIDTH, HEIGHT);

    tvgDrawCmds(glCanvas.get());
}

void drawGLview(Evas_Object *obj)
{
    auto gl = elm_glview_gl_api_get(obj);
    gl->glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    gl->glClear(GL_COLOR_BUFFER_BIT);

    if (glCanvas->draw() == tvg::Result::Success) {
        glCanvas->sync();
    }
}

void tvgSwTest(uint32_t* buffer, const char* fileName, const char* animationName, const char* animationName2);
void drawSwView(void* data, Eo* obj);

void win_del(void *data, Evas_Object *o, void *ev)
{
   elm_exit();
}

static void
_menu1_clicked(void *data, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED)
{
    gAnim = false;
    if (animator) ecore_animator_del(animator);
    delete actor;
    animation2 = NULL;
    animationTime = 0.0;
    evas_object_move(gView, -150, 200);

    std::string assetPath = AssetsPath;
    assetPath = assetPath + "Mi.flr";
    if (!strcmp((char *)data, "DanceJump"))
      tvgSwTest(buffer, assetPath.c_str(), "Dance", "Jump");
    else if (!strcmp((char *)data, "WaveJump"))
      tvgSwTest(buffer, assetPath.c_str(), "Wave", "Jump");
    else
      tvgSwTest(buffer, assetPath.c_str(), (char *)data, NULL);
}

static void
_menu2_clicked(void *data, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED)
{
    if (animator) ecore_animator_del(animator);
    delete actor;
    animation2 = NULL;
    animationTime = 0.0;
    evas_object_move(gView, 150, 300);

    std::string assetPath = AssetsPath;
    assetPath = assetPath + "ActionMenu.flr";

    tvgSwTest(buffer, assetPath.c_str(), (char*)data, NULL);
}

static void
_menu3_clicked(void *data, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED)
{
    if (animator) ecore_animator_del(animator);
    delete actor;
    animation2 = NULL;
    animationTime = 0.0;
    evas_object_move(gView, -100, 100);

    std::string assetPath = AssetsPath;
    assetPath = assetPath + "Heart.flr";

    tvgSwTest(buffer, assetPath.c_str(), (char*)data, NULL);
}

static void
_menu4_clicked(void *data, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED)
{
    if (animator) ecore_animator_del(animator);
    delete actor;
    animation2 = NULL;
    animationTime = 0.0;
    evas_object_move(gView, 250, 300);

    std::string assetPath = AssetsPath;
    assetPath = assetPath + "effect.flr";

    tvgSwTest(buffer, assetPath.c_str(), (char*)data, NULL);
}

static void
_menu5_clicked(void *data, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED)
{
    if (animator) ecore_animator_del(animator);
    delete actor;
    animation2 = NULL;
    animationTime = 0.0;
    evas_object_move(gView, 50, 350);

    std::string assetPath = AssetsPath;
    assetPath = assetPath + "PushButton.flr";

    tvgSwTest(buffer, assetPath.c_str(), (char*)data, NULL);
}

static void
move_ctxpopup(Evas_Object *ctxpopup, Evas_Object *btn)
{
        Evas_Coord x, y, w , h;
        evas_object_geometry_get(btn, &x, &y, &w, &h);
        evas_object_move(ctxpopup, x + (w / 2), y + (h / 2));
}
                                                                                                      
static void                                                                                           
btn_sample1_cb(void *data, Evas_Object *obj, void *event_info)                                      
{
	Eo *win = (Eo*)data;
        Eo *ctxpopup = elm_ctxpopup_add(win);                                                              
        elm_ctxpopup_item_append(ctxpopup, "Stand", NULL, _menu1_clicked, "Stand");           
        elm_ctxpopup_item_append(ctxpopup, "Dance", NULL, _menu1_clicked, "Dance");             
        elm_ctxpopup_item_append(ctxpopup, "Wave", NULL, _menu1_clicked, "Wave");          
        elm_ctxpopup_item_append(ctxpopup, "Jump", NULL, _menu1_clicked, "Jump");            
                                                                                                      
        move_ctxpopup(ctxpopup, obj);                                                                 
        evas_object_show(ctxpopup);                                                                   
}

static void                                                                                           
btn_sample2_cb(void *data, Evas_Object *obj, void *event_info)
{
	Eo *win = (Eo*)data;
        Eo *ctxpopup = elm_ctxpopup_add(win);                                                              
        elm_ctxpopup_item_append(ctxpopup, "Activate", NULL, _menu2_clicked, "activate");
        elm_ctxpopup_item_append(ctxpopup, "DeActivate", NULL, _menu2_clicked, "deactivate");
 
        move_ctxpopup(ctxpopup, obj);                                                                 
        evas_object_show(ctxpopup);                                                                   
}

static void                                                                                           
btn_sample3_cb(void *data, Evas_Object *obj, void *event_info)
{
	Eo *win = (Eo*)data;
        Eo *ctxpopup = elm_ctxpopup_add(win);                                                              
        elm_ctxpopup_item_append(ctxpopup, "Like", NULL, _menu3_clicked, "Like");
        elm_ctxpopup_item_append(ctxpopup, "Unlike", NULL, _menu3_clicked, "Unlike");
 
        move_ctxpopup(ctxpopup, obj);                                                                 
        evas_object_show(ctxpopup);                                                                   
}

static void                                                                                           
btn_sample4_cb(void *data, Evas_Object *obj, void *event_info)
{
	Eo *win = (Eo*)data;
        Eo *ctxpopup = elm_ctxpopup_add(win);                                                              
        elm_ctxpopup_item_append(ctxpopup, "Alarm", NULL, _menu4_clicked, "Alarm");
 
        move_ctxpopup(ctxpopup, obj);                                                                 
        evas_object_show(ctxpopup);                                                                   
}

static void                                                                                           
btn_sample5_cb(void *data, Evas_Object *obj, void *event_info)
{
	Eo *win = (Eo*)data;
        Eo *ctxpopup = elm_ctxpopup_add(win);                                                              
        elm_ctxpopup_item_append(ctxpopup, "Cancel", NULL, _menu5_clicked, "cancel");
        elm_ctxpopup_item_append(ctxpopup, "Star", NULL, _menu5_clicked, "star");
 
        move_ctxpopup(ctxpopup, obj);                                                                 
        evas_object_show(ctxpopup);                                                                   
}

static Eo* createSwView()
{
    Eo* win = elm_win_util_standard_add(NULL, "ThorVG Test");
    evas_object_smart_callback_add(win, "delete,request", win_del, 0);

    Eo* view = evas_object_image_filled_add(evas_object_evas_get(win));
    evas_object_image_size_set(view, WIDTH, HEIGHT);
    evas_object_image_data_set(view, buffer);
    evas_object_image_pixels_get_callback_set(view, drawSwView, nullptr);
    evas_object_image_pixels_dirty_set(view, EINA_TRUE);
    evas_object_image_data_update_add(view, 0, 0, WIDTH, HEIGHT);
    evas_object_size_hint_weight_set(view, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_show(view);

    Eo *bg = evas_object_rectangle_add(evas_object_evas_get(win));
    evas_object_color_set(bg, 0, 0, 0, 255);
    evas_object_show(bg);
    elm_win_resize_object_add(win, bg);

    evas_object_geometry_set(win, 0, 0, WIDTH, HEIGHT);
    evas_object_move(view, gViewX, gViewY);
    evas_object_resize(view, WIDTH, HEIGHT);
    evas_object_show(win);

    Eo *button = elm_button_add(win);
    evas_object_smart_callback_add(button, "clicked", btn_sample1_cb, win);
    elm_object_text_set(button, "Minions");
    evas_object_resize(button, 200, 50);
    evas_object_move(button, 0, 0);
    evas_object_show(button);

    button = elm_button_add(win);
    evas_object_smart_callback_add(button, "clicked", btn_sample2_cb, win);
    elm_object_text_set(button, "ActionMenu");
    evas_object_resize(button, 250, 50);
    evas_object_move(button, 0, 50);
    evas_object_show(button);

    button = elm_button_add(win);
    evas_object_smart_callback_add(button, "clicked", btn_sample3_cb, win);
    elm_object_text_set(button, "Like");
    evas_object_resize(button, 200, 50);
    evas_object_move(button, 0, 100);
    evas_object_show(button);

    button = elm_button_add(win);
    evas_object_smart_callback_add(button, "clicked", btn_sample4_cb, win);
    elm_object_text_set(button, "Effect");
    evas_object_resize(button, 200, 50);
    evas_object_move(button, 0, 150);
    evas_object_show(button);

    button = elm_button_add(win);
    evas_object_smart_callback_add(button, "clicked", btn_sample5_cb, win);
    elm_object_text_set(button, "Push Button");
    evas_object_resize(button, 300, 50);
    evas_object_move(button, 0, 200);
    evas_object_show(button);

    return view;
}

void initGLview(Evas_Object *obj);
void drawGLview(Evas_Object *obj);

static Eo* createGlView()
{
    elm_config_accel_preference_set("gl");

    Eo* win = elm_win_util_standard_add(NULL, "ThorVG Test");
    evas_object_smart_callback_add(win, "delete,request", win_del, 0);

    Eo* view = elm_glview_add(win);
    evas_object_size_hint_weight_set(view, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    elm_glview_mode_set(view, ELM_GLVIEW_ALPHA);
    elm_glview_resize_policy_set(view, ELM_GLVIEW_RESIZE_POLICY_RECREATE);
    elm_glview_render_policy_set(view, ELM_GLVIEW_RENDER_POLICY_ON_DEMAND);
    elm_glview_init_func_set(view, initGLview);
    elm_glview_render_func_set(view, drawGLview);
    evas_object_show(view);

    elm_win_resize_object_add(win, view);
    evas_object_geometry_set(win, 0, 0, WIDTH, HEIGHT);
    evas_object_show(win);

    return view;
}

int main(int argc, char **argv)
{
    tvg::CanvasEngine tvgEngine = tvg::CanvasEngine::Sw;

    if (argc > 1) {
        if (!strcmp(argv[1], "gl")) tvgEngine = tvg::CanvasEngine::Gl;
   }

    //Initialize ThorVG Engine
    if (tvgEngine == tvg::CanvasEngine::Sw) {
        cout << "tvg engine: software" << endl;
    } else {
        cout << "tvg engine: opengl" << endl;
    }

    //Threads Count
    auto threads = std::thread::hardware_concurrency();

    //Initialize ThorVG Engine
    if (tvg::Initializer::init(tvgEngine, threads) == tvg::Result::Success) {

        elm_init(argc, argv);

        if (tvgEngine == tvg::CanvasEngine::Sw) {
            gView = createSwView();
        } else {
            createGlView();
        }

        elm_run();
        elm_shutdown();

        //Terminate ThorVG Engine
        tvg::Initializer::term(tvgEngine);

    } else {
        cout << "engine is not supported" << endl;
    }
    return 0;
}
