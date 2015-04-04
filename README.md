BOARD
=====
The board is represented implicity through an out of bounds function (defined in moveable.h) as 35x20 board

GAMELOOP
===========
The gameloop runs as follows:
* update each of the moveable entities in the game
* take in the players input and move/update or fire based on the input
* check for collisions between player's lasers and enemies and between enemies' lasers and the player.
* check for enemies crossing bottom of the screen and making the player lose
* draw all of the entities to the screen and refresh the screen
* increment the tick marker
* when the game ends wait for the player to dismiss the end screen

MOVEMENT
=========
Movement happens by set the x and y position of the whatever moveable entity is moving to the new position. i.e. if the entity moves left then it's x gets set to 1 less than it's current value and the y remains the same.

Movement is also checked for bounds so that entities cannot illegally go out of bounds. Some entities such as bullets can go out of bounds but when they do they are deleted/removed from the game.

COLLISION
=========
Collision is checked by simply checking whether the x and y coordinates of two gameobjects are the same.


GAME OBJECT HIERARCHY
=====================
                abstract GameObject
                        |
                        |
                abstract   Moveable
                |                 |
                |                 |
               ship            projectile
            |          |
            |          | 
          player     enemy  

OBJECT SPECIFICATIONS
=====================

GameObject
----------
defines the base characteristics that all gameobjects must have
* getting the shape of the ship
* setting x and y 
* getting x and y
* checking for collisions
* displaying the object


Moveable
---------
defines the behavior specific to moveable gameobjects and inherits from game object
* moving the gameobject based on a movement enum
* the previous position of the moveable so that if an illegal collision or out of bounds occurs then the movement can be undone.


Ship
----
defines abstract ship specific behavior and inherits from moveable
* ships must be able to fire

PlayerShip
----------
* contains information about how a playership should fire, update at each timestep, and it's shape. 
* also keeps track of the lasers that this ship has fired

EnemyShip
---------
* defines enemy ship specific behavior
* defines a firerate specific to the type of enemy ship (u vs W)
* defines a shape (u vs W)
* keeps track of the number of movements that a ship has made in a specific direction so that at standard intervals it can switch between moving left and right

Projectile
-----------
* defines generic projectile specific behavior
* a projectile's movement is defined by it's constructed velocity (i.e. projectiles moving down are implicitly enemy projectiles and ones moving up are the player's )

Extras rules and game things
------------------------------------------
* Player can only fire 5 bullets at once
* When the game ends, the player gets either a game win or lose screen or if the player quits prior to the game ending, just exits
* Moveable abstract base class used to distinguish between projectiles and players and non-moving objects (which could be implemented later such as a barrier)
