#ifndef OBJECTS_HPP_BUBBLE
#define OBJECTS_HPP_BUBBLE

#include "raylib.h"

class Base {

public:

    Base(Rectangle d) : domain(d) {}

    void move(int ax, int ay){ domain.x += ax; domain.y += ay; }

    Rectangle domain;
    Texture tex;

};

class Enemy {

public:

    Enemy(Rectangle d) : domain(d) {}
    void move(int ax, int ay){ domain.x += ax; domain.y += ay; }
    Rectangle domain;
    Texture tex;
    Vector2 spawn;
    Vector2 dest;
    double dist;
    bool traveling = false;


};

class Player {

public:

    Player(Rectangle d) : domain(d) {}
    void move(float ax, float ay){ domain.x += ax; domain.y += ay; }
    Rectangle domain;
    Texture tex;
    int health = 3;


};

class Bullet {
public:

    Bullet(float xx, float yy, float rr, Vector2 v, float sp) : x(xx), y(yy), r(rr), vec(v), speed(sp) {}

    float x, y, r;
    Vector2 vec;
    float speed;
};

class Wall {
public:
    Rectangle domain;
    Wall(Rectangle wall){domain = wall;}

};

#endif
