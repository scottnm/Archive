#ifndef ESHIP_H_
#define ESHIP_H_

#include <cstdlib>
#define FIREFACTOR 10000
#define MOVENUM 20
#define MOVEDOWNCNT 280

#include "Ship.h"

class EnemyShip : public Ship{
private:
    int firerate; /* defined in X ten-thousandths */
    char shape;
    int left_moves;

public:
    EnemyShip(int init_x, int init_y, char shape);
    virtual ~EnemyShip(){}
    virtual void timeStep(int ticks);
    virtual char getShape() const;
    virtual bool fire() const;
};

#endif
