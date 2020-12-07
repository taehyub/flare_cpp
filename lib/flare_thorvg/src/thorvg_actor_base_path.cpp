#include "flare_thorvg/thorvg_actor_base_path.hpp"
#include "flare/path/actor_base_path.hpp"
#include <cmath>

using namespace flare;

//Flare - ThorVg
Vec2D applyTransform(Vec2D &vec, Mat2D &mat)
{
	tvg::Matrix m = {1, 0, 0, 0, 1, 0, 0, 0, 1};
	m.e11 = mat[0];
	m.e12 = mat[2];
	m.e13 = mat[4];
	m.e21 = mat[1];
	m.e22 = mat[3];
	m.e23 = mat[5];

	Vec2D ret;
	ret[0] = round(vec[0] * m.e11 + vec[1] * m.e12 + m.e13);
	ret[1] = round(vec[0] * m.e21 + vec[1] * m.e22 + m.e23);

	return ret;
}

void TvgActorBasePath::path(tvg::Shape *tvgPath)
{
//FIXME: The following line is commented to apply transform for each shapes,
//       because the transform is cacluated in this function.
//       With the following line, the transform will not be calculated.
//       So I commented the following line ,but the line should be restore later
//	 for performance.
//	if (!m_IsPathValid)
//	{
		updatePath(tvgPath);
		m_IsPathValid = true;
//	}
}

TvgActorBasePath::TvgActorBasePath(ActorBasePath* path) : m_BasePath(path), m_IsPathValid(false) {}

void TvgActorBasePath::updatePath(tvg::Shape *tvgPath)
{
	Mat2D pathTransform = this->basePath()->pathTransform();
	const std::vector<PathPoint*>& pts = m_BasePath->deformedPoints();

	if (pts.size() != 0)
	{
		std::vector<PathPoint*> renderPoints;
		std::vector<PathPoint*> cleanup;
		int pl = pts.size();

		const float arcConstant = 0.55f;
		const float iarcConstant = 1.0f - arcConstant;
		bool isClosed = m_BasePath->isClosed();
		PathPoint* previous = isClosed ? pts[pl - 1] : nullptr;
		for (int i = 0; i < pl; i++)
		{
			PathPoint* point = pts[i];
			switch (point->type())
			{
				case PointType::Straight:
				{
					StraightPathPoint* straightPoint = reinterpret_cast<StraightPathPoint*>(point);
					float radius = straightPoint->radius();
					if (radius > 0.0f)
					{
						if (!isClosed && (i == 0 || i == pl - 1))
						{
							renderPoints.push_back(point);
							previous = point;
						}
						else
						{
							PathPoint* next = pts[(i + 1) % pl];
							const Vec2D& prevPoint = previous->type() == PointType::Straight
							                             ? previous->translation()
							                             : reinterpret_cast<CubicPathPoint*>(previous)->outPoint();
							const Vec2D& nextPoint = next->type() == PointType::Straight
							                             ? next->translation()
							                             : reinterpret_cast<CubicPathPoint*>(next)->inPoint();
							const Vec2D& pos = point->translation();

							Vec2D toPrev;
							Vec2D::subtract(toPrev, prevPoint, pos);
							float toPrevLength = Vec2D::length(toPrev);
							toPrev[0] /= toPrevLength;
							toPrev[1] /= toPrevLength;

							Vec2D toNext;
							Vec2D::subtract(toNext, nextPoint, pos);
							float toNextLength = Vec2D::length(toNext);
							toNext[0] /= toNextLength;
							toNext[1] /= toNextLength;

							float renderRadius = std::min(toPrevLength, std::min(toNextLength, radius));

							{
								Vec2D translation, outPoint;
								Vec2D::scaleAndAdd(translation, pos, toPrev, renderRadius);
								Vec2D::scaleAndAdd(outPoint, pos, toPrev, iarcConstant * renderRadius);

								CubicPathPoint* newPoint = new CubicPathPoint(translation, translation, outPoint);
								cleanup.push_back(newPoint);
								renderPoints.push_back(newPoint);
							}

							{
								Vec2D translation, inPoint;
								Vec2D::scaleAndAdd(translation, pos, toNext, renderRadius);
								Vec2D::scaleAndAdd(inPoint, pos, toNext, iarcConstant * renderRadius);

								CubicPathPoint* newPoint = new CubicPathPoint(translation, inPoint, translation);
								cleanup.push_back(newPoint);
								renderPoints.push_back(newPoint);

								previous = newPoint;
							}
						}
					}
					else
					{
						renderPoints.push_back(point);
						previous = point;
					}
					break;
				}
				default:
					renderPoints.push_back(point);
					previous = point;
					break;
			}
		}

		PathPoint* firstPoint = renderPoints[0];
		Vec2D& translation = firstPoint->translation();

		Vec2D transformed_t = applyTransform(translation, pathTransform);

		tvgPath->moveTo(transformed_t[0], transformed_t[1]);
		for (int i = 0, l = isClosed ? renderPoints.size() : renderPoints.size() - 1, pl = renderPoints.size(); i < l;
		     i++)
		{
			PathPoint* point = renderPoints[i];
			PathPoint* nextPoint = renderPoints[(i + 1) % pl];
			Vec2D* cin = nextPoint->type() != PointType::Straight ? &(reinterpret_cast<CubicPathPoint*>(nextPoint)->inPoint()) : nullptr;
			Vec2D* cout = point->type() != PointType::Straight ? &(reinterpret_cast<CubicPathPoint*>(point)->outPoint()) : nullptr;

			if (cin == nullptr && cout == nullptr)
			{
				Vec2D& translation = nextPoint->translation();
				Vec2D transformed_t = applyTransform(translation, pathTransform);
				tvgPath->lineTo(transformed_t[0], transformed_t[1]);
			}
			else
			{
				if(cout == nullptr)
				{
					cout = &point->translation();
				}
				if(cin == nullptr)
				{
					cin = &nextPoint->translation();
				}

				Vec2D& fcin = *cin;
				Vec2D& fcout = *cout;
				Vec2D& translation = nextPoint->translation();

				Vec2D transformed_cin = applyTransform(fcin, pathTransform);
				Vec2D transformed_cout = applyTransform(fcout, pathTransform);
				Vec2D transformed_t = applyTransform(translation, pathTransform);
				tvgPath->cubicTo(transformed_cout[0], transformed_cout[1], transformed_cin[0], transformed_cin[1], transformed_t[0], transformed_t[1]);
			}
		}

		if (isClosed)
		{
			tvgPath->close();
		}
		for (PathPoint* point : cleanup)
		{
			delete point;
		}
	}
}
