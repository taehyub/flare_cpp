#include "flare_thorvg/thorvg_gradient_stroke.hpp"
#include "flare/actor_shape.hpp"

/*
#include "SkGradientShader.h"
#include "SkShader.h"
*/
#include <vector>

using namespace flare;

/*
void SkrGradientStroke::updatePaint()
{
	m_Paint.setBlendMode(ToSkia::convert(m_Shape->blendMode()));
    SkrStroke::updatePaint();

	const std::vector<float>& stops = colorStops();
	int numStops = stops.size() / 5;
	SkPoint points[2] = {ToSkia::convert(renderStart()), ToSkia::convert(renderEnd())};
	SkColor colors[numStops];
	SkScalar pos[numStops];

	flare::Color color;
	int idx = 0;
	for (int i = 0; i < numStops; i++)
	{
		color[0] = stops[idx++];
		color[1] = stops[idx++];
		color[2] = stops[idx++];
		color[3] = stops[idx++];

		colors[i] = ToSkia::convert(color).toSkColor();
		pos[i] = stops[idx++];
	}

	m_Paint.setShader(SkGradientShader::MakeLinear(points, colors, pos, numStops, SkTileMode::kClamp, 0, nullptr));
	onPaintUpdated();
}
*/
