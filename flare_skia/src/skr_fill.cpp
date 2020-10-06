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

void TvgFill::initializeGraphics()
{
	m_ActorFill = dynamic_cast<ActorFill*>(this);
	assert(m_ActorFill != nullptr);

	m_Paint.setStyle(SkPaint::Style::kFill_Style);
	m_Paint.setAntiAlias(true);
	onPaintUpdated();
}

void TvgFill::paint(tvg::Canvas *canvas, tvg::Shape *path)
{
	m_ActorFill->validatePaint();

	//FIXME: Tvg Filltype is not supported yet
	switch (m_ActorFill->fillRule())
	{
		case FillRule::EvenOdd:

	//		path.setFillType(SkPathFillType::kEvenOdd);
			break;
		case FillRule::NonZero:
	//		path.setFillType(SkPathFillType::kEvenOdd);
			break;
	}

	int r, g, b, a;
	r = this->m_color[0] * 255;
	g = this->m_color[1] * 255;
	b = this->m_color[2] * 255;
	a = this->m_color[3] * 255;
	path->fill(r, g, b, a);
}
