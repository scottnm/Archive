"""AccuracyHandler.py: generates gr"""

__author__ = "Scott Munro"
__copyright__ = "Copyright 2015"

import math
import pygame
import game_constants


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

    """
    def draw(self):
        # draw the two lines
        rotation = self.player.rotation % (math.pi+math.pi)
        x_bound = 0
        y_bound = 0

        if 0 <= rotation < math.pi/2:
            x_bound = game_constants.WINDOW_WIDTH
            y_bound = game_constants.WINDOW_HEIGHT

        elif math.pi/2 <= rotation < math.pi:
            y_bound = game_constants.WINDOW_HEIGHT

        # elif math.pi <= rotation < (math.pi/2 + math.pi) do nothing

        elif (math.pi/2 + math.pi) <= rotation < (math.pi + math.pi):
            x_bound = game_constants.WINDOW_WIDTH

        xdiff = math.fabs(self.player.rect.x - x_bound)
        ydiff = math.fabs(self.player.rect.y - y_bound)

        if xdiff < ydiff:
            xpos_
    """
    def draw_lines(self):

        rotation1 = self.player.rotation + self.growth
        rotation2 = self.player.rotation - self.growth
        print str(rotation1)+'\t'+str(self.player.rotation)+'\t'+str(rotation2)

        self.draw(rotation1, game_constants.WHITE)
        self.draw(rotation2, game_constants.RED)

    def draw(self, rotation, color):
        # draw two lines
        x_pos = 0
        if 0 <= rotation < math.pi/2 or (math.pi/2 + math.pi) <= rotation < (math.pi + math.pi):
            x_pos = game_constants.WINDOW_WIDTH

        x_diff = x_pos - (self.player.rect.x+game_constants.PLAYER_WIDTH/2)
        y_pos = x_diff * math.tan(rotation)
        player_center = self.player.get_center()
        pygame.draw.line(self.screen, color, (player_center[0], player_center[1]), (x_pos, y_pos), 1)
