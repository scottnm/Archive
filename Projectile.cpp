#include "Projectile.h"
#include "Moveable.h"

Projectile::Projectile(int init_x, int init_y, int xvel, int yvel) : Moveable(init_x, init_y), xvel(xvel), yvel(yvel){}

/* logic for when the time increments */
void Projectile::timeStep(int ticks)
{
    if(xvel > 0){
        for(int i = 0; i < xvel; i++)
            move(RIGHT);
    }
    else{
        for(int i = xvel; i < 0; i++)
            move(LEFT);
    }

    if(yvel > 0){
        for(int i = 0; i < yvel; i++)
            move(DOWN);
        }
    else{
        for(int i = yvel; i < 0; i++)
            move(UP);
    }
}

/* get the text version of the ship shape*/
char Projectile::getShape() const
{
    return PROJECTILESHAPE;
}
