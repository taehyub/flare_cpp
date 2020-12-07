#ifndef _SKR_ACTOR_RECTANGLE_HPP_
#define _SKR_ACTOR_RECTANGLE_HPP_

#include "flare/path/actor_rectangle.hpp"
#include "thorvg_actor_base_path.hpp"

namespace flare
{
	class TvgActorRectangle : public ActorRectangle, public TvgActorBasePath
	{
		typedef ActorRectangle Base;

	public:
		TvgActorRectangle();

		void invalidateDrawable() override;
	};
} // namespace flare

#endif
