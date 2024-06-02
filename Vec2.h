#pragma once
#include<iostream>

class Vec2
{
public:
	float x = 0;
	float y = 0;

	Vec2();
	Vec2(float xIn, float Yin);

	friend std::ostream& operator<<(std::ostream& os, Vec2& aVec2);

	bool operator==(const Vec2& aVec2) const;
	bool operator!=(const Vec2& aVec2) const;

	Vec2 operator+(const Vec2& aVec2) const;
	Vec2 operator-(const Vec2& aVec2) const;
	Vec2 operator*(const float val) const;
	Vec2 operator/(const float val) const;

	Vec2 operator+=(const Vec2& aVec2);
	Vec2 operator-=(const Vec2& aVec2);
	Vec2 operator*=(const float val);
	Vec2 operator/=(const float val);

	float distance(const Vec2& aVec2);
	float length() const;
	Vec2 normalize() const;
};




