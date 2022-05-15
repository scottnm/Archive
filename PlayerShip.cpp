#include "PlayerShip.h"
#include "Moveable.h"

PlayerShip::PlayerShip(int init_x, int init_y, list<Projectile>& lasersFired) : Ship(init_x, init_y), lasersFired(lasersFired) {}

bool PlayerShip::fire() const
{
    return lasersFired.size() < 5;
}

/* get the text version of the ship shape*/
char PlayerShip::getShape() const
{
    return '^';
}
