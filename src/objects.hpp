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


};


#endif
