"""AccuracyHandler.py: generates gr"""

__author__ = "Scott Munro"
__copyright__ = "Copyright 2015"

import math


class AccuracyHandler(object):
    RANGE = math.pi / 24
    GROWTH_FACTOR = math.pi / 4320

    def __init__(self, screen, player):
        self.screen = screen  # display to blit onto
        self.player = player
        self.theta_last = player.rotation
        self.growth = math.pi / 24

    def update(self):
        if math.fabs(self.theta_last - self.player.rotation) < AccuracyHandler.RANGE:
            self.growth -= AccuracyHandler.GROWTH_FACTOR
        else:
            self.growth += AccuracyHandler.GROWTH_FACTOR

        self.theta_last = self.player.rotation

    def draw(self):
        # draw the two lines
        None