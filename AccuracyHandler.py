"""AccuracyHandler.py: generates gr"""

__author__ = "Scott Munro"
__copyright__ = "Copyright 2015"

import math
import pygame
import game_constants


class AccuracyHandler(object):
    RANGE = math.pi / 24
    GROWTH_FACTOR = math.pi / 2880
    FREEDOM = math.pi / 1000

    def __init__(self, screen, player):
        self.screen = screen  # display to blit onto
        self.player = player
        self.theta_last = player.rotation
        self.growth = AccuracyHandler.RANGE

    def update(self):
        if math.fabs(self.theta_last - self.player.rotation) < AccuracyHandler.FREEDOM:
            self.growth -= AccuracyHandler.GROWTH_FACTOR
            if self.growth < 0:
                self.growth = 0
        else:
            self.growth += AccuracyHandler.GROWTH_FACTOR
            self.growth += AccuracyHandler.GROWTH_FACTOR
            if self.growth > AccuracyHandler.RANGE:
                self.growth = AccuracyHandler.RANGE

        self.theta_last = self.player.rotation


    def draw_lines(self):

        rotation1 = self.player.rotation + self.growth
        rotation2 = self.player.rotation - self.growth

        print 'R1: '+str(rotation1)+'\tS: '+str(self.player.rotation)+'\tR2: '+str(rotation2)

        self.draw(rotation1, game_constants.WHITE)
        self.draw(self.player.rotation, game_constants.BLUE)
        self.draw(rotation2, game_constants.RED)

    def draw(self, rotation, color):
        player_center = self.player.get_center()

        x_pos = game_constants.WINDOW_DIAGONAL*math.cos(rotation)
        y_pos = game_constants.WINDOW_DIAGONAL*math.sin(rotation)

        pygame.draw.line(self.screen, color, (player_center[0], player_center[1]), (player_center[0]+x_pos, player_center[1]+y_pos), 1)
