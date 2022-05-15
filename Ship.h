#ifndef SHIP_H_
#define SHIP_H_

#include "Moveable.h"

class Ship: public Moveable
{
protected:
    /* returns whether the ship should fire or not */
    virtual bool fire() const = 0;

public:
    Ship(int init_x, int init_y) : Moveable(init_x, init_y){}

};

#endif
