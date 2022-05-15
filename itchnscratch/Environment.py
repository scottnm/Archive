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
import Items
import sys
import AccuracyHandler
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
ammo_group = pygame.sprite.Group()
obstacle_group = pygame.sprite.Group()
ObstacleGenerator.generate_obstacle_group(obstacle_group)
playerOne = Player(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, PLAYER_WIDTH, PLAYER_HEIGHT, game_constants.PISTOL_KEY,
                   bullet_group, None, MAIN_DISPLAY)
playerInputProcessor = InputProcessor.KeyboardInputProcessor(playerOne)
player_group = pygame.sprite.Group(playerOne)

# accuracy_handler = AccuracyHandler.AccuracyHandler(MAIN_DISPLAY, playerOne)

player_obstacles = pygame.sprite.Group(obstacle_group, player_group)
enemy_obstacles = pygame.sprite.Group(obstacle_group, enemy_group)
collision_group = pygame.sprite.Group(obstacle_group, player_group, enemy_group)
all_sprites = pygame.sprite.Group(enemy_group, bullet_group, player_group, ammo_group)


def game():
    for i in range(1, 10):
        pygame.time.delay(100)
        print i

    # used to regulate the number of enemies for the time being
    frame_counter = 0
    enemy_counter = 0

    while True:

        # generate enemies
        if frame_counter % (CLOCK_FPS * ENEMY_PERIOD) == 0 and random.random() > .5:
            enemy_group.add(enemies.BasicEnemy())
            enemy_counter += 1

        # generate ammo drop
        if frame_counter % (CLOCK_FPS * AMMO_PERIOD) == 0 and random.random() > .4:
            ammo_group.add(Items.PistolAmmo(6))

        # handle events
        for event in pygame.event.get():
            playerInputProcessor.handle_event(event)

        # update all sprites

        # update players
        for player in player_group:
            # player removed so that no collision with self is detected
            player_obstacles.remove(player)
            player.calculate_rotation()
            player.update()

            # check for ammo grabs
            ammo_collision = pygame.sprite.spritecollideany(player, ammo_group)
            if ammo_collision is not None:
                ammo_collision.kill()
                player.ammo_inventory[ammo_collision.key] += 1

            # move and then check for collisions
            player.move_x()
            while pygame.sprite.spritecollideany(player, player_obstacles) is not None:
                player.revert_x()

            player.move_y()
            while pygame.sprite.spritecollideany(player, player_obstacles) is not None:
                player.revert_y()

            player.revert_from_mouse()

            player_obstacles.add(player)

        # update enemies
        for enemy in enemy_group:
            # enemy is removed so that no collision with self is detected
            enemy_obstacles.remove(enemy)
            enemy.update(playerOne.rect.x, playerOne.rect.y)
            enemy.move_x()
            while pygame.sprite.spritecollideany(enemy, enemy_obstacles) is not None:
                enemy.revert_x()

            enemy.move_y()
            while pygame.sprite.spritecollideany(enemy, enemy_obstacles) is not None:
                enemy.revert_y()
            enemy_obstacles.add(enemy)

        # update player/enemy collisions
        for player in player_group:
            while pygame.sprite.spritecollideany(player, enemy_group) is not None and player.invincibility_frames == 0:
                player.take_damage(game_constants.B_E_COLLISION)
                if player.health <= 0:
                    pygame.quit()
                    sys.exit()

        # update bullets
        for bullet in bullet_group:
            bullet.update()
            collided_obj = pygame.sprite.spritecollideany(bullet, collision_group)
            if collided_obj is not None and type(collided_obj) is not Player:
                bullet.kill()
                if type(collided_obj) is not Obstacle:
                    collided_obj.kill()

        # update Sprite groups
        update_sprite_groups()

        # draw all sprites
        MAIN_DISPLAY.blit(BACKGROUND, (0, 0))
        all_sprites.draw(MAIN_DISPLAY)

        # draw accuracy lines
        playerOne.accuracy_handler.update()
        playerOne.accuracy_handler.draw_lines()

        pygame.display.update()
        clock.tick(CLOCK_FPS)

        frame_counter += CLOCK_FPS

        # pygame.time.delay(500)

def update_sprite_groups():
    collision_group.add(obstacle_group, player_group, enemy_group)
    player_obstacles.add(obstacle_group, player_group)
    enemy_obstacles.add(obstacle_group, enemy_group)
    all_sprites.add(obstacle_group, enemy_group, bullet_group, player_group, ammo_group)

if __name__ == '__main__':
    game()
