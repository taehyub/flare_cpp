#ifndef _SKR_ACTOR_SHAPE_HPP_
#define _SKR_ACTOR_SHAPE_HPP_

#include "flare/actor_shape.hpp"
#include "skr_drawable.hpp"
#include "SkPath.h"
#include <thorvg.h>
#include <vector>
#include <stdint.h>

namespace flare
{
	class SkrActorBasePath;
	class SkrFill;
	class SkrStroke;

	class SkrActorShape : public ActorShape, public SkrDrawable
	{
		typedef ActorShape Base;

	private:
		bool m_IsValid;
		SkPath m_Path;
		std::vector<SkrActorBasePath*> m_SubPaths;

		// We currently only support rendering a single fill/stroke so we only
		// store these two concrete pointers.
		SkrStroke* m_Stroke;
		SkrFill* m_Fill;

	protected:
		void onBlendModeChanged(BlendMode from, BlendMode to) override;
		void onRenderOpacityChanged() override;
		void onChildrenChanged() override;

	public:
		SkrActorShape();
		void invalidateDrawable() override;

		void draw(SkCanvas* canvas) override;
		SkPath& path();

	protected:
		void onStrokesChanged() override;
		void onFillsChanged() override;
		void onClipsChanged() override { SkrDrawable::updateClips(this); }
	};

	class TvgActorBasePath;
	class TvgFill;
	class TvgStroke;

	class TvgActorShape : public ActorShape, public TvgDrawable
	{
		typedef ActorShape Base;

	private:
		// Flare - Thorvg
		tvg::Shape *tvgPath;
		bool m_IsValid;
                bool pushed;
		SkPath m_Path;
		std::vector<TvgActorBasePath*> m_SubPaths;

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
		void path(tvg::Canvas *canvas);

	protected:
		void onStrokesChanged() override;
		void onFillsChanged() override;
		void onClipsChanged() override { TvgDrawable::updateClips(this); }
	};
} // namespace flare

#endif
