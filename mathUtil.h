#pragma once

#include <cmath>
#include <ctime>
#include <cstdlib>

namespace util::math{
	//Linear interpolation by lerpFactor
	template<typename T1, typename T2>
	inline constexpr T1 lerp(const T1& t1, const T1& t2, const T2& factor) noexcept{
		return t1 + (t2 - t1) * factor;
	}

	//Clamping a value between min and max
	template<typename T>
	inline constexpr T clamp(const T& value, const T& min, const T& max) noexcept{
		if(value < min)
			return min;

		if(value > max)
			return max;

		return value;
	}

	//Simple wrapper around std::rand, just for consistency...
	inline void srand(unsigned int seed) noexcept{
		std::srand(seed);
	}

	//Initializes the random number generator using the current time as seed
	inline void init_random() noexcept{
		std::srand(static_cast<unsigned int>(std::time(nullptr)));
	}

	//Returns a random number between min and max
	inline float rand_range(float min, float max) noexcept{
		return lerp(min, max, std::rand() / static_cast<float>(RAND_MAX));
	}

	//Vector types

	struct Vec2f{
		float x = 0.0f, y = 0.0f;

		constexpr Vec2f operator+(Vec2f other) const noexcept{ return {x + other.x, y + other.y}; }
		constexpr Vec2f operator-(Vec2f other) const noexcept{ return {x - other.x, y - other.y}; }
		constexpr Vec2f operator*(Vec2f other) const noexcept{ return {x * other.x, y * other.y}; }
		constexpr Vec2f operator/(Vec2f other) const noexcept{ return {x / other.x, y / other.y}; }
		Vec2f& operator+=(Vec2f other) noexcept{ return *this = *this + other; }
		Vec2f& operator-=(Vec2f other) noexcept{ return *this = *this - other; }
		Vec2f& operator*=(Vec2f other) noexcept{ return *this = *this * other; }
		Vec2f& operator/=(Vec2f other) noexcept{ return *this = *this / other; }
		constexpr float dot(Vec2f other) const noexcept{ return x * other.x + y * other.y; }
		constexpr float length_sq() const noexcept{ return dot(*this); }
		float length() const{ return std::sqrtf(length_sq()); }
	};

	struct Vec3f{
		float x = 0.0f, y = 0.0f, z = 0.0f;

		constexpr Vec3f operator+(Vec3f other) const noexcept{ return {x + other.x, y + other.y, z + other.z}; }
		constexpr Vec3f operator-(Vec3f other) const noexcept{ return {x - other.x, y - other.y, z - other.z}; }
		constexpr Vec3f operator*(Vec3f other) const noexcept{ return {x * other.x, y * other.y, z * other.z}; }
		constexpr Vec3f operator/(Vec3f other) const noexcept{ return {x / other.x, y / other.y, z / other.z}; }
		Vec3f& operator+=(Vec3f other) noexcept{ return *this = *this + other; }
		Vec3f& operator-=(Vec3f other) noexcept{ return *this = *this - other; }
		Vec3f& operator*=(Vec3f other) noexcept{ return *this = *this * other; }
		Vec3f& operator/=(Vec3f other) noexcept{ return *this = *this / other; }
		constexpr float dot(Vec3f other) const noexcept{ return x * other.x + y * other.y + z * other.z; }
		constexpr float length_sq() const noexcept{ return dot(*this); }
		float length() const{ return std::sqrtf(length_sq()); }
		constexpr Vec3f cross(Vec3f other) const noexcept{ return {y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x}; }
	};

	struct Vec4f{
		float x = 0.0f, y = 0.0f, z = 0.0f, w = 1.0f;

		constexpr Vec4f operator+(Vec4f other) const noexcept{ return {x + other.x, y + other.y, z + other.z, w + other.w}; }
		constexpr Vec4f operator-(Vec4f other) const noexcept{ return {x - other.x, y - other.y, z - other.z, w - other.w}; }
		constexpr Vec4f operator*(Vec4f other) const noexcept{ return {x * other.x, y * other.y, z * other.z, w * other.w}; }
		constexpr Vec4f operator/(Vec4f other) const noexcept{ return {x / other.x, y / other.y, z / other.z, w / other.w}; }
		Vec4f& operator+=(Vec4f other) noexcept{ return *this = *this + other; }
		Vec4f& operator-=(Vec4f other) noexcept{ return *this = *this - other; }
		Vec4f& operator*=(Vec4f other) noexcept{ return *this = *this * other; }
		Vec4f& operator/=(Vec4f other) noexcept{ return *this = *this / other; }
	};
}