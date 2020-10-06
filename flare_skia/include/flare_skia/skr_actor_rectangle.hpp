#ifndef _SKR_ACTOR_RECTANGLE_HPP_
#define _SKR_ACTOR_RECTANGLE_HPP_

#include "flare/path/actor_rectangle.hpp"
#include "skr_actor_base_path.hpp"

namespace flare
{

	class SkrActorRectangle : public ActorRectangle, public SkrActorBasePath
	{
		typedef ActorRectangle Base;

	public:
		SkrActorRectangle();

		void invalidateDrawable() override;
	};

	class TvgActorRectangle : public ActorRectangle, public TvgActorBasePath
	{
		typedef ActorRectangle Base;

	public:
		TvgActorRectangle();

		void invalidateDrawable() override;
	};
} // namespace flare

#endif