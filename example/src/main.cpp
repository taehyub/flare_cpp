#define SK_GL

//const char* fileName = "assets/clippath.flr";
//const char* animationName = "Untitled";
const char* fileName = "assets/Mi.flr";
const char* animationName = "Wave";
//const char* fileName = "assets/evenodd.flr";
//const char* animationName = "Untitled";
//const char* fileName = "assets/Ball.flr";
//const char* animationName = "Bounds";
//const char* fileName = "assets/Filip.flr";
//const char* animationName = "idle";
// const char* fileName = "assets/Teddy.flr";
// const char* animationName = "test";

#include "GrBackendSurface.h"
#include "GrContext.h"
#include "SkCanvas.h"
#include "SkColorSpace.h"
#include "SkSurface.h"
#include "SkTypes.h"
#include "flare/animation/actor_animation.hpp"
#include "flare/exceptions/overflow_exception.hpp"
#include "flare/exceptions/unsupported_version_exception.hpp"
#include "flare_math/mat2d.hpp"
#include "flare_skia/skr_actor.hpp"
#include "flare_skia/skr_actor_artboard.hpp"
#include "flare_skia/to_skia.hpp"
#include "gl/GrGLInterface.h"
#include "layout.hpp"
#include <cmath>
#include <stdio.h>

//#define FLARE_SKIA
//Flare - Skia
#ifdef FLARE_SKIA

#include "GLFW/glfw3.h"

bool isRightMouseDown = false;
double mouseX = 0.0, mouseY = 0.0;
double lastMouseX = 0.0, lastMouseY = 0.0;
double viewX = 0.0, viewY = 0.0;
double viewTargetX = 0.0, viewTargetY = 0.0;
double viewZoom = 1.0;
double viewZoomTarget = 1.0;
int perfBarWidth = 10;

void zoomTo(double x, double y, double scale)
{
	scale = std::max(0.05, scale);
	double zoomDelta = scale / viewZoomTarget;
	viewZoomTarget = scale;

	double ox = x - viewTargetX;
	double oy = y - viewTargetY;

	double ox2 = ox * zoomDelta;
	double oy2 = oy * zoomDelta;

	viewTargetX += ox - ox2;
	viewTargetY += oy - oy2;
}

void glfwErrorCallback(int error, const char* description) { puts(description); }

static void glfwCursorPositionCallback(GLFWwindow* window, double x, double y)
{
	mouseX = x;
	mouseY = y;
	if (isRightMouseDown)
	{
		// dragging
		double dx = (x - lastMouseX);
		double dy = (y - lastMouseY);
		viewTargetX += dx;
		viewTargetY += dy;
	}
	lastMouseX = mouseX;
	lastMouseY = mouseY;
}

void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
        viewTargetX = viewTargetY = 0.0f;
		viewZoomTarget = 1.0f;
	}
}

void glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		switch (action)
		{
			case GLFW_PRESS:
				lastMouseX = mouseX;
				lastMouseY = mouseY;
				isRightMouseDown = true;
				break;
			case GLFW_RELEASE:
				isRightMouseDown = false;
				break;
		}
	}
}

std::vector<float> performanceBars;
int performanceIndex = 0;

void glfwScrollCallback(GLFWwindow* window, double x, double y) { zoomTo(mouseX, mouseY, viewZoomTarget + y / 22.0); }

