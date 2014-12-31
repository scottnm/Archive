import pygame, sys, player
from pygame.locals import *
from player import *

#CONSTANTS
WINDOW_WIDTH = 1280
WINDOW_HEIGHT = 720
CLOCKFPS = 30

#game initialization
pygame.init()
pygame.display.set_caption("itch'n'scratch")
MAINDISPLAY = pygame.display.set_mode((WINDOW_WIDTH, WINDOW_HEIGHT))

#base elements
BACKGROUND = pygame.Surface((WINDOW_WIDTH, WINDOW_HEIGHT))
player1 = Player(WINDOW_WIDTH/2, WINDOW_HEIGHT/2, 20, 20, None, None)

def game():

    while True:
        for event in pygame.event.get():
            #closing the window
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()

            elif event.type == pygame.KEYDOWN:
                #movement
                if event.key == pygame.K_w:
                    player1.move(True, -1)
                elif event.key == pygame.K_a:
                    player1.move(False, -1)
                elif event.key == pygame.K_s:
                    player1.move(True,  1)
                elif event.key == pygame.K_d:
                    player1.move(False, 1)

                #rotation
                elif event.key == pygame.K_j:
                    player1.rotate(-1)
                elif event.key == pygame.K_l:
                    player1.rotate(1)

                #firing i == fire
                elif event.key == pygame.K_i:
                    player1.shoot()

                #reloading == SPACE KEY
                elif event.key == pygame.K_SPACE:
                    player1.shoot()

            elif event.type == pygame.KEYUP:
                #movement
                if event.key == pygame.K_w or event.key == pygame.K_s:
                    player1.move(True, 0)
                elif event.key == pygame.K_a or event.key == pygame.K_d:
                    player1.move(False, 0)

                #rotation
                elif event.key == pygame.K_j or event.key == pygame.K_l:
                    player1.rotate(0)

        #update all sprites
        player1.update()

        #draw all sprites
        MAINDISPLAY.blit(BACKGROUND, (0,0))
        MAINDISPLAY.blit(player1.sprite, (player1.x_pos, player1.y_pos))

        pygame.display.update()




if __name__ == '__main__':
    game()
