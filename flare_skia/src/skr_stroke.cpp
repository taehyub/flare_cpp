#include "flare_skia/skr_stroke.hpp"
#include "flare/paint/actor_stroke.hpp"
#include "flare_skia/to_skia.hpp"
#include <cassert>

using namespace std;
using namespace flare;

struct Bezier
{
    tvg::Point start;
    tvg::Point ctrl1;
    tvg::Point ctrl2;
    tvg::Point end;
};

static float _lineLength(const tvg::Point& pt1, const tvg::Point& pt2)
{
    /* approximate sqrt(x*x + y*y) using alpha max plus beta min algorithm.
       With alpha = 1, beta = 3/8, giving results with the largest error less
       than 7% compared to the exact value. */
    tvg::Point diff = {pt2.x - pt1.x, pt2.y - pt1.y};
    if (diff.x < 0) diff.x = -diff.x;
    if (diff.y < 0) diff.y = -diff.y;
    return (diff.x > diff.y) ? (diff.x + diff.y * 0.375f) : (diff.y + diff.x * 0.375f);
}

void bezSplit(const Bezier&cur, Bezier& left, Bezier& right)
{
    auto c = (cur.ctrl1.x + cur.ctrl2.x) * 0.5f;
    left.ctrl1.x = (cur.start.x + cur.ctrl1.x) * 0.5f;
    right.ctrl2.x = (cur.ctrl2.x + cur.end.x) * 0.5f;
    left.start.x = cur.start.x;
    right.end.x = cur.end.x;
    left.ctrl2.x = (left.ctrl1.x + c) * 0.5f;
    right.ctrl1.x = (right.ctrl2.x + c) * 0.5f;
    left.end.x = right.start.x = (left.ctrl2.x + right.ctrl1.x) * 0.5f;

    c = (cur.ctrl1.y + cur.ctrl2.y) * 0.5f;
    left.ctrl1.y = (cur.start.y + cur.ctrl1.y) * 0.5f;
    right.ctrl2.y = (cur.ctrl2.y + cur.end.y) * 0.5f;
    left.start.y = cur.start.y;
    right.end.y = cur.end.y;
    left.ctrl2.y = (left.ctrl1.y + c) * 0.5f;
    right.ctrl1.y = (right.ctrl2.y + c) * 0.5f;
    left.end.y = right.start.y = (left.ctrl2.y + right.ctrl1.y) * 0.5f;
}


float bezLength(const Bezier& cur)
{
    Bezier left, right;
    auto len = _lineLength(cur.start, cur.ctrl1) + _lineLength(cur.ctrl1, cur.ctrl2) + _lineLength(cur.ctrl2, cur.end);
    auto chord = _lineLength(cur.start, cur.end);

    if (fabs(len - chord) > FLT_EPSILON) {
        bezSplit(cur, left, right);
        return bezLength(left) + bezLength(right);
    }
    return len;
}

void bezSplitLeft(Bezier& cur, float at, Bezier& left)
{
    left.start = cur.start;

    left.ctrl1.x = cur.start.x + at * (cur.ctrl1.x - cur.start.x);
    left.ctrl1.y = cur.start.y + at * (cur.ctrl1.y - cur.start.y);

    left.ctrl2.x = cur.ctrl1.x + at * (cur.ctrl2.x - cur.ctrl1.x); // temporary holding spot
    left.ctrl2.y = cur.ctrl1.y + at * (cur.ctrl2.y - cur.ctrl1.y); // temporary holding spot

    cur.ctrl2.x = cur.ctrl2.x + at * (cur.end.x - cur.ctrl2.x);
    cur.ctrl2.y = cur.ctrl2.y + at * (cur.end.y - cur.ctrl2.y);

    cur.ctrl1.x = left.ctrl2.x + at * (cur.ctrl2.x - left.ctrl2.x);
    cur.ctrl1.y = left.ctrl2.y + at * (cur.ctrl2.y - left.ctrl2.y);

    left.ctrl2.x = left.ctrl1.x + at * (left.ctrl2.x - left.ctrl1.x);
    left.ctrl2.y = left.ctrl1.y + at * (left.ctrl2.y - left.ctrl1.y);

    left.end.x = cur.start.x = left.ctrl2.x + at * (cur.ctrl1.x - left.ctrl2.x);
    left.end.y = cur.start.y = left.ctrl2.y + at * (cur.ctrl1.y - left.ctrl2.y);
}

