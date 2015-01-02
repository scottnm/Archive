import pygame, random, player, InputProcessor, enemies, game_constants
from pygame.locals import *
from player import *
from game_constants import *

#game initialization
pygame.init()
pygame.display.set_caption("itch'n'scratch")
MAINDISPLAY = pygame.display.set_mode((WINDOW_WIDTH, WINDOW_HEIGHT))
clock = pygame.time.Clock()


#base elements
BACKGROUND = pygame.Surface((WINDOW_WIDTH, WINDOW_HEIGHT))
player1 = Player(WINDOW_WIDTH/2, WINDOW_HEIGHT/2, PLAYER_WIDTH, PLAYER_HEIGHT, None, None)
playerInputProcessor = InputProcessor.keyboardInputProcessor( player1 )
enemies_list = [None, None, None, None, None]

def game():
    framecounter = 0
    enemycounter = 0

    while True:

        #generate enemies
        if framecounter % (CLOCKFPS*ENEMY_PERIOD) == 0 and random.random() > .5:
            framecounter = 0
            enemies_list[enemycounter % len(enemies_list)] = enemies.BasicEnemy()
            enemycounter += 1

        for event in pygame.event.get():
            playerInputProcessor.handleEvent( event )

        #update all sprites
        player1.update()
        for enemy in enemies_list:
            if enemy != None:
                enemy.move(player1.x, player1.y)
                enemy.update()

        #draw all sprites
        MAINDISPLAY.blit(BACKGROUND, (0,0))
        MAINDISPLAY.blit(player1.sprite, (player1.x, player1.y))
        for enemy in enemies_list:
            if enemy != None:
                MAINDISPLAY.blit(enemy.sprite, (enemy.x, enemy.y))

        pygame.display.update()
        clock.tick( CLOCKFPS )
        framecounter += CLOCKFPS




if __name__ == '__main__':
    game()
