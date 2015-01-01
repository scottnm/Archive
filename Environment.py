import pygame, sys, player, game_constants
from pygame.locals import *
from player import *
from game_constants import *

#game initialization
pygame.init()
pygame.display.set_caption("itch'n'scratch")
MAINDISPLAY = pygame.display.set_mode((WINDOW_WIDTH, WINDOW_HEIGHT))

#base elements
BACKGROUND = pygame.Surface((WINDOW_WIDTH, WINDOW_HEIGHT))
player1 = Player(WINDOW_WIDTH/2, WINDOW_HEIGHT/2, PLAYER_WIDTH, PLAYER_HEIGHT, None, None)

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
                    player1.move(1)
                elif event.key == pygame.K_s:
                    player1.move(-1)


                #firing i == fire
                elif event.type == MOUSEBUTTONDOWN and event.button == 1:
                    player1.shoot()

                #reloading == SPACE KEY
                elif event.key == pygame.K_SPACE:
                    player1.reload()

            elif event.type == pygame.KEYUP:
                #movement
                if event.key == pygame.K_w or event.key == pygame.K_s:
                    player1.move(0)

        #update all sprites
        player1.update()

        #draw all sprites
        MAINDISPLAY.blit(BACKGROUND, (0,0))
        MAINDISPLAY.blit(player1.sprite, (player1.x, player1.y))

        pygame.display.update()




if __name__ == '__main__':
    game()
