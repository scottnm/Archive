__author__ = 'Scott Munro'

import math
import game_constants
import pygame

class BaseBullet(object):
    def __init__(self):
        raise Exception("Bullets must be initialized")

    def update(self):
        raise Exception("Bullets must implement an 'update' method")


class PistolBullet(BaseBullet):
    def __init__(self, x, y, rotation):
        self.x = x
        self.y = y
        self.velocity = game_constants.PISTOL_VELOCITY
        self.rotation = rotation
        self.sprite = pygame.Surface((game_constants.PISTOL_BULLET_WIDTH, game_constants.PISTOL_BULLET_HEIGHT))
        self.sprite.fill(game_constants.BLUE)
        self.hitbox = self.sprite.get_rect()

    def update(self):
        self.x += self.velocity * math.cos(self.rotation)
        self.y += self.velocity * math.sin(self.rotation)
        #bullet out of bounds
        if self.x < 0 or self.x > game_constants.WINDOW_WIDTH or self.y < 0 or self.y > game_constants.WINDOW_HEIGHT:
            return 0
        # elif bullet collides return 0
        else:
            return 1

