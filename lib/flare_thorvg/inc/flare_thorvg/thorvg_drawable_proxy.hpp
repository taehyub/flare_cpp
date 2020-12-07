#ifndef _SKR_DRAWABLE_PROXY_HPP_
#define _SKR_DRAWABLE_PROXY_HPP_

#include "flare/actor_drawable_proxy.hpp"

namespace flare
{
    class TvgDrawable;

	class TvgDrawableProxy : public ActorDrawableProxy
	{
        typedef ActorDrawableProxy Base;

        TvgDrawable* m_TvgDrawable;
	public:
		TvgDrawableProxy(ActorDrawable* drawable);
        TvgDrawable* skrDrawable() const { return m_TvgDrawable; }
	};
} // namespace flare
#endif
