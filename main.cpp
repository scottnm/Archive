/**
Name: Scott Munro
Eid: snm2232
*/

#include <ncurses.h>    // the ncurses library
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <unistd.h> // sleep
#include <string.h>

#include "PlayerShip.h"
#include "Projectile.h"
#include "EnemyShip.h"
#include "Moveable.h"
#include "GameInfo.h"

#include <list>
using std::list;

#include "timehelper.cpp"

int main (int argc, char const *argv[])
{

    srand (time(NULL)); /// seed to some random number based on time
    if ( initscr() == NULL ) { /// init the screen, defaults to stdscr
        fprintf(stderr, "Error initialising ncurses.\n");
        exit(EXIT_FAILURE);
    }

    noecho(); /// don't echo keypresses
    keypad(stdscr, TRUE); /// set the keypad to stdscr
    nodelay(stdscr, false);
    curs_set(0); /// set cursor to invisible (if allowed by the os)
    const int delay = 50; /// milliseconds
    timeout(delay); /// set how long between keypresses (more or less)

    /* game state */
    int ch = 0;
    bool quit = false;
    bool won = false;
    int points = 0;
    int elapsedTime = getElapsedTime();
    int ticks = 0;

    /* initialize lists of entities for game to manage */
    list<EnemyShip> enemies;
    /* build enemy list */
    int rows = 2;
    int cols = 21;
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j+=2){
            char shipshape = ((i*cols + j) % 4) == 0 ? 'W' : 'u';
            EnemyShip enemy(j+12, i+1, shipshape);
            enemies.push_back(enemy);
        }
    }

    list<Projectile> playerLasers;
    list<Projectile> enemyLasers;
    /* player character */
    PlayerShip player(BOARDWIDTH/2, BOARDHEIGHT, playerLasers);

    while (!quit){
        ch = getch();
        erase(); /// erase the screen (after getch())
        if ( ch != ERR) { /// user has a keypress
            /// this is to delay until the next tick
            elapsedTime = getElapsedTime();
            if (elapsedTime < delay){
                nsleep(delay-elapsedTime);}
        }

        /// this is how to print a string to the screen, 0, 0 is the  y, x location
        mvprintw(0, 0, "Points: %i.   'q' to quit.\n", points);

        /* update all current entities */
        // update enemies and enemy lasers
        for(list<EnemyShip>::iterator iter = enemies.begin(); iter != enemies.end(); iter++){
            iter->timeStep(ticks);
            if(iter->outOfBounds()){
                if(iter->getY() >= BOARDHEIGHT){
                    quit = true;
                    break;
                }
                else{
                    iter->revertMove();
                }
            }
            if(iter->fire()){
                Projectile eLaser(iter->getX(), iter->getY()+1, 0, 1);
                enemyLasers.push_back(eLaser);
            }
        }

        list<Projectile>::iterator iter = enemyLasers.begin();
        while(iter != enemyLasers.end()){
            bool removed = false;
            iter->timeStep(ticks);
            if(iter->outOfBounds()){
                iter = enemyLasers.erase(iter);
                removed = true;
            }

            if(!removed)
                iter++;
        }

        // update player lasers
        iter = playerLasers.begin();
        while(iter != playerLasers.end()){
            bool removed = false;
            iter->timeStep(ticks);
            if(iter->outOfBounds()){
                iter = playerLasers.erase(iter);
                removed = true;
            }

            if(!removed)
                iter++;
        }

        /// take in player input
        switch(ch){
                case KEY_RIGHT:
                    player.move(RIGHT);
                    break;
                case KEY_LEFT:
                    player.move(LEFT);
                    break;
                case ' ':
                    if(player.fire()){
                        Projectile laser(player.getX(), player.getY()-1, 0, -1);
                        playerLasers.push_front(laser);
                    }
                    break;

                case 'q':
                    endwin();
                    return 0;
                    break;
        }

        /* check for out of bounds player movement */
        if(player.outOfBounds())
            player.revertMove();

        /* check for collisions */
        /* check for player losing collisions */
        for(list<EnemyShip>::iterator iter = enemies.begin(); iter != enemies.end(); iter++){
            if(iter->isCollision(player))
                quit = true;
        }

        for(list<Projectile>::iterator iter = enemyLasers.begin(); iter != enemyLasers.end(); iter++){
            if(iter->isCollision(player))
                quit = true;
        }

        /* check for collisions between player-lasers and enemies */
        list<EnemyShip>::iterator enemyship = enemies.begin();
        while(enemyship != enemies.end()){

            list<Projectile>::iterator plaser = playerLasers.begin();
            bool wasCollision = false;
            while(plaser != playerLasers.end()){
                if(enemyship->isCollision(*plaser)){
                    /* if there is a collision remove both elements and move on */
                    enemyship = enemies.erase(enemyship);
                    plaser = playerLasers.erase(plaser);
                    wasCollision = true;
                    points++;
                    break;
                }
                /* if there wasn't a collision get the next laser to test */
                plaser++;
            }

            /* if there wasn't a collision get the next enemy ship and look again */
            if(!wasCollision)
                enemyship++;
        }

        if(enemies.size() <= 0){
            won = true;
            quit = true;
        }

        /* draw all entities */
        player.display();

        for(list<Projectile>::iterator iter = playerLasers.begin(); iter != playerLasers.end(); iter++)
            iter->display();

        for(list<EnemyShip>::iterator iter = enemies.begin(); iter != enemies.end(); iter++)
            iter->display();

        for(list<Projectile>::iterator iter = enemyLasers.begin(); iter != enemyLasers.end(); iter++)
            iter->display();


        refresh(); // refresh the screen after adding everything
        move(0,0); /// move cursor to 0,0 (looks prettier if os doesn't allow invisible cursors)
        ticks++;
    }

    const char* endscreen = won ? "Congrats!" : "Sucks to Suck";
    const char* sub1 = won ? "You Won!" : "Try Again";
    const char* sub2 = "q = quit";
    /* print end screen */
    erase();
    mvwprintw(stdscr, BOARDHEIGHT/2, (BOARDWIDTH-strlen(endscreen))/2, "%s", endscreen);
    mvwprintw(stdscr, BOARDHEIGHT/2 + 1, (BOARDWIDTH-strlen(sub1))/2, "%s", sub1);    
    mvwprintw(stdscr, BOARDHEIGHT/2 + 2, (BOARDWIDTH-strlen(sub2))/2, "%s", sub2);

    /* wait for player to quit */
    while ( (ch = getch() != 'q') ){}

    endwin();   /// cleanup the window

    return 0;
}
