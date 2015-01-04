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

        elif event.type == pygame.KEYDOWN:
            # movement
            if event.key == pygame.K_w:
                self.player.move(1)
            elif event.key == pygame.K_s:
                self.player.move(-1)

            # reloading == SPACE KEY
            elif event.key == pygame.K_SPACE:
                self.player.reload()

        elif event.type == pygame.KEYUP:
            # movement
            if event.key == pygame.K_w or event.key == pygame.K_s:
                self.player.move(0)

        # firing i == fire
        elif event.type == pygame.MOUSEBUTTONDOWN:
            if event.button == 1:
                self.player.shoot()
