#include "flare_skia/skr_actor_shape.hpp"
#include "flare/paint/actor_fill.hpp"
#include "flare/paint/actor_stroke.hpp"
#include "flare/path/actor_base_path.hpp"
#include "flare_skia/skr_actor_base_path.hpp"
#include "flare_skia/to_skia.hpp"
#include "flare_skia/skr_stroke.hpp"
#include "flare_skia/skr_fill.hpp"
#include <thorvg.h>

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

void TvgActorShape::path(tvg::Canvas *canvas)
{
    if (!m_IsValid)
	{
		m_IsValid = true;
		int i=0;
		for (auto path : m_SubPaths)
		{
			Mat2D pathTransform = path->basePath()->pathTransform();
			this->m_Paths.push_back(tvg::Shape::gen().release());
			path->path(canvas, m_Paths[i]);
			canvas->push(std::unique_ptr<tvg::Shape>(m_Paths[i++]));
		}
	}
}

void TvgActorShape::draw(tvg::Canvas *canvas)
{
	if (!doesDraw())
	{
		return;
	}

	if (!this->pushed)
	{
		path(canvas);
	}

	Mat2D pathTransform;
	int i = 0;
	for (auto path : m_SubPaths)
	{
		pathTransform = path->basePath()->pathTransform();
		tvg::Matrix m = {1, 0, 0, 0, 1, 0, 0, 0, 1};
		m.e11 = pathTransform[0];
		m.e12 = pathTransform[2];
		m.e13 = pathTransform[4];
		m.e21 = pathTransform[1];
		m.e22 = pathTransform[3];
		m.e23 = pathTransform[5];
		m_Paths[i++]->transform(m);
	}

	//FIXME: Clip is not supported yet
	/*
	auto clips = clippingShapes();
	for(auto& clipLayer : clips)
	{
		if (clipLayer.size() == 1)
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
	*/

	i = 0;
	for (auto path : m_SubPaths)
	{
		if (m_Fill != nullptr)
			m_Fill->paint(canvas, m_Paths[i]);

		if (m_Stroke != nullptr)
			m_Stroke->paint(canvas, m_Paths[i]);

		canvas->update(m_Paths[i++]);
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


