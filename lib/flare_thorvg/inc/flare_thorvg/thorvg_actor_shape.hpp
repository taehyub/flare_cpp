#ifndef _SKR_ACTOR_SHAPE_HPP_
#define _SKR_ACTOR_SHAPE_HPP_

#ifndef FLARE_EXPORT
#define FLARE_EXPORT __attribute__((visibility("default")))
#endif

#include "flare/actor_shape.hpp"
#include "thorvg_drawable.hpp"
#include <thorvg.h>
#include <vector>
#include <stdint.h>

namespace flare
{
	class TvgActorBasePath;
	class TvgFill;
	class TvgStroke;

	class FLARE_EXPORT TvgActorShape : public ActorShape, public TvgDrawable
	{
		typedef ActorShape Base;

	private:
		// Flare - Thorvg
		tvg::Shape *m_Path;
		bool m_IsValid;
                bool pushed;
		std::vector<TvgActorBasePath*> m_SubPaths;
		std::vector<tvg::Shape *> clipPaths;

		// We currently only support rendering a single fill/stroke so we only
		// store these two concrete pointers.
		TvgStroke* m_Stroke;
		TvgFill* m_Fill;

	protected:
		void onBlendModeChanged(BlendMode from, BlendMode to) override;
		void onRenderOpacityChanged() override;
		void onChildrenChanged() override;

	public:
		TvgActorShape();
		void invalidateDrawable() override;

		void draw(tvg::Canvas *canvas) override;
		void path(tvg::Canvas *canvas, bool pushed = false);
		ActorFill* getFill();

	protected:
		void onStrokesChanged() override;
		void onFillsChanged() override;
		void onClipsChanged() override { TvgDrawable::updateClips(this); }
	};
} // namespace flare

#endif
