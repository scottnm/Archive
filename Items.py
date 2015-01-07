class Item(pygame.sprite.Sprite):
    #parent class of all items
    def __init__(self):
        pygame.sprite.Sprite.__init__(self)

class Ammo(Item):
    #generic ammo class
    def __init__(self):
        Item.__init__(self)

class PistolAmmo(Ammo):
    def __init__(self, quantity):
        Ammo.__init__(self)
        self.quantity = quantity