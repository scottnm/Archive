itchnscratch
============
Goal: Top-down zombie shooting game

Current working phase: Prototyping
============
Prototyping

Basic control (currently working on)

Basic mechanics

No dynamic sprites

Basic enemies with basic generation

Basic ammo drops

Files:

  bullet.py: Contains all bullet subclasses. Each subclass contains updating each bullet for the environment to call

  enemies.py: an enemy class that currently stores a skeleton class and a basic enemy class
  
  Environment.py: a program that initiates the pygame instance and handles the game loop

  game_constants.py: collection of constants for use throughout game

  InputProcessor.py: Contains classes for different InputProcessing/Control schemes: Keyboard, gamepad, etc
  
  Items.py: The item classes and subclasses: currently includes PISTOLAMMO...
  
  Obstacle.py: The obstacle and obstacle subclasses
  
  ObstacleGenerator.py: used to quickly generate levels and obstacles
  
  player.py: a player class to control player behavior (reloading, shooting, movement)
  
  run.sh: just a quick bash command to run python in 32-bit mode to be compatible with pygame
  
  Weapon.py: Contains all weapon classes. Each class controls behavior for a specific weapon.
  


