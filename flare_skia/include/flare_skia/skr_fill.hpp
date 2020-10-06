#ifndef _SKR_FILL_HPP_
#define _SKR_FILL_HPP_

#include "SkCanvas.h"
#include "SkPaint.h"
#include "SkPath.h"
#include "flare/paint/actor_fill.hpp"
#include "flare_math/color.hpp"
#include <thorvg.h>

namespace flare
{
	class SkrFill
	{
	protected:
		SkPaint m_Paint;
		ActorFill* m_ActorFill;

	public:
		void initializeGraphics();
		virtual void onPaintUpdated() {}
		void updatePaint();
		void paint(SkCanvas* canvas, SkPath& path);
	};

	class TvgFill
	{
	protected:
		SkPaint m_Paint;
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
