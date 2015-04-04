#ifndef MOVE_H_
#define MOVE_H_

#include "GameObject.h"
#include "GameInfo.h"

enum Movement{UP, DOWN, LEFT, RIGHT};

class Moveable : public GameObject{
protected:

    int prevX;
    int prevY;
    Moveable(int init_x, int init_y) : prevX(init_x), prevY(init_y), GameObject(init_x, init_y){}

public:

    virtual void move(Movement move){
        prevX = x;
        prevY = y;
        switch(move){
            case UP:
                setY(y-1);
                break;
            case DOWN:
                setY(y+1);
                break;
            case LEFT:
                setX(x-1);
                break;
            case RIGHT:
                setX(x+1);
                break;
        }
    }

    virtual bool outOfBounds(){
        bool x_in_bounds = x < BOARDWIDTH && x >= 0;
        bool y_in_bounds = y <= BOARDHEIGHT && y > 0;
        return !(x_in_bounds && y_in_bounds);
    }

    virtual void revertMove(){
        x = prevX;
        y = prevY;
    }
};

#endif
