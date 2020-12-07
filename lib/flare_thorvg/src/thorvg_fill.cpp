#include "flare_thorvg/thorvg_fill.hpp"
#include <cassert>

using namespace flare;

void TvgFill::initializeGraphics()
{
	m_ActorFill = dynamic_cast<ActorFill*>(this);
	assert(m_ActorFill != nullptr);

	onPaintUpdated();
}

void TvgFill::paint(tvg::Canvas *canvas, tvg::Shape *path)
{
	m_ActorFill->validatePaint();

	switch (m_ActorFill->fillRule())
	{
		case FillRule::EvenOdd:
			path->fill(tvg::FillRule::EvenOdd);
			break;
		case FillRule::NonZero:
			break;
	}

	int r, g, b, a;
	r = this->m_color[0] * 255;
	g = this->m_color[1] * 255;
	b = this->m_color[2] * 255;
	a = this->m_color[3] * 255;
	path->fill(r, g, b, a);
	canvas->update(path);
}