float bezOffsetAt(const Bezier& bz, float offset)
{
    float len = bezLength(bz);
    float at = offset / len;

    if (offset <= 0)
      return 0.0;
    if (offset >= len)
      return 1.0;

    while (true) {
        auto right = bz;
        Bezier left;
        bezSplitLeft(right, at, left);
        float len2 = bezLength(left);

        // offset length diff is under 1 pixel, finish this loop
        if (fabs(len2 - offset) < 1) break;

        if (len2 < offset)
            at +=  at * 0.1;
        else
            at -= at * 0.1;
    }

    return at;
}

void lineSplitAt(float at, const tvg::Point& from, const tvg::Point& to, tvg::Point& left_from, tvg::Point& left_to, tvg::Point& right_from, tvg::Point& right_to)
{
   tvg::Point atPoint;
   atPoint.x = from.x + (to.x - from.x) * at;
   atPoint.y = from.y + (to.y - from.y) * at;

   left_from = from;
   left_to = atPoint;

   right_from = atPoint;
   right_to = to;
}

void bezSplitAt(const Bezier& cur, float at, Bezier& left, Bezier& right)
{
    right = cur;
    auto t = bezOffsetAt(right, at);
    bezSplitLeft(right, t, left);
}

float GetShapeLength(tvg::Shape *shape)
{
    const tvg::PathCommand* cmds = nullptr;
    auto cmdCnt = shape->pathCommands(&cmds);

    const tvg::Point* pts = nullptr;
    auto ptsCnt = shape->pathCoords(&pts);

    Bezier cur;
    tvg::Point before;
    float ShapeLength = 0.0;

    for (int i = 0; i < cmdCnt; i++) {
        switch(*cmds) {
           case tvg::PathCommand::Close: {
                ++pts;
                break;
            }
            case tvg::PathCommand::MoveTo: {
                before = *pts;
                ++pts;
                break;
            }
            case tvg::PathCommand::LineTo: {
                tvg::Point from = before;
                tvg::Point to = *pts;
                float length = _lineLength(from, to);
                ShapeLength += length;
                before = *pts;
                ++pts;
                break;
            }
            case tvg::PathCommand::CubicTo: {
                cur.start = before;
                cur.ctrl1 = *pts;
                cur.ctrl2 = *(pts + 1);
                cur.end = *(pts + 2);
                float length = bezLength(cur);
                ShapeLength += length;
                pts += 3;
                before = cur.end;
                break;
            }
        }
        cmds++;
    }

   return ShapeLength;
}

