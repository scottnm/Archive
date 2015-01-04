#!/usr/bin/env python

"""Weapon.py: Contains all weapon classes. Each class controls behavior for a specific weapon."""

__author__      = "Scott Munro"
__copyright__   = "Copyright 2015"

import bullet

class Weapon(object):
    def fire(self):
        raise Exception("Must implement 'Fire' method")

    def reload(self):
        raise Exception("Must implement 'reload' method")


class Pistol(Weapon):
    def __init__(self):
        self.CLIP_CAPACITY = 6
        self.ammo_left = 6

    def fire(self, x, y, rotation):
        if self.ammo_left > 0:
            self.ammo_left -= 1
            return bullet.PistolBullet(x, y, rotation)

        return None

    def reload(self):
        self.ammo_left = 6


# class Shotgun( Weapon ):

# class Crossbow( Weapon ):
