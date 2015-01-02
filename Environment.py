import pygame
import random
import InputProcessor
import enemies
from pygame.locals import *
from player import *
from game_constants import *

# game initialization
pygame.init()
pygame.display.set_caption("itch'n'scratch")
MAIN_DISPLAY = pygame.display.set_mode((WINDOW_WIDTH, WINDOW_HEIGHT))
clock = pygame.time.Clock()


# base elements
BACKGROUND = pygame.Surface((WINDOW_WIDTH, WINDOW_HEIGHT))
playerOne = Player(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, PLAYER_WIDTH, PLAYER_HEIGHT, None, None)
playerInputProcessor = InputProcessor.KeyboardInputProcessor(playerOne)
enemiesList = [None, None, None, None, None]


def game():
    frame_counter = 0
    enemy_counter = 0

    while True:

        # generate enemies
        if frame_counter % (CLOCK_FPS * ENEMY_PERIOD) == 0 and random.random() > .5:
            frame_counter = 0
            enemiesList[enemy_counter % len(enemiesList)] = enemies.BasicEnemy()
            enemy_counter += 1

        for event in pygame.event.get():
            playerInputProcessor.handle_event(event)

        # update all sprites
        playerOne.update()
        for enemy in enemiesList:
            if enemy is not None:
                enemy.move(playerOne.x, playerOne.y)
                enemy.update()

        # draw all sprites
        MAIN_DISPLAY.blit(BACKGROUND, (0, 0))
        MAIN_DISPLAY.blit(playerOne.sprite, (playerOne.x, playerOne.y))
        for enemy in enemiesList:
            if enemy is not None:
                MAIN_DISPLAY.blit(enemy.sprite, (enemy.x, enemy.y))

        pygame.display.update()
        clock.tick(CLOCK_FPS)
        frame_counter += CLOCK_FPS


if __name__ == '__main__':
    game()
