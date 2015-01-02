import pygame, sys
from pygame.locals import *

'''
An Input Processor handles the player's input
and interacts with the processor's player object
'''
class InputProcessor( object ):
    def __init__( self, player ):
        raise Exception("All InputProcessors must be initialized")

    def handleEvent( self, event):
        raise Exception("All InputProcessors must implement a 'handleEvent()' method")


class keyboardInputProcessor( InputProcessor ):
    def __init__( self, player ):
        self.player = player

    def handleEvent( self, event ):
        #closing the window
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit()

        elif event.type == pygame.KEYDOWN:
            #movement
            if event.key == pygame.K_w:
                self.player.move(1)
            elif event.key == pygame.K_s:
                self.player.move(-1)


            #firing i == fire
            elif event.type == MOUSEBUTTONDOWN and event.button == 1:
                self.player.shoot()

            #reloading == SPACE KEY
            elif event.key == pygame.K_SPACE:
                self.player.reload()

        elif event.type == pygame.KEYUP:
            #movement
            if event.key == pygame.K_w or event.key == pygame.K_s:
                self.player.move(0)
