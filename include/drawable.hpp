#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP

#include <raylib.h>

class Drawable
{
public:
    Vector2 position;

    virtual void draw() = 0;
};

#endif