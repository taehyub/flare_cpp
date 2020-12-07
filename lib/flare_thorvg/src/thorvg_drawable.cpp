#include "flare_thorvg/thorvg_drawable.hpp"
#include "flare/actor_node.hpp"
#include "flare_thorvg/thorvg_actor_shape.hpp"

using namespace flare;

void TvgDrawable::updateClips(ActorNode* node)
{
	m_ClippingShapes.clear();

	ActorNode* clipSearch = node;
	while (clipSearch != nullptr)
	{
		const std::vector<ActorNodeClip>& clips = clipSearch->clips();

		if (clips.size() > 0)
		{
			std::vector<TvgActorShape*> clippingLevel;
			for (const ActorNodeClip& clip : clips)
			{
				clip.node->all([&](ActorNode* child) {
					if (child->type() == ComponentType::ActorShape)
					{
						TvgActorShape* shape = static_cast<TvgActorShape*>(child);
						if (shape != nullptr)
						{
							clippingLevel.push_back(shape);
						}
					}

					return true;
				});
			}
            m_ClippingShapes.emplace_back(clippingLevel);
		}
		clipSearch = clipSearch->parent();
	}
}
