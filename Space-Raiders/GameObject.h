#ifndef GAMEOBJ_H_
#define GAMEOBJ_H_

#include <ncurses.h>

class GameObject
{
protected:
    int x;
    int y;

public:

    GameObject(int init_x, int init_y) : x(init_x), y(init_y) {}

    /* logic for when the time increments */
    virtual void timeStep(int ticks) = 0;

    /* get the text version of the ship shape*/
    virtual char getShape() const = 0;

    /* getter and setter for position */
    virtual void setX(const int new_x){x = new_x;}
    virtual void setY(const int new_y){y = new_y;}

    virtual int getX() const{return x;}
    virtual int getY() const{return y;}

    /* check for collisions */
    virtual bool isCollision(const GameObject& other){
        return x == other.getX() && y == other.getY();
    }

    /* print out GameObject on the screen */
    virtual void display(){
        mvaddch(y,x,getShape());
    }
};

#endif
