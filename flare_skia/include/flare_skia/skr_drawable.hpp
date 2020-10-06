#ifndef _SKR_DRAWABLE_HPP_
#define _SKR_DRAWABLE_HPP_

class SkCanvas;

#include <vector>
#include <stdint.h>
#include <thorvg.h>

namespace flare
{
	class ActorNode;
	class SkrActorShape;
	class SkrDrawable
	{
		std::vector<std::vector<SkrActorShape*>> m_ClippingShapes;

	public:
		virtual void draw(SkCanvas* canvas) = 0;
		void updateClips(ActorNode* node);

		const std::vector<std::vector<SkrActorShape*>>& clippingShapes() const { return m_ClippingShapes; }
	};

	class TvgActorShape;
	class TvgDrawable
	{
		std::vector<std::vector<TvgActorShape*>> m_ClippingShapes;

	public:
		virtual void draw(tvg::Canvas *canvas) = 0;
		void updateClips(ActorNode* node);

		const std::vector<std::vector<TvgActorShape*>>& clippingShapes() const { return m_ClippingShapes; }
	};
} // namespace flare
#endif