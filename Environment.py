#!/usr/bin/env python

"""Environment.py: The Environment that the game runs in. Contains the game loop and updating the display"""

__author__ = "Scott Munro"
__copyright__ = "Copyright 2015"

import pygame
import random
import InputProcessor
import enemies
import Obstacle
import ObstacleGenerator
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
enemy_group = pygame.sprite.Group()
bullet_group = pygame.sprite.Group()
obstacle_group = pygame.sprite.Group()
ObstacleGenerator.generate_obstacle_group(obstacle_group)
playerOne = Player(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, PLAYER_WIDTH, PLAYER_HEIGHT, game_constants.PISTOL_KEY,
                   bullet_group, None)
playerInputProcessor = InputProcessor.KeyboardInputProcessor(playerOne)
player_group = pygame.sprite.Group(playerOne)

#groups don't update as references FIX FIX FIX
collision_group = pygame.sprite.Group(obstacle_group, player_group, enemy_group)
all_sprites = pygame.sprite.Group(enemy_group, bullet_group, player_group)


def game():

    # used to regulate the number of enemies for the time being
    frame_counter = 0
    enemy_counter = 0

    while True:
        print playerOne.rotation
        # generate enemies
        if enemy_counter < 5 and frame_counter % (CLOCK_FPS * ENEMY_PERIOD) == 0 and random.random() > .5:
            enemy_group.add(enemies.BasicEnemy())
            enemy_counter += 1

        # handle events
        for event in pygame.event.get():
            playerInputProcessor.handle_event(event)

        # update all sprites

        # update players
        for player in player_group:
            collision_group.remove(player)
            player.update()
            # move and then check for collisions
            player.move_x()
            while pygame.sprite.spritecollideany(player, collision_group) is not None:
                player.revert_x()

            player.move_y()
            while pygame.sprite.spritecollideany(player, collision_group) is not None:
                player.revert_y()

            collision_group.add(player)

        # update enemies
        for enemy in enemy_group:
            collision_group.remove(enemy)
            enemy.update(playerOne.rect.x, playerOne.rect.y)
            enemy.move_x()
            while pygame.sprite.spritecollideany(enemy, collision_group) is not None:
                enemy.revert_x()

            enemy.move_y()
            while pygame.sprite.spritecollideany(enemy, collision_group) is not None:
                enemy.revert_y()
            collision_group.add(enemy)


        # update bullets
        for bullet in bullet_group:
            bullet.update()
            collided_obj = pygame.sprite.spritecollideany(bullet, collision_group)
            if collided_obj is not None and type(collided_obj) is not Player:
                bullet.kill()
                if type(collided_obj) is not Obstacle:
                    collided_obj.kill()

            """
            if not bullet.update():
                bullet_group.remove(bullet)
            """

        # draw all sprites
        MAIN_DISPLAY.blit(BACKGROUND, (0, 0))

        # MAIN_DISPLAY.blit(playerOne.image, (playerOne.rect.x, playerOne.rect.y))
        player_group.draw(MAIN_DISPLAY)

        '''
        for enemy in enemy_group:
            if enemy is not None:
                MAIN_DISPLAY.blit(enemy.image, (enemy.rect.x, enemy.rect.y))
        '''
        enemy_group.draw(MAIN_DISPLAY)

        '''
        for bullet in bullet_group:
            MAIN_DISPLAY.blit(bullet.image, (bullet.rect.x, bullet.rect.y))
        '''
        bullet_group.draw(MAIN_DISPLAY)

        collision_group.add(obstacle_group, player_group, enemy_group)
        all_sprites.add(obstacle_group, enemy_group, bullet_group, player_group)

        pygame.display.update()
        clock.tick(CLOCK_FPS)

        frame_counter += CLOCK_FPS


if __name__ == '__main__':
    game()
