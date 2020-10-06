#ifndef _SKR_ACTOR_ARTBOARD_HPP_
#define _SKR_ACTOR_ARTBOARD_HPP_

#include "flare/actor_artboard.hpp"
#include <thorvg.h>

class SkCanvas;

namespace flare
{
	class SkrActor;
	class SkrActorArtboard : public ActorArtboard
	{
        typedef Actor Base;
	public:
		SkrActorArtboard(SkrActor* actor);
		void initializeGraphics();
		void draw(SkCanvas* canvas);
	};

	class TvgActor;
	class TvgActorArtboard : public ActorArtboard
	{
        typedef Actor Base;
	public:
		TvgActorArtboard(TvgActor* actor);
		void initializeGraphics();
		void draw(tvg::Canvas *canvas);
	};
} // namespace flare
#endif