#ifndef PSHIP_H_
#define PSHIP_H_

#include "Moveable.h"
#include "Projectile.h"
#include "Ship.h"
#include <list>
using std::list;

class PlayerShip : public Ship
{
protected:
    list<Projectile>& lasersFired;
public:
    PlayerShip(int init_x, int init_y, list<Projectile>& lasersFired);
    virtual ~PlayerShip(){}
    virtual void timeStep(int ticks){};
    virtual char getShape() const;
    virtual bool fire() const;
};

#endif
