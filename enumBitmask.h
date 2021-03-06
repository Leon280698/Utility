#pragma once

#include <type_traits>

namespace util{
	template<typename EnumType>
	class EnumBitmask{
		static_assert(std::is_enum<EnumType>{});
	public:
		using UnderlyingType = typename std::underlying_type<EnumType>::type;

		constexpr EnumBitmask() = default;
		constexpr EnumBitmask(UnderlyingType mask) noexcept : mask{mask}{}
		constexpr EnumBitmask(EnumType initialValue) noexcept : EnumBitmask{static_cast<UnderlyingType>(initialValue)}{}

		constexpr UnderlyingType value() const noexcept{ return mask; }
		constexpr EnumBitmask operator&(EnumType flag) const noexcept{ return EnumBitmask{mask & static_cast<UnderlyingType>(flag)}; }
		constexpr EnumBitmask operator|(EnumType flag) const noexcept{ return EnumBitmask{mask | static_cast<UnderlyingType>(flag)}; }

		EnumBitmask operator&=(EnumType flag) noexcept{
			mask &= static_cast<UnderlyingType>(flag);

			return *this;
		}

		EnumBitmask operator|=(EnumType flag) noexcept{
			mask |= static_cast<UnderlyingType>(flag);

			return *this;
		}

		constexpr EnumBitmask operator&(EnumBitmask other) const noexcept{
			return EnumBitmask{mask & other.mask};
		}

		constexpr EnumBitmask operator|(EnumBitmask other) const noexcept{
			return EnumBitmask{mask | other.mask};
		}

		EnumBitmask operator&=(EnumBitmask other) noexcept{
			mask &= other.mask;

			return *this;
		}

		EnumBitmask operator|=(EnumBitmask other) noexcept{
			mask |= other.mask;

			return *this;
		}

		constexpr EnumBitmask operator~() const noexcept{ return EnumBitmask{~mask}; }
		constexpr operator UnderlyingType() const noexcept{ return mask; }

	private:
		UnderlyingType mask = 0;

		friend constexpr EnumBitmask operator&(EnumType e1, EnumBitmask e2) noexcept{ return e2 & e1; }
		friend constexpr EnumBitmask operator|(EnumType e1, EnumBitmask e2) noexcept{ return e2 | e1; }
	};
}

#define UTIL_DECLARE_ENUM_BITMASK_OPERATORS(enumType) inline constexpr util::EnumBitmask<enumType> operator&(enumType e1, enumType e2) noexcept{ \
														  return util::EnumBitmask<enumType>{e1} & e2; \
													  } \
													  inline constexpr util::EnumBitmask<enumType> operator|(enumType e1, enumType e2) noexcept{ \
														  return util::EnumBitmask<enumType>{e1} | e2; \
													  } \
													  inline constexpr util::EnumBitmask<enumType> operator~(enumType e) noexcept{ \
														  return ~util::EnumBitmask<enumType>{e}; \
													  } \
													  inline constexpr util::EnumBitmask<enumType> operator&(util::EnumBitmask<enumType>::UnderlyingType e1, enumType e2) noexcept{ \
														  return util::EnumBitmask<enumType>{e1} & e2; \
													  } \
													  inline constexpr util::EnumBitmask<enumType> operator|(util::EnumBitmask<enumType>::UnderlyingType e1, enumType e2) noexcept{ \
														  return util::EnumBitmask<enumType>{e1} | e2; \
													  } \
													  inline constexpr util::EnumBitmask<enumType>::UnderlyingType operator&=(util::EnumBitmask<enumType>::UnderlyingType& e1, enumType e2) noexcept{ \
														  return e1 &= static_cast<util::EnumBitmask<enumType>::UnderlyingType>(e2); \
													  } \
													  inline constexpr util::EnumBitmask<enumType>::UnderlyingType operator|=(util::EnumBitmask<enumType>::UnderlyingType& e1, enumType e2) noexcept{ \
														  return e1 |= static_cast<util::EnumBitmask<enumType>::UnderlyingType>(e2); \
													  }