float _trimPath(tvg::Shape *shape, float start, float end, bool complement, bool isSequential,
                vector<tvg::PathCommand> &vecPathCmds, vector<tvg::Point> &vecPoints)
{
    float totalLength = GetShapeLength(shape);

    float startOffset = start * totalLength;
    float endOffset = end * totalLength;

    const tvg::PathCommand* cmds = nullptr;
    auto cmdCnt = shape->pathCommands(&cmds);

    const tvg::Point* pts = nullptr;
    auto ptsCnt = shape->pathCoords(&pts);

    Bezier cur, left, right;
    tvg::Point before;
    float ShapeLength = 0.0;
    float CurLength = 0.0;
    bool startFound = false;
    bool endFound = false;

    for (int i=0; i < cmdCnt; i++) {
        switch(*cmds) {
           case tvg::PathCommand::Close: {
                ++pts;
                break;
            }
            case tvg::PathCommand::MoveTo: {
                before = *pts;
                ++pts;
                break;
            }
            case tvg::PathCommand::LineTo: {

                float length = _lineLength(before, *pts);
                ShapeLength += length;

                int modified = false;
                tvg::Point left[2];
                tvg::Point right[2];

                if (!startFound && ShapeLength >= startOffset) {
                  startFound = true;
                  modified = true;
                  float at = (startOffset - CurLength) / (ShapeLength - CurLength);
                  lineSplitAt(at, before, *pts, left[0], left[1], right[0], right[1]);
                  vecPathCmds.push_back(tvg::PathCommand::MoveTo);
                  vecPoints.push_back(right[0]);
                  vecPathCmds.push_back(tvg::PathCommand::LineTo);
                  vecPoints.push_back(right[1]);
                }

                if (!endFound && ShapeLength >= endOffset) {
                  endFound = true;
                  if (modified) {
                    float at = (endOffset - startOffset) / (ShapeLength - CurLength - (startOffset - CurLength));
                    lineSplitAt(at, right[0], right[1], left[0], left[1], right[0], right[1]);
		    vecPoints[vecPoints.size()-1] = left[1];
                    vecPoints[vecPoints.size()-2] = left[0];
                  }
                 else {
                    float at = (endOffset - CurLength) / (ShapeLength - CurLength);
                    lineSplitAt(at, before, *pts, left[0], left[1], right[0], right[1]);
                    vecPathCmds.push_back(tvg::PathCommand::LineTo);
                    vecPoints.push_back(left[1]);
                 }
                   modified = true;
                   return 0;
                }

		if (!modified && startFound) {
                   vecPathCmds.push_back(tvg::PathCommand::LineTo);
                   vecPoints.push_back(*pts);
                }

                before = *pts;
                ++pts;
                CurLength = ShapeLength;
                break;
            }
            case tvg::PathCommand::CubicTo: {
                cur.start = before;
                cur.ctrl1 = *pts;
                cur.ctrl2 = *(pts + 1);
                cur.end = *(pts + 2);
                float length = bezLength(cur);
                ShapeLength += length;

                int modified = false;

		if (!startFound && ShapeLength >= startOffset) {
                   startFound = true;
                   modified = true;

                   float at = (startOffset - CurLength);
                   bezSplitAt(cur, at, left, right);

                   vecPathCmds.push_back(tvg::PathCommand::MoveTo);
                   vecPoints.push_back(right.start);
                   vecPathCmds.push_back(tvg::PathCommand::CubicTo);
                   vecPoints.push_back(right.ctrl1);
                   vecPoints.push_back(right.ctrl2);
                   vecPoints.push_back(right.end);
                }

                if (!endFound && ShapeLength >= endOffset) {
                   endFound = true;

                   if (modified) {
                      cur = right;
                      float at = (endOffset - startOffset);
                      bezSplitAt(cur, at, left, right);
                      vecPoints[vecPoints.size()-1] = left.end;
                      vecPoints[vecPoints.size()-2] = left.ctrl2;
                      vecPoints[vecPoints.size()-3] = left.ctrl1;
                      vecPoints[vecPoints.size()-4] = left.start;
                   }
                   else {
                      float at = (endOffset - CurLength);
                      bezSplitAt(cur, at, left, right);
                      vecPathCmds.push_back(tvg::PathCommand::CubicTo);
                      vecPoints.push_back(left.ctrl1);
                      vecPoints.push_back(left.ctrl2);
                      vecPoints.push_back(left.end);
                   }

                   modified = true;
                   return 0;
                }

		if (!modified && startFound) {
                   vecPathCmds.push_back(tvg::PathCommand::CubicTo);
                   vecPoints.push_back(cur.ctrl1);
                   vecPoints.push_back(cur.ctrl2);
                   vecPoints.push_back(cur.end);
                }

                pts += 3;
                CurLength = ShapeLength;
                before = cur.end;
                break;
            }
        }
        cmds++;
    }

    return 0;
}

void SkrStroke::initializeGraphics()
{
    m_ActorStroke = dynamic_cast<ActorStroke*>(this);
    m_Paint.setStyle(SkPaint::Style::kStroke_Style);
    m_Paint.setAntiAlias(true);
	assert(m_ActorStroke != nullptr);
}

void SkrStroke::updatePaint()
{
	m_Paint.setStrokeWidth(m_ActorStroke->width());
	m_Paint.setStrokeCap(ToSkia::convert(m_ActorStroke->cap()));
	m_Paint.setStrokeJoin(ToSkia::convert(m_ActorStroke->join()));
}

void SkrStroke::paint(SkCanvas* canvas, SkPath& path)
{
	if(m_ActorStroke->width() == 0)
	{
		return;
	}
	m_ActorStroke->validatePaint();
	if(m_ActorStroke->trimPath() != TrimPath::Off)
	{
		// TODO: Trim Path
	}
	canvas->drawPath(path, m_Paint);
}

