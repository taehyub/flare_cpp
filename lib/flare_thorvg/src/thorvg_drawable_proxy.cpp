#include "flare/actor_drawable.hpp"
#include "flare_thorvg/thorvg_drawable_proxy.hpp"
#include "flare_thorvg/thorvg_drawable.hpp"

using namespace flare;

TvgDrawableProxy::TvgDrawableProxy(ActorDrawable* drawable) : Base(drawable), m_TvgDrawable(dynamic_cast<TvgDrawable*>(drawable))
{

}
