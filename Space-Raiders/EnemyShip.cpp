#include "EnemyShip.h"
#include "Moveable.h"

EnemyShip::EnemyShip(int init_x, int init_y, char shape) : Ship(init_x, init_y), shape(shape), left_moves(0)
{
    firerate = shape == 'W' ? 150 : 75; /* set fire rate based on ship type */
}

bool EnemyShip::fire() const
{
    int random = rand();
    return random%FIREFACTOR < firerate;
}

/* logic for when the time increments */
void EnemyShip::timeStep(int ticks)
{
    if(ticks%MOVENUM == MOVENUM-1){
        if(left_moves % 14 < 7)
            move(LEFT);
        else
            move(RIGHT);
        left_moves++;

        if(ticks%MOVEDOWNCNT == MOVEDOWNCNT - 1)
            move(DOWN);
    }
}

/* get the text version of the ship shape*/
char EnemyShip::getShape() const
{
    return shape;
}
