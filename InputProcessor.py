#!/usr/bin/env python

"""InputProcessor.py: Contains classes for different InputProcessing/Control schemes: Keyboard, gamepad, etc"""

__author__      = "Scott Munro"
__copyright__   = "Copyright 2015"

import pygame
import sys
from pygame.locals import *

'''
An Input Processor handles the player's input
and interacts with the processor's player object
'''


class InputProcessor(object):
    def __init__(self, player):
        raise Exception("All InputProcessors must be initialized")

    def handle_event(self, event):
        raise Exception("All InputProcessors must implement a 'handleEvent()' method")


class KeyboardInputProcessor(InputProcessor):
    def __init__(self, player):
        self.player = player

    def handle_event(self, event):
        # closing the window
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit()

        elif event.type == pygame.MOUSEMOTION:
            self.player.calculate_rotation()

        elif event.type == pygame.KEYDOWN:
            # movement
            if event.key == pygame.K_w:
                print 'W_ON'
                self.player.accelerate(1)
            elif event.key == pygame.K_s:
                print 'S_ON'
                self.player.accelerate(-1)

            # reloading == SPACE KEY
            elif event.key == pygame.K_SPACE:
                print 'SPACE_ON'
                self.player.reload()

        elif event.type == pygame.KEYUP:
            # movement
            if event.key == pygame.K_w or event.key == pygame.K_s:
                print 'S_W_OFF'
                self.player.accelerate(0)

        # firing i == fire
        elif event.type == pygame.MOUSEBUTTONDOWN:
            if event.button == 1:
                print 'M1_ON'
                self.player.shoot()
