#!/usr/bin/env python

"""enemies.py: Contains all enemy classes which contain enemy behavior and updating"""

__author__      = "Scott Munro"
__copyright__   = "Copyright 2015"

import pygame
import random
import game_constants


class Enemy(pygame.sprite.Sprite):
    # movement skeleton needed for each enemy
    def move(self):
        raise Exception("All enemies must implement 'move()'")

    # update skeleton
    def update(self):
        raise Exception("All enemies must implement 'update()'")

    def __init__(self):
        pygame.sprite.Sprite.__init__(self)


class BasicEnemy(Enemy):
    def move(self, target_x, target_y):
        if self.rect.x > target_x+10:
            self.x_vel = -1 * game_constants.BASIC_ENEMY_VELOCITY
        elif self.rect.x < target_x-10:
            self.x_vel = game_constants.BASIC_ENEMY_VELOCITY
        else:
            self.x_vel = 0

        if self.rect.y > target_y+10:
            self.y_vel = -1 * game_constants.BASIC_ENEMY_VELOCITY
        elif self.rect.y < target_y-10:
            self.y_vel = game_constants.BASIC_ENEMY_VELOCITY
        else:
            self.y_vel = 0

    def update(self):
        self.rect.x += self.x_vel
        self.rect.y += self.y_vel

    def __init__(self):
        Enemy.__init__(self)

        self.image = pygame.Surface((game_constants.BASIC_ENEMY_WIDTH, game_constants.BASIC_ENEMY_HEIGHT))
        self.image.fill(game_constants.GREEN)
        self.rect = self.image.get_rect()
        
        self.rect.x = random.randint(0, game_constants.WINDOW_WIDTH - game_constants.BASIC_ENEMY_WIDTH)
        self.rect.y = random.randint(0, game_constants.WINDOW_HEIGHT - game_constants.BASIC_ENEMY_HEIGHT)

        self.x_vel = 0
        self.y_vel = 0
