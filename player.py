#!/usr/bin/env python

"""player.py: The player character class. Controls all of the basic player behavior: firing, reloading, moving"""

__author__      = "Scott Munro"
__copyright__   = "Copyright 2015"

import pygame
import Weapon
import math
import game_constants
import collections


class Player(pygame.sprite.Sprite):
    def __init__(self, x, y, width, height, weapon, bullet_group, sprite):
        pygame.sprite.Sprite.__init__(self)

        # rotation in radians
        self.rotation = 0

        # movement
        self.velocity = 0
        self.x_vel = 0
        self.y_vel = 0

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

        """
        simple infrastructure for ammo reloading
        perhaps in the future have random drops with random ammount of ammo in them?
        """
        self.ammo_inventory = collections.defaultdict(int)

    # players will move in all directions at a CONSTANT velocity of ____

    def equip_none(self):
        self.weapon = None

    def equip_pistol(self):
        self.weapon = Weapon.Pistol()

    def shoot(self):
        self.calculate_rotation()
        if self.weapon is not None:
            center = self.get_center()
            bullet = self.weapon.fire(center[0], center[1], self.rotation)
            if bullet is not None:
                self.bullet_group.add(bullet)

    def reload(self):
        if self.ammo_inventory[self.weapon.key] > 0:
            self.weapon.reload()
            self.ammo_inventory[self.weapon.key] -= 1

    '''
    walk = True if walking, False if strafing
    direction = > 0 if forward; < 0 if backwards; == 0 if still
    '''

    def accelerate(self, direction):
        self.velocity = direction * game_constants.PLAYER_VELOCITY

    def calculate_rotation(self):
        mouse_position = pygame.mouse.get_pos()
        center = self.get_center()
        delta_y = (mouse_position[1] - center[1]) * 1.0
        delta_x = mouse_position[0] - center[0]
        # delta_y = (mouse_position[1] - self.rect.y ) * 1.0
        # delta_x = mouse_position[0] - self.rect.x

        # handles division by zero
        if delta_x == 0:
            if delta_y < 0:
                self.rotation = -1 * math.pi/2
            else:
                self.rotation = math.pi/2
        else:
            self.rotation = math.atan(delta_y / delta_x)
            if delta_x < 0:
                self.rotation += math.pi

    def update(self):
        self.x_vel = self.velocity * math.cos(self.rotation)
        self.y_vel = self.velocity * math.sin(self.rotation)

    def move_x(self):
        self.rect.x += self.x_vel

    def revert_x(self):
        if self.x_vel > 0:
            self.rect.x -= 1
        else:
            self.rect.x += 1

    def move_y(self):
        self.rect.y += self.y_vel

    def revert_y(self):
        if self.y_vel > 0:
            self.rect.y -= 1
        else:
            self.rect.y += 1

    def get_center(self):
        center_coordinates = (self.rect.x + game_constants.PLAYER_WIDTH/2, self.rect.y + game_constants.PLAYER_HEIGHT/2)
        return center_coordinates


