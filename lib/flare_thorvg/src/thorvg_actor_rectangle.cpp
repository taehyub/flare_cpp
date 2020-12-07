#include "flare_thorvg/thorvg_actor_rectangle.hpp"

using namespace flare;

TvgActorRectangle::TvgActorRectangle() : TvgActorBasePath(this) {}

void TvgActorRectangle::invalidateDrawable() { m_IsPathValid = false; }
