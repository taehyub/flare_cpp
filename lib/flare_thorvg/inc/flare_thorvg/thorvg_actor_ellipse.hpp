#ifndef _SKR_ACTOR_ELLIPSE_HPP_
#define _SKR_ACTOR_ELLIPSE_HPP_

#include "flare/path/actor_ellipse.hpp"
#include "thorvg_actor_base_path.hpp"

namespace flare
{
	class TvgActorEllipse : public ActorEllipse, public TvgActorBasePath
	{
		typedef ActorEllipse Base;

	public:
		TvgActorEllipse();

		void invalidateDrawable() override;
	};
} // namespace flare

#endif
