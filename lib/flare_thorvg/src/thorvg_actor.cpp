#include "flare_thorvg/thorvg_actor.hpp"
#include "flare_thorvg/thorvg_actor_artboard.hpp"
#include "flare_thorvg/thorvg_actor_ellipse.hpp"
#include "flare_thorvg/thorvg_actor_path.hpp"
#include "flare_thorvg/thorvg_actor_polygon.hpp"
#include "flare_thorvg/thorvg_actor_rectangle.hpp"
#include "flare_thorvg/thorvg_actor_shape.hpp"
#include "flare_thorvg/thorvg_actor_star.hpp"
#include "flare_thorvg/thorvg_actor_triangle.hpp"
#include "flare_thorvg/thorvg_color_fill.hpp"
#include "flare_thorvg/thorvg_color_stroke.hpp"
#include "flare_thorvg/thorvg_drawable_proxy.hpp"
#include "flare_thorvg/thorvg_gradient_fill.hpp"
#include "flare_thorvg/thorvg_gradient_stroke.hpp"
#include "flare_thorvg/thorvg_radial_gradient_fill.hpp"
#include "flare_thorvg/thorvg_radial_gradient_stroke.hpp"

using namespace flare;

//Flare - ThorVg
ActorArtboard* TvgActor::makeArtboard() { return new TvgActorArtboard(this); }
ActorShape* TvgActor::makeShape() { return new TvgActorShape(); }
ActorColorFill* TvgActor::makeColorFill() { return new TvgColorFill(); }
ActorColorStroke* TvgActor::makeColorStroke() { return new TvgColorStroke(); }
ActorPath* TvgActor::makePath() { return new TvgActorPath(); }
ActorRectangle* TvgActor::makeRectangle() { return new TvgActorRectangle(); }
ActorEllipse* TvgActor::makeEllipse() { return new TvgActorEllipse(); }
ActorDrawableProxy* TvgActor::makeDrawableProxy(ActorDrawable* drawable) { return new TvgDrawableProxy(drawable); }
