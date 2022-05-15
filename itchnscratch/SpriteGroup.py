#!/usr/bin/env python

"""NOT IMPLEMENTEDSpriteGroup.py: class to override pygame's spritegroup"""

__author__ = "Scott Munro"
__copyright__ = "Copyright 2015"

import pygame


class SpriteGroup(pygame.sprite.Group):
    def __init__(self, *sprites):
        self.Group = pygame.sprite.Group(self, *sprites)

    def update_x(self):
        for sprite in self.Group:
            sprite.update_x()

    def update_y(self):
        for sprite in self.Group:
            sprite.update_y()