tvg::StrokeCap GetTvgStrokeCap(flare::StrokeCap cap)
{
	switch (cap)
	{
		case flare::StrokeCap::Butt:
			return tvg::StrokeCap::Butt;
		case flare::StrokeCap::Round:
			return tvg::StrokeCap::Round;
		case flare::StrokeCap::Square:
			return tvg::StrokeCap::Square;
	}
	return tvg::StrokeCap::Square;
}

tvg::StrokeJoin GetTvgStrokeJoin(flare::StrokeJoin join)
{
	switch (join)
	{
		case flare::StrokeJoin::Round:
			return tvg::StrokeJoin::Round;
		case flare::StrokeJoin::Bevel:
			return tvg::StrokeJoin::Bevel;
		case flare::StrokeJoin::Miter:
			return tvg::StrokeJoin::Miter;
	}
	return tvg::StrokeJoin::Bevel;
}

void TvgStroke::initializeGraphics()
{
	m_ActorStroke = dynamic_cast<ActorStroke*>(this);
	m_Paint.setStyle(SkPaint::Style::kStroke_Style);
	m_Paint.setAntiAlias(true);
	assert(m_ActorStroke != nullptr);
}

void TvgStroke::updatePaint()
{
	m_Paint.setStrokeWidth(m_ActorStroke->width());
	m_Paint.setStrokeCap(ToSkia::convert(m_ActorStroke->cap()));
	m_Paint.setStrokeJoin(ToSkia::convert(m_ActorStroke->join()));
}

void TvgStroke::paint(tvg::Canvas *canvas, tvg::Shape *path)
{
	bool isTrimed = false;
	if(m_ActorStroke->width() == 0)
	{
		return;
	}
	m_ActorStroke->validatePaint();
	if(m_ActorStroke->trimPath() != TrimPath::Off)
	{
		isTrimed = true;
		bool isSequential = (m_ActorStroke->trimPath() == TrimPath::Sequential)? true : false;
		float start = m_ActorStroke->trimStart();
		float end = m_ActorStroke->trimEnd();
		float offset = m_ActorStroke->trimOffset();
		bool inverted = (start > end)? true : false;
		if ( fabs(start - end) != 1.0)
		{
			start = (start + offset);
			end = (end + offset);

			if (start < 0)
				start += 1.0;
			if (end < 0)
				end += 1.0;
			if (inverted)
			{
				float swap = end;
				end = start;
				start = swap;
			}

                        if (m_OriginShape == NULL) {
                           const tvg::PathCommand* cmds = nullptr;
                           const tvg::Point* pts = nullptr;
                           int cmdCnt = path->pathCommands(&cmds);
                           int ptsCnt = path->pathCoords(&pts);

                           m_OriginShape = tvg::Shape::gen().release();
			   m_OriginShape->appendPath(cmds, cmdCnt, pts, ptsCnt);
                        }

			m_TrimPathCmds.clear();
			m_TrimPathPoints.clear();

			if (end >= start)
				_trimPath(m_OriginShape, start, end, false, isSequential, m_TrimPathCmds, m_TrimPathPoints);
			else
				_trimPath(m_OriginShape, end, start, true, isSequential, m_TrimPathCmds, m_TrimPathPoints);


                        tvg::PathCommand *render_cmds = (tvg::PathCommand*)calloc(m_TrimPathCmds.size(), sizeof(tvg::PathCommand));
                        tvg::Point *render_pts = (tvg::Point*)calloc(m_TrimPathPoints.size(), sizeof(tvg::Point));

			for (int i=0; i<m_TrimPathCmds.size(); i++)
				render_cmds[i] = m_TrimPathCmds[i];

			for (int i=0; i<m_TrimPathPoints.size(); i++)
				render_pts[i] = m_TrimPathPoints[i];

                        path->reset();
                        path->appendPath(render_cmds, m_TrimPathCmds.size(), render_pts, m_TrimPathPoints.size());

			canvas->update(path);

			free(render_cmds);
			free(render_pts);
		}
	}

	int r, g, b, a;
	r = this->m_color[0] * 255;
	g = this->m_color[1] * 255;
	b = this->m_color[2] * 255;
	a = this->m_color[3] * 255;

	path->stroke(r, g, b, a);
	path->stroke(m_ActorStroke->width());
	path->stroke(GetTvgStrokeCap(m_ActorStroke->cap()));
	path->stroke(GetTvgStrokeJoin(m_ActorStroke->join()));

	if (!isTrimed)
		canvas->update(path);
}
