#!/usr/bin/env python

"""ObstacleGenerator.py: used to quickly generate levels and obstacles"""

__author__      = "Scott Munro"
__copyright__   = "Copyright 2015"

from game_constants import *
from Obstacle import *


def generate_obstacle_group(group):
    # left bounding wall for the screen
    bounding_wall_left = Obstacle(-1*BOUNDING_WALL_THICKNESS, -1*BOUNDING_WALL_THICKNESS, BOUNDING_WALL_THICKNESS, BOUNDING_WALL_THICKNESS+WINDOW_HEIGHT)
    bounding_wall_right = Obstacle(WINDOW_WIDTH, 0, BOUNDING_WALL_THICKNESS, BOUNDING_WALL_THICKNESS+WINDOW_HEIGHT)
    bounding_wall_top = Obstacle(0, -1*BOUNDING_WALL_THICKNESS, BOUNDING_WALL_THICKNESS+WINDOW_WIDTH, BOUNDING_WALL_THICKNESS)
    bounding_wall_bottom = Obstacle(-1*BOUNDING_WALL_THICKNESS, WINDOW_HEIGHT, BOUNDING_WALL_THICKNESS+WINDOW_WIDTH, BOUNDING_WALL_THICKNESS)

    group.add(bounding_wall_bottom, bounding_wall_left, bounding_wall_right, bounding_wall_top)