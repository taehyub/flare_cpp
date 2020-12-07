#include "flare_thorvg/thorvg_actor_artboard.hpp"
#include "flare_thorvg/thorvg_actor.hpp"
#include "flare_thorvg/thorvg_drawable.hpp"
#include "flare_thorvg/thorvg_drawable_proxy.hpp"

using namespace flare;

TvgActorArtboard::TvgActorArtboard(TvgActor* actor) : ActorArtboard(actor) {}

void TvgActorArtboard::initializeGraphics()
{
	for (int i = 0; i < m_DrawableCount; i++)
	{
		m_Drawables[i]->drawable()->initializeGraphics();
	}
}

void TvgActorArtboard::draw(tvg::Canvas *canvas)
{
	for (int i = 0; i < m_DrawableCount; i++)
	{
		reinterpret_cast<TvgDrawableProxy*>(m_Drawables[i])->skrDrawable()->draw(canvas);
	}
}



