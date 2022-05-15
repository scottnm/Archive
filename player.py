#!/usr/bin/env python

"""player.py: The player character class. Controls all of the basic player behavior: firing, reloading, moving"""

__author__      = "Scott Munro"
__copyright__   = "Copyright 2015"

import pygame
import Weapon
import math
import random
import game_constants
import collections
import AccuracyHandler


class Player(pygame.sprite.Sprite):
    def __init__(self, x, y, width, height, weapon, bullet_group, sprite, screen):
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

        # define a bounding circle for mouse tracking
        # center is center of player sprite
        self.circle_radius = game_constants.PLAYER_BUFFER_RADIUS
        
        # define position
        self.rect.x = x
        self.rect.y = y

        # spawn weapon
        self.weapon = None
        if weapon is game_constants.PISTOL_KEY:
            self.equip_pistol()

        self.bullet_group = bullet_group

        self.ammo_inventory = collections.defaultdict(int)

        self.accuracy_handler = AccuracyHandler.AccuracyHandler(screen, self)

        # player attributes
        self.health = 100
        self.invincibility_frames = 0

    # players will move in all directions at a CONSTANT velocity of ____

    def equip_none(self):
        self.weapon = None

    def equip_pistol(self):
        self.weapon = Weapon.Pistol()

    def shoot(self):
        self.calculate_rotation()
        noise = random.uniform(-1*self.accuracy_handler.growth, self.accuracy_handler.growth)
        if self.weapon is not None:
            center = self.get_center()
            bullet = self.weapon.fire(center[0], center[1], self.rotation+noise)
            if bullet is not None:
                self.bullet_group.add(bullet)

    def reload(self):
        if self.ammo_inventory[self.weapon.key] > 0:
            self.weapon.reload()
            self.ammo_inventory[self.weapon.key] -= 1

    def accelerate(self, direction):
        self.velocity = direction * game_constants.PLAYER_VELOCITY

    def calculate_rotation(self):
        mouse_position = pygame.mouse.get_pos()
        center = self.get_center()
        delta_y = (mouse_position[1] - center[1]) * 1.0
        delta_x = mouse_position[0] - center[0]

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

        if self.invincibility_frames > 0:
            self.invincibility_frames -= 1
            if self.invincibility_frames == 0:
                self.image.fill(game_constants.RED)

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

    def revert_from_mouse(self):
        mouse_position = pygame.mouse.get_pos()
        center = self.get_center()
        x_diff = mouse_position[0] - center[0]
        distance = x_diff / math.cos(self.rotation)
        if distance < 1:
            distance = math.fabs(mouse_position[1] - center[1])

        print distance

        if distance < self.circle_radius:
            print 'CLICK'
            x_diff = self.circle_radius * math.cos(self.rotation)
            y_diff = self.circle_radius * math.sin(self.rotation)
            new_center = (mouse_position[0] - x_diff, mouse_position[1] - y_diff)
            self.rect.x = new_center[0] - game_constants.PLAYER_WIDTH/2
            self.rect.y = new_center[1] - game_constants.PLAYER_HEIGHT/2

    def take_damage(self, DAMAGE_CODE):
        if DAMAGE_CODE == game_constants.B_E_COLLISION:
            self.health -= 10
            self.invincibility_frames = 45
            self.image.fill(game_constants.YELLOW)



