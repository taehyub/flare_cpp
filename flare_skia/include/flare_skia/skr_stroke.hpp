#ifndef _SKR_STROKE_HPP_
#define _SKR_STROKE_HPP_

#include "SkCanvas.h"
#include "SkPaint.h"
#include "SkPath.h"
#include "flare/paint/actor_stroke.hpp"
#include "flare_math/color.hpp"
#include <thorvg.h>

namespace flare
{
	class SkrStroke
	{
	protected:
		SkPaint m_Paint;
        SkPath* m_EffectPath;
        ActorStroke* m_ActorStroke;

	public:
		void initializeGraphics();
		virtual void onPaintUpdated() {}
		void updatePaint();
		void paint(SkCanvas* canvas, SkPath& path);
	};

	class TvgStroke
	{
	protected:
		SkPaint m_Paint;
        SkPath* m_EffectPath;
        ActorStroke* m_ActorStroke;

	public:
                flare::Color m_color;
		void initializeGraphics();
		virtual void onPaintUpdated() {}
		void updatePaint();
		void paint(tvg::Canvas *canvas, tvg::Shape *path);
	};
} // namespace flare
#endif
