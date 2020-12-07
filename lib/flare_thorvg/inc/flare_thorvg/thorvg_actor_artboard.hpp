#ifndef _SKR_ACTOR_ARTBOARD_HPP_
#define _SKR_ACTOR_ARTBOARD_HPP_

#include "flare/actor_artboard.hpp"
#include <thorvg.h>

#ifndef FLARE_EXPORT
#define FLARE_EXPORT __attribute__((visibility("default")))
#endif

namespace flare
{
	class FLARE_EXPORT TvgActor;
	class FLARE_EXPORT TvgActorArtboard : public ActorArtboard
	{
        typedef Actor Base;
	public:
		TvgActorArtboard(TvgActor* actor);
		void initializeGraphics();
		void draw(tvg::Canvas *canvas);
	};
} // namespace flare
#endif
