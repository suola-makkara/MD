#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <iostream>
#include <cmath>

const constexpr double PI = 3.14159265359;

class Vec3
{
public:
	constexpr Vec3() : x(0), y(0), z(0) { }
	constexpr Vec3(double v) : x(v), y(v), z(v) { }
	constexpr Vec3(double x, double y, double z) : x(x), y(y), z(z) { }
	constexpr Vec3(const Vec3& v) : x(v.x), y(v.y), z(v.z) { }

	constexpr Vec3 operator+(const Vec3& v) const { return Vec3(x + v.x, y + v.y, z + v.z); }
	constexpr Vec3 operator-(const Vec3& v) const { return Vec3(x - v.x, y - v.y, z - v.z); }
	constexpr Vec3 operator*(const Vec3& v) const { return Vec3(x * v.x, y * v.y, z * v.z); }
	constexpr Vec3 operator/(const Vec3& v) const { return Vec3(x / v.x, y / v.y, z / v.z); }
	inline Vec3 operator%(const Vec3& v) const { return Vec3(std::fmod(x, v.x), std::fmod(y, v.y), std::fmod(z, v.z)); }
	constexpr Vec3 operator+(double s) const { return Vec3(x + s, y + s, z + s); }
	constexpr Vec3 operator-(double s) const { return Vec3(x - s, y - s, z - s); }
	constexpr Vec3 operator*(double s) const { return Vec3(x * s, y * s, z * s); }
	constexpr Vec3 operator/(double s) const { return Vec3(x / s, y / s, z / s); }
	inline Vec3 operator%(double s) const { return Vec3(std::fmod(x, s), std::fmod(y, s), std::fmod(z, s)); }

	constexpr Vec3& operator+=(const Vec3& v) { x += v.x, y += v.y, z += v.z; return *this; }
	constexpr Vec3& operator-=(const Vec3& v) { x -= v.x, y -= v.y, z -= v.z; return *this; }
	constexpr Vec3& operator*=(const Vec3& v) { x *= v.x, y *= v.y, z *= v.z; return *this; }
	constexpr Vec3& operator/=(const Vec3& v) { x /= v.x, y /= v.y, z /= v.z; return *this; }
	inline Vec3& operator%=(const Vec3& v) { x = fmod(x, v.x), y = fmod(y, v.y), z = fmod(z, v.z); return *this; }
	constexpr Vec3& operator+=(double s) { x += s, y += s, z += s; return *this; }
	constexpr Vec3& operator-=(double s) { x -= s, y -= s, z -= s; return *this; }
	constexpr Vec3& operator*=(double s) { x *= s, y *= s, z *= s; return *this; }
	constexpr Vec3& operator/=(double s) { x /= s, y /= s, z /= s; return *this; }
	inline Vec3& operator%=(double s) { x = fmod(x, s), y = fmod(y, s), z = fmod(z, s); return *this; }

	constexpr double dot(const Vec3& v) const { return x * v.x + y * v.y + z * v.z; }
	constexpr Vec3 cross(const Vec3& v) const { return Vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }

	constexpr double norm2() const { return dot(*this); }
	inline double norm() const { return std::sqrt(norm2()); }
	inline Vec3 normalize() const { return *this * (1. / norm()); }

	constexpr Vec3 min(const Vec3& v) const { return Vec3(x < v.x ? x : v.x, y < v.y ? y : v.y, z < v.z ? z : v.z); }
	constexpr Vec3 max(const Vec3& v) const { return Vec3(x > v.x ? x : v.x, y > v.y ? y : v.y, z > v.z ? z : v.z); }

	constexpr Vec3 to(const Vec3& v) const { return v - *this; }

	constexpr double& operator[](unsigned int i) const { return ((double*)&(*this))[i]; }

	static inline Vec3 rand2D() { double r = (rand() % 10001) / 10000. * 2 * PI; return Vec3(std::cos(r), std::sin(r), 0.); }
	static inline Vec3 rand3D()
	{
		double r1 = (rand() % 10001) / 10000. * 2 * PI;
		double r2 = (double)((rand() % 10000) - 5000) / 5000.;
		double s = std::sqrt(1 - r2 * r2);
		return Vec3(s * std::cos(r1), s * std::sin(r1), r2);
	}

	double x, y, z;
};

static std::ostream& operator<<(std::ostream& out, const Vec3& v)
{
	out << v.x << " " << v.y << " " << v.z;
	return out;
}

static constexpr Vec3 operator+(double s, const Vec3& v) { return Vec3(s + v.x, s + v.y, s + v.z); }
static constexpr Vec3 operator-(double s, const Vec3& v) { return Vec3(s - v.x, s - v.y, s - v.z); }
static constexpr Vec3 operator*(double s, const Vec3& v) { return Vec3(s * v.x, s * v.y, s * v.z); }
static constexpr Vec3 operator/(double s, const Vec3& v) { return Vec3(s / v.x, s / v.y, s / v.z); }

#endif // VECTOR_HPP