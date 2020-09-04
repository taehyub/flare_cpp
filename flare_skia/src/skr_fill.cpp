#include "flare_skia/skr_fill.hpp"
#include <cassert>

using namespace flare;

void SkrFill::initializeGraphics()
{
	m_ActorFill = dynamic_cast<ActorFill*>(this);
	assert(m_ActorFill != nullptr);

	m_Paint.setStyle(SkPaint::Style::kFill_Style);
	m_Paint.setAntiAlias(true);
	onPaintUpdated();
}

void SkrFill::paint(SkCanvas* canvas, SkPath& path)
{
	m_ActorFill->validatePaint();
	switch (m_ActorFill->fillRule())
	{
		case FillRule::EvenOdd:
			path.setFillType(SkPathFillType::kEvenOdd);
			break;
		case FillRule::NonZero:
			path.setFillType(SkPathFillType::kEvenOdd);
			break;
	}
	canvas->drawPath(path, m_Paint);
}