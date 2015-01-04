#!/usr/bin/env python

"""bullet.py: Contains all bullet subclasses. Each subclass contains updating each bullet for the environment to call"""

__author__      = "Scott Munro"
__copyright__   = "Copyright 2015"

import math
import game_constants
import pygame

class BaseBullet(pygame.sprite.Sprite):
    def __init__(self):
        pygame.sprite.Sprite.__init__(self)


    def update(self):
        raise Exception("Bullets must implement an 'update")

class PistolBullet(BaseBullet):
    def __init__(self, x, y, rotation):

        BaseBullet.__init__(self)

        
        self.velocity = game_constants.PISTOL_VELOCITY
        self.rotation = rotation
        self.image = pygame.Surface((game_constants.PISTOL_BULLET_WIDTH, game_constants.PISTOL_BULLET_HEIGHT))
        self.image.fill(game_constants.BLUE)
        self.rect = self.image.get_rect()
        
        self.rect.x = x
        self.rect.y = y

    def update(self):
        self.rect.x += self.velocity * math.cos(self.rotation)
        self.rect.y += self.velocity * math.sin(self.rotation)

        # bullet out of bounds
        if self.rect.x < 0 or self.rect.x > game_constants.WINDOW_WIDTH or self.rect.y < 0 or self.rect.y > game_constants.WINDOW_HEIGHT:
            return 0
        # elif bullet collides return 0
        else:
            return 1

