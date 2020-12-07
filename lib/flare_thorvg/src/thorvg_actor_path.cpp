#include "flare_thorvg/thorvg_actor_path.hpp"

using namespace flare;

TvgActorPath::TvgActorPath() : TvgActorBasePath(this) {}

void TvgActorPath::invalidateDrawable()
{
	Base::invalidateDrawable();
	m_IsPathValid = false;
}