int main()
{
	if (!glfwInit())
	{
		// Initialization failed
		printf("Failed to initialize glfw.\n");
		return 1;
	}
	glfwSetErrorCallback(glfwErrorCallback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	GLFWwindow* window = glfwCreateWindow(1280, 720, "Flare", NULL, NULL);
	//GLFWwindow* window = glfwCreateWindow(1920, 1080, "Flare", NULL, NULL);
	if (!window)
	{
		// Window or OpenGL context creation failed
		printf("Failed to make window or GL.\n");
		glfwTerminate();
		return 1;
	}
	glfwSetCursorPosCallback(window, glfwCursorPositionCallback);
	glfwSetMouseButtonCallback(window, glfwMouseButtonCallback);
	glfwSetKeyCallback(window, glfwKeyCallback);
	glfwSetScrollCallback(window, glfwScrollCallback);

	glfwMakeContextCurrent(window);
	// Enable VSYNC.
	glfwSwapInterval(1);

	// Setup Skia
	GrContextOptions options;
	sk_sp<GrContext> context = GrContext::MakeGL(nullptr, options);
	GrGLFramebufferInfo framebufferInfo;
	// glGetIntegerv(GL_FRAMEBUFFER_BINDING, &framebufferInfo.fFBOID);
	framebufferInfo.fFBOID = 0;
	framebufferInfo.fFormat = GL_RGBA8;

	SkSurface* surface = nullptr;
	SkCanvas* canvas = nullptr;

	flare::SkrActor* actor = new flare::SkrActor();

	try
	{
		actor->load(fileName);
	}
	catch (flare::OverflowException ex)
	{
		printf("Bad data, got an overflow. %s\n", ex.message().c_str());
	}
	catch (flare::UnsupportedVersionException ex)
	{
		printf("Unsupported version. %d %d\n", ex.versionFound(), ex.versionRequired());
	}
	printf("Loaded ok.\n");

	flare::SkrActorArtboard* artboard = actor->artboard<flare::SkrActorArtboard>();
	artboard->initializeGraphics();

	flare::ActorAnimation* animation = artboard->animation(animationName);
	float animationTime = 0.0f;

	int width = 0, height = 0;
	int lastScreenWidth = 0, lastScreenHeight = 0;
	double lastTime = glfwGetTime();
	while (!glfwWindowShouldClose(window))
	{
		glfwGetFramebufferSize(window, &width, &height);
		if (surface == nullptr || width != lastScreenWidth || height != lastScreenHeight)
		{
			performanceBars.resize(width / perfBarWidth);
			performanceIndex = 0;

			lastScreenWidth = width;
			lastScreenHeight = height;

			SkColorType colorType;
//			if (kRGBA_8888_GrPixelConfig == kSkia8888_GrPixelConfig)
//			{
				colorType = kRGBA_8888_SkColorType;
//			}
//			else
//			{
///				colorType = kBGRA_8888_SkColorType;
//			}

			GrBackendRenderTarget backendRenderTarget(width, height,
			                                          0, // sample count
			                                          0, // stencil bits
			                                          framebufferInfo);

			delete surface;
			surface = SkSurface::MakeFromBackendRenderTarget(context.get(), backendRenderTarget, kBottomLeft_GrSurfaceOrigin,
			                                                            colorType, nullptr, nullptr)
			                         .release();
			if (surface == nullptr)
			{
				printf("Failed to create Skia surface\n");
				return 1;
			}
			canvas = surface->getCanvas();
		}

		double time = glfwGetTime();
		float elapsed = (float)(time - lastTime);
		lastTime = time;

		// --- update camera
		double ds = viewZoomTarget - viewZoom;
		double dx = viewTargetX - viewX;
		double dy = viewTargetY - viewY;

		double factor = std::min(1.0, elapsed * 16.0);

		if (std::abs(ds) > 0.00001)
		{
			ds *= factor;
		}
		if (std::abs(dx) > 0.01)
		{
			dx *= factor;
		}
		if (std::abs(dy) > 0.01)
		{
			dy *= factor;
		}

		viewZoom += ds;
		viewX += dx;
		viewY += dy;

		flare::Mat2D view;
		view[0] = viewZoom;
		view[3] = viewZoom;
		view[4] = viewX;
		view[5] = viewY;
		// ---

		if (animation != nullptr)
		{
			animationTime += elapsed;
			animation->apply(std::fmod(animationTime, animation->duration()), artboard, 1.0);
		}
		artboard->advance(elapsed);

		SkPaint paint;
		paint.setColor(SK_ColorDKGRAY);
		canvas->drawPaint(paint);
		flare::AABB screenBounds(0.0f, 0.0f, width, height);
		canvas->save();
		canvas->concat(flare::ToSkia::convert(view));
		flare::Alignment::center().transform(canvas, screenBounds, artboard->aabb(), flare::Fit::contain);

		artboard->draw(canvas);
		canvas->restore();

		// Draw some perf metrics.
		float fps = 1.0 / elapsed;
		performanceBars[performanceIndex % performanceBars.size()] = fps;
		performanceIndex = (performanceIndex + 1) % performanceBars.size();

		float x = 0.0;
		paint.setColor(SkColorSetARGB(0x88, 0xAA, 0xAA, 0xAA));
		for (float fps : performanceBars)
		{
			float h = fps / 60.0 * height/3.0;

			canvas->drawRect({x, height - h, x+static_cast<SkScalar>(perfBarWidth)-1.0f, static_cast<SkScalar>(height)}, paint);

			x += perfBarWidth;
		}
		paint.setColor(SkColorSetARGB(0xFF, 0xFF, 0x56, 0x78));
		canvas->drawRect({0.0f, height*0.66f, static_cast<SkScalar>(width), height*0.66f+2.0f}, paint);

		context->flush();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Cleanup.
	delete actor;

	// Cleanup Skia.
	delete surface;
	context = nullptr;

	// Cleanup GLFW.
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

//Flare - Thorvg
#else

#include "flare/paint/actor_color_fill.hpp"
#include "flare_skia/skr_color_fill.hpp"
#include <iostream>
#include <thread>
#include <Evas_GL.h>
#include <Elementary.h>
#include <thorvg.h>

#define WIDTH 1024
#define HEIGHT 768

Eo *gView;
flare::TvgActorArtboard* artboard;
flare::ActorAnimation* animation;

using namespace std;

void tvgDrawCmds(tvg::Canvas* canvas)
{
    if (!canvas) return;

}

double animationTime = 0.0;
double elapsed = 0.0;
double lastTime = 0.0;
double currentTime = 0.0;

Eina_Bool animGlCb(void* data)
{
    tvg::Canvas *canvas = (tvg::Canvas*)data;

    currentTime = ecore_time_get();
    double elapsed = currentTime - lastTime;
    lastTime = currentTime;
    animationTime += elapsed;
    animation->apply(std::fmod(animationTime, animation->duration()), artboard, 1.0);
    artboard->advance(elapsed);
    artboard->draw(canvas);

    auto img = (Eo*) gView;
    evas_object_image_pixels_dirty_set(img, EINA_TRUE);
    evas_object_image_data_update_add(img, 0, 0, WIDTH, HEIGHT);

    return ECORE_CALLBACK_RENEW;
}


static unique_ptr<tvg::SwCanvas> swCanvas;

void tvgSwTest(uint32_t* buffer)
{
    //Create a Canvas
    swCanvas = tvg::SwCanvas::gen();
    swCanvas->target(buffer, WIDTH, WIDTH, HEIGHT, tvg::SwCanvas::ARGB8888);

    tvg::Canvas *canvas = swCanvas.get();

    tvgDrawCmds(canvas);

    flare::TvgActor* actor = new flare::TvgActor();
    actor->load(fileName);
    artboard = actor->artboard<flare::TvgActorArtboard>();
    artboard->initializeGraphics();
    animation = artboard->animation(animationName);

    lastTime = ecore_time_get();
    ecore_animator_frametime_set(1. / 60);
    ecore_animator_add(animGlCb, canvas);
}

void drawSwView(void* data, Eo* obj)
{
    if (swCanvas->draw() == tvg::Result::Success) {
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

void tvgSwTest(uint32_t* buffer);
void drawSwView(void* data, Eo* obj);

void win_del(void *data, Evas_Object *o, void *ev)
{
   elm_exit();
}

static Eo* createSwView()
{
    static uint32_t buffer[WIDTH * HEIGHT];

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

    elm_win_resize_object_add(win, view);
    evas_object_geometry_set(win, 0, 0, WIDTH, HEIGHT);
    evas_object_show(win);

    tvgSwTest(buffer);

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
#endif
