#ifndef OBJECTS_HPP_BUBBLE
#define OBJECTS_HPP_BUBBLE

#include "raylib.h"

class Base {

public:

    Base(Rectangle d) : domain(d) {}

    Rectangle domain;
    Texture tex;

};


#endif
