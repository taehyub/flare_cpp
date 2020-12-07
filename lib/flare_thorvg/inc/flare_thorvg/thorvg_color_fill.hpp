#ifndef _SKR_COLOR_FILL_HPP_
#define _SKR_COLOR_FILL_HPP_

#include "flare/paint/actor_color_fill.hpp"
#include "thorvg_fill.hpp"

namespace flare
{
	class TvgColorFill : public ActorColorFill, public TvgFill
	{
		typedef ActorColorFill Base;

	public:
		void initializeGraphics() override { TvgFill::initializeGraphics(); }
		void updatePaint() override;
	};
} // namespace flare

#endif
