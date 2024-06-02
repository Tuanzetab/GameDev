#pragma once
#include "Vec2.h"
#include <SFML/Graphics.hpp>

class CTransform {
public:
    Vec2 pos = { 0.f, 0.f };
    Vec2 velocity = { 0.f, 0.f };
    float angle = 0.f;
    CTransform(const Vec2& p, const Vec2& v , float a );
};

class CShape {
public:
    sf::CircleShape circle;
    CShape(float radius, int points, const sf::Color& fill, const sf::Color& outline, float thickness);
};

class CCollision {
public:
    float radius;
    CCollision(float r);
};

class CScore {
public:
    int score;
    CScore(int s);
};

class CLifespan {
public:
    int remaining = 0;
    int total = 0;
    CLifespan(int total);
};

class CInput {
public:
    bool up;
    bool left;
    bool right;
    bool down;
    bool shoot;
    CInput();
};

