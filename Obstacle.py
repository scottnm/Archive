import pygame
import game_constants


class Obstacle(pygame.sprite.Sprite):

    def __init__(self, x, y, width, height):
        pygame.sprite.Sprite.__init__(self)

        self.image = pygame.Surface((width, height))
        self.image.fill(game_constants.WHITE)
        self.rect = self.image.get_rect()
        self.rect.x = x
        self.rect.y = y
