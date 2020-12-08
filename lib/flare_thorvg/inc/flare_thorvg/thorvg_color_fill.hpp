#ifndef _SKR_COLOR_FILL_HPP_
#define _SKR_COLOR_FILL_HPP_

#ifndef FLARE_EXPORT
#define FLARE_EXPORT __attribute__((visibility("default")))
#endif

#include "flare/paint/actor_color_fill.hpp"
#include "thorvg_fill.hpp"

namespace flare
{
	class FLARE_EXPORT TvgColorFill : public ActorColorFill, public TvgFill
	{
		typedef ActorColorFill Base;

	public:
		void initializeGraphics() override { TvgFill::initializeGraphics(); }
		void updateColor(float r, float g, float b, float a);
		void updatePaint() override;
	};
} // namespace flare

#endif
