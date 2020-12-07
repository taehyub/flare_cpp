#ifndef _SKR_COLOR_STROKE_HPP_
#define _SKR_COLOR_STROKE_HPP_

#include "flare/paint/actor_color_stroke.hpp"
#include "thorvg_stroke.hpp"

namespace flare
{
	class TvgColorStroke : public ActorColorStroke, public TvgStroke
	{
		typedef ActorColorStroke Base;

	public:
		void initializeGraphics() override { TvgStroke::initializeGraphics(); }
		void updatePaint() override;
	};
} // namespace flare

#endif
