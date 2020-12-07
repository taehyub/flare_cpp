#ifndef _SKR_ACTOR_BASE_PATH_HPP_
#define _SKR_ACTOR_BASE_PATH_HPP_

#include <thorvg.h>

namespace flare
{
	class ActorBasePath;

	class TvgActorBasePath
	{
		ActorBasePath* m_BasePath;

	protected:
		bool m_IsPathValid;

	private:
		void updatePath(tvg::Shape *tvgPath);

	protected:
		TvgActorBasePath(ActorBasePath* path);

	public:
		void path(tvg::Shape *tvgPath);
		ActorBasePath* basePath() const { return m_BasePath; }
	};

} // namespace flare

#endif
