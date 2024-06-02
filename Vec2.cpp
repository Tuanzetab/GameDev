#include "Vec2.h"


Vec2::Vec2()
{}

Vec2::Vec2(float xIn, float yIn) :
	x(xIn), y(yIn)
{}

std::ostream& operator<<(std::ostream& os, Vec2& aVec2)
{
	os << "{" << aVec2.x << ", " << aVec2.y << "}";
	return os;
}

bool Vec2::operator==(const Vec2& aVec2) const
{
	return x == aVec2.x && y == aVec2.y;
}

bool Vec2::operator!=(const Vec2& aVec2) const
{
	return !(*this == aVec2);
}

Vec2 Vec2::operator+(const Vec2& aVec2) const
{
	return Vec2(x + aVec2.x, y + aVec2.y);
}

Vec2 Vec2::operator+=(const Vec2& aVec2)
{
	x += aVec2.x;
	y += aVec2.y;

	return *this;
}

Vec2 Vec2::operator-(const Vec2& aVec2) const
{
	return Vec2(x - aVec2.x, y - aVec2.y);
}

Vec2 Vec2::operator-=(const Vec2& aVec2)
{
	x -= aVec2.x;
	y -= aVec2.y;

	return *this;
}

Vec2 Vec2::operator * (const float val) const
{
	return Vec2(x * val, y * val);
}

Vec2 Vec2::operator / (const float val) const
{
	return Vec2(x / val, y / val);
}

Vec2 Vec2::operator *= (const float val)
{
	x *= val;
	y *= val;

	return *this;
}

Vec2 Vec2::operator /= (const float val)
{
	x /= val;
	y /= val;

	return *this;
}

float Vec2::distance(const Vec2& aVec2) 
{
	return std::hypot(this->x - aVec2.x, this->y - aVec2.y);
}

float Vec2::length() const {
	return std::sqrt(x * x + y * y);
}

Vec2 Vec2::normalize() const {
	float len = length();
	return Vec2(x / len, y / len);
}