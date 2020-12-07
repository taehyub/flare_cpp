#ifndef _FLARE_ACTOR_FLAGS_HPP_
#define _FLARE_ACTOR_FLAGS_HPP_

namespace flare
{
	enum class ActorFlags : unsigned short
	{
		IsDrawOrderDirty = 1 << 0,
		IsDirty = 1 << 1,
		//FIXME::plase remove this for flutter sync, In flutter Isinstance is deprecated,
		//but I leave IsInstance enum value here for the legacy codes.
		//IsInstance is changed to 1<<2 to avoid ABI break.
		IsInstance = 1 << 2,
		//
	};

	inline constexpr ActorFlags operator&(ActorFlags x, ActorFlags y)
	{
		return static_cast<ActorFlags>(static_cast<unsigned short>(x) & static_cast<unsigned short>(y));
	}

	inline constexpr ActorFlags operator|(ActorFlags x, ActorFlags y)
	{
		return static_cast<ActorFlags>(static_cast<unsigned short>(x) | static_cast<unsigned short>(y));
	}

	inline constexpr ActorFlags operator^(ActorFlags x, ActorFlags y)
	{
		return static_cast<ActorFlags>(static_cast<unsigned short>(x) ^ static_cast<unsigned short>(y));
	}

	inline constexpr ActorFlags operator~(ActorFlags x)
	{
		return static_cast<ActorFlags>(~static_cast<unsigned short>(x));
	}

	inline ActorFlags& operator&=(ActorFlags& x, ActorFlags y)
	{
		x = x & y;
		return x;
	}

	inline ActorFlags& operator|=(ActorFlags& x, ActorFlags y)
	{
		x = x | y;
		return x;
	}

	inline ActorFlags& operator^=(ActorFlags& x, ActorFlags y)
	{
		x = x ^ y;
		return x;
	}
} // namespace flare
#endif
