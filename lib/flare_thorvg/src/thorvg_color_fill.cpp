#include "flare_thorvg/thorvg_color_fill.hpp"
#include "flare/actor_shape.hpp"

using namespace flare;

void TvgColorFill::updateColor(float r, float g, float b, float a)
{
   m_Color[0] = r;
   m_Color[1] = g;
   m_Color[2] = b;
   m_Color[3] = a;

   updatePaint();
}

void TvgColorFill::updatePaint()
{
    this->m_color[0] = m_Color[0];
    this->m_color[1] = m_Color[1];
    this->m_color[2] = m_Color[2];
    this->m_color[3] = m_Color[3] * m_RenderOpacity;

    onPaintUpdated();
}
