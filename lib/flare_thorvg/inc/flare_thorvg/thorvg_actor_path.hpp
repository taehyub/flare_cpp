#ifndef _SKR_ACTOR_PATH_HPP_
#define _SKR_ACTOR_PATH_HPP_

#include "flare/path/actor_path.hpp"
#include "thorvg_actor_base_path.hpp"

namespace flare
{
	class TvgActorPath : public ActorPath, public TvgActorBasePath
	{
		typedef ActorPath Base;

	public:
		TvgActorPath();

		void invalidateDrawable() override;
	};
} // namespace flare

#endif
