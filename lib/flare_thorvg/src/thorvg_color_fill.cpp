#include "flare_thorvg/thorvg_color_fill.hpp"
#include "flare/actor_shape.hpp"

using namespace flare;

void TvgColorFill::updatePaint()
{
    this->m_color[0] = m_Color[0];
    this->m_color[1] = m_Color[1];
    this->m_color[2] = m_Color[2];
    this->m_color[3] = m_Color[3] * m_RenderOpacity;

    onPaintUpdated();
}
