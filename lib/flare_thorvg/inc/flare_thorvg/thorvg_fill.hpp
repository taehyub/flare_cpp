#ifndef _SKR_FILL_HPP_
#define _SKR_FILL_HPP_

#include "flare/paint/actor_fill.hpp"
#include "flare_math/color.hpp"
#include <thorvg.h>

namespace flare
{
	class TvgFill
	{
	protected:
		ActorFill* m_ActorFill;

	public:
                flare::Color m_color;
		void initializeGraphics();
		virtual void onPaintUpdated() {}
		void updatePaint();
		void paint(tvg::Canvas *canvas, tvg::Shape *path);
	};
} // namespace flare
#endif
