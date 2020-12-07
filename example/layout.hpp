#ifndef _FLARE_EXAMPLE_LAYOUT_HPP_
#define _FLARE_EXAMPLE_LAYOUT_HPP_

#include "aabb.hpp"
#include <cmath>

namespace flare
{
	enum class Fit
	{
		fill,
		contain,
		cover,
		fitWidth,
		fitHeight,
		none,
		scaleDown
	};

	class Alignment
	{
		float m_X, m_Y;

	public:
		Alignment(float x, float y) : m_X(x), m_Y(y) {}

		float x() const { return m_X; }
		float y() const { return m_Y; }

		static const Alignment topLeft()
		{
			static Alignment align(-1.0f, -1.0f);
			return align;
		}

		static const Alignment topCenter()
		{
			static Alignment align(0.0f, -1.0f);
			return align;
		}

		static const Alignment topRight()
		{
			static Alignment align(1.0f, -1.0f);
			return align;
		}

		static const Alignment centerLeft()
		{
			static Alignment align(-1.0f, 0.0f);
			return align;
		}

		static const Alignment center()
		{
			static Alignment align(0.0f, 0.0f);
			return align;
		}

		static const Alignment centerRight()
		{
			static Alignment align(1.0f, 0.0f);
			return align;
		}

		static const Alignment bottomLeft()
		{
			static Alignment align(-1.0f, 1.0f);
			return align;
		}

		static const Alignment bottomCenter()
		{
			static Alignment align(0.0f, 1.0f);
			return align;
		}

		static const Alignment bottomRight()
		{
			static Alignment align(1.0f, 1.0f);
			return align;
		}

	};
} // namespace flare
#endif
