import pygame
import random
from game_constants import *

class Item(pygame.sprite.Sprite):
    # parent class of all items
    def __init__(self):
        pygame.sprite.Sprite.__init__(self)

class Ammo(Item):
    # generic ammo class
    def __init__(self):
        Item.__init__(self)

class PistolAmmo(Ammo):
    def __init__(self, quantity):
        Ammo.__init__(self)
        self.quantity = quantity
        self.image = pygame.Surface((PISTOL_AMMO_WIDTH, PISTOL_AMMO_HEIGHT))
        self.image.fill(YELLOW)
        self.rect = self.image.get_rect()
        self.key = PISTOL_KEY

        # initialized to a random location
        self.rect.x = random.random()*(WINDOW_WIDTH-PISTOL_AMMO_WIDTH)
        self.rect.y = random.random()*(WINDOW_HEIGHT-PISTOL_AMMO_HEIGHT)