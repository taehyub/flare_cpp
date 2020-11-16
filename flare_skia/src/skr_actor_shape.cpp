#include "flare_skia/skr_actor_shape.hpp"
#include "flare/paint/actor_fill.hpp"
#include "flare/paint/actor_stroke.hpp"
#include "flare/path/actor_base_path.hpp"
#include "flare_skia/skr_actor_base_path.hpp"
#include "flare_skia/to_skia.hpp"
#include "flare_skia/skr_stroke.hpp"
#include "flare_skia/skr_fill.hpp"
#include <thorvg.h>
#include <vector>

using namespace flare;

SkrActorShape::SkrActorShape() : m_IsValid(false), m_Fill(nullptr), m_Stroke(nullptr) {}

void SkrActorShape::onBlendModeChanged(BlendMode from, BlendMode to)
{
	// We mark the paints dirty here so that their update can change the blend modes.
	for (auto stroke : m_Strokes)
	{
		stroke->markPaintDirty();
	}
	for (auto fill : m_Fills)
	{
		fill->markPaintDirty();
	}
}

void SkrActorShape::onRenderOpacityChanged()
{
	// We mark the paints dirty here so that their update can change the blend modes.
	for (auto stroke : m_Strokes)
	{
		stroke->markPaintDirty();
	}
	for (auto fill : m_Fills)
	{
		fill->markPaintDirty();
	}
}

void SkrActorShape::onChildrenChanged()
{
	m_SubPaths.clear();
	auto kids = children();
	for (auto child : kids)
	{
		SkrActorBasePath* path = dynamic_cast<SkrActorBasePath*>(child);
		if (path == nullptr)
		{
			continue;
		}
		m_SubPaths.push_back(path);
	}
}

void SkrActorShape::invalidateDrawable() { m_IsValid = false; }

SkPath& SkrActorShape::path()
{
    if (!m_IsValid)
	{
		m_IsValid = true;
		m_Path.rewind();

		for (auto path : m_SubPaths)
		{
			const Mat2D& pathTransform = path->basePath()->pathTransform();
			m_Path.addPath(path->path(), ToSkia::convert(pathTransform));
		}
	}
    return m_Path;
}

void SkrActorShape::draw(SkCanvas* canvas)
{
	if (!doesDraw())
	{
		return;
	}

    SkPath& renderPath = path();

	canvas->save();

    auto clips = clippingShapes();
    for(auto& clipLayer : clips)
    {
        if(clipLayer.size() == 1)
        {
            canvas->clipPath(clipLayer[0]->path(), true);
        }
        else
        {
            SkPath complexClip;
            for(auto& clipShape : clipLayer)
            {
                complexClip.addPath(clipShape->path());
            }
            canvas->clipPath(complexClip, true);
        }
    }

    if(m_Fill != nullptr)
    {
        m_Fill->paint(canvas, renderPath);
    }
    if(m_Stroke != nullptr)
    {
        m_Stroke->paint(canvas, renderPath);
    }

	canvas->restore();
}

void SkrActorShape::onStrokesChanged()
{
    if(m_Strokes.size() == 0)
    {
        m_Stroke = nullptr;
    }
    else
    {
        m_Stroke = dynamic_cast<SkrStroke*>(m_Strokes[0]);
    }
}

void SkrActorShape::onFillsChanged()
{
    if(m_Fills.size() == 0)
    {
        m_Fill = nullptr;
    }
    else
    {
        m_Fill = dynamic_cast<SkrFill*>(m_Fills[0]);
    }
}

// Flare - Thorvg
TvgActorShape::TvgActorShape() : m_IsValid(false), m_Fill(nullptr), m_Stroke(nullptr) {
	this->pushed = false;
	this->m_Path = tvg::Shape::gen().release();
}

void TvgActorShape::onBlendModeChanged(BlendMode from, BlendMode to)
{
	// We mark the paints dirty here so that their update can change the blend modes.
	for (auto stroke : m_Strokes)
	{
		stroke->markPaintDirty();
	}
	for (auto fill : m_Fills)
	{
		fill->markPaintDirty();
	}
}

void TvgActorShape::onRenderOpacityChanged()
{
	// We mark the paints dirty here so that their update can change the blend modes.
	for (auto stroke : m_Strokes)
	{
		stroke->markPaintDirty();
	}
	for (auto fill : m_Fills)
	{
		fill->markPaintDirty();
	}
}

void TvgActorShape::onChildrenChanged()
{
	m_SubPaths.clear();
	auto kids = children();
	for (auto child : kids)
	{
		TvgActorBasePath* path = dynamic_cast<TvgActorBasePath*>(child);
		if (path == nullptr)
		{
			continue;
		}
		m_SubPaths.push_back(path);
	}
}

void TvgActorShape::invalidateDrawable() { m_IsValid = false; }

void TvgActorShape::path(tvg::Canvas *canvas, bool pushed)
{
    if (!m_IsValid)
	{
		m_IsValid = true;
		m_Path->reset();
		for (auto path : m_SubPaths)
			path->path(m_Path);

		if (!pushed)
			canvas->push(std::unique_ptr<tvg::Shape>(m_Path));
	}
}

void TvgActorShape::draw(tvg::Canvas *canvas)
{
	if (!doesDraw())
	{
		return;
	}

	path(canvas, this->pushed);

	//FIXME: This transform code is preventing clip source object is disappeared.
	//       Without this transform code, clip source object will be disappeared.
	//       To fix this behavior, ThorVG should be modified and verified for this.
	tvg::Matrix m = {1, 0, 0, 0, 1, 0, 0, 0, 1};
	m_Path->transform(m);

	auto clips = clippingShapes();
	this->clipPaths.clear();

	for(auto& clipLayer : clips)
	{
		if (clipLayer.size() == 1)
		{
			this->clipPaths.push_back(clipLayer[0]->m_Path);
		}
		else
		{
			for(auto& clipShape : clipLayer)
			{
				clipShape->path(canvas, clipShape->pushed);
				this->clipPaths.push_back(clipShape->m_Path);
			}
		}
	}

	if (m_Fill != nullptr)
		m_Fill->paint(canvas, m_Path);
	if (m_Stroke != nullptr)
		m_Stroke->paint(canvas, m_Path);

	if (this->clipPaths.size() != 0)
	{
		auto copy = std::unique_ptr<tvg::Shape>(static_cast<tvg::Shape*>(this->clipPaths[0]->duplicate()));
	        m_Path->composite(std::move(copy), tvg::CompositeMethod::ClipPath);
	}

	this->pushed = true;
}

void TvgActorShape::onStrokesChanged()
{
    if(m_Strokes.size() == 0)
    {
        m_Stroke = nullptr;
    }
    else
    {
        m_Stroke = dynamic_cast<TvgStroke*>(m_Strokes[0]);
    }
}

void TvgActorShape::onFillsChanged()
{
    if(m_Fills.size() == 0)
    {
        m_Fill = nullptr;
    }
    else
    {
        m_Fill = dynamic_cast<TvgFill*>(m_Fills[0]);
    }
}


