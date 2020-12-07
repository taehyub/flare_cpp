#ifndef _SKR_STROKE_HPP_
#define _SKR_STROKE_HPP_

#include "flare/paint/actor_stroke.hpp"
#include "flare_math/color.hpp"
#include <thorvg.h>
#include <vector>

namespace flare
{
	class TvgStroke
	{
	protected:
		ActorStroke *m_ActorStroke;
		tvg::Shape *m_OriginShape;
		std::vector<tvg::PathCommand> m_TrimPathCmds;
		std::vector<tvg::Point> m_TrimPathPoints;
	public:
                flare::Color m_color;
		void initializeGraphics();
		virtual void onPaintUpdated() {}
		void updatePaint();
		void paint(tvg::Canvas *canvas, tvg::Shape *path);
	};
} // namespace flare
#endif
