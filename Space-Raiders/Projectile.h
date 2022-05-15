#ifndef PROJ_H_
#define PROJ_H_

#define PROJECTILESHAPE '|'

#include "Moveable.h"

class Projectile : public Moveable{
protected:
    /* velocities */
    int xvel;
    int yvel;

public:
    /* constructor */
    Projectile(int init_x, int init_y, int xvel, int yvel);
    /* destructor */
    virtual ~Projectile(){}
    virtual void timeStep(int ticks);
    virtual char getShape() const;
};

#endif
