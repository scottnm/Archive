#!/usr/bin/env python

"""player.py: The player character class. Controls all of the basic player behavior: firing, reloading, moving"""

__author__      = "Scott Munro"
__copyright__   = "Copyright 2015"

import pygame
import Weapon
import math
import game_constants


class Player(pygame.sprite.Sprite):
    def __init__(self, x, y, width, height, weapon, bullet_group, sprite):
        pygame.sprite.Sprite.__init__(self)

        # rotation in radians
        self.rotation = math.pi / 2

        # movement
        self.velocity = 0

        # define the hitbox of the player
        self.image = pygame.Surface((width, height))
        self.image.fill(game_constants.RED)
        self.rect = self.image.get_rect()
        
        # define position
        self.rect.x = x
        self.rect.y = y

        # spawn weapon
        self.weapon = None
        if weapon is game_constants.PISTOL_KEY:
            self.equip_pistol()

        self.bullet_group = bullet_group

        self.extra_ammo = 0

    # players will move in all directions at a CONSTANT velocity of ____

    def equip_none(self):
        self.weapon = None

    def equip_pistol(self):
        self.weapon = Weapon.Pistol()

    def shoot(self):
        if self.weapon is not None:
            bullet = self.weapon.fire(self.rect.x, self.rect.y, self.rotation)
            if bullet is not None:
                self.bullet_group.add(bullet)

    def reload(self):
        if self.weapon is not None and self.extra_ammo > 0:
            self.weapon.reload()
            self.extra_ammo -= 1

    '''
    walk = True if walking, False if strafing
    direction = > 0 if forward; < 0 if backwards; == 0 if still
    '''

    def accelerate(self, direction):
        self.velocity = direction * game_constants.PLAYER_VELOCITY

    def update(self):
        mouse_position = pygame.mouse.get_pos()
        delta_y = mouse_position[1] - self.rect.y
        delta_x = mouse_position[0] - self.rect.x

        # handles division by zero
        if delta_x == 0:
            if delta_y < 0:
                self.rotation = -1 * math.pi
            else:
                self.rotation = math.pi
        else:
            self.rotation = math.atan(delta_y / delta_x)
            if delta_x < 0:
                self.rotation += math.pi

        self.rect.x += self.velocity * math.cos(self.rotation)
        self.rect.y += self.velocity * math.sin(self.rotation)


