#include "flare_thorvg/thorvg_actor_ellipse.hpp"

using namespace flare;

TvgActorEllipse::TvgActorEllipse() : TvgActorBasePath(this) {}

void TvgActorEllipse::invalidateDrawable() { m_IsPathValid = false; }
