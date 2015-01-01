import pygame, Weapon, math, game_constants

class Player( object ):

    #players will move in all directions at a CONSTANT velocity of ____

    def equipNone(self):
        self.weapon = None

    def equipPistol(self):
        self.weapon = Weapon.Pistol()

    def shoot(self):
        if self.weapon != None:
            self.weapon.fire()

    def reload(self):
        if(self.weapon != None and self.extra_ammo > 0):
            self.weapon.reload()
            self.extra_ammo -= 1

    '''
    walk = True if walking, False if strafing
    direction = > 0 if forward; < 0 if backwards; == 0 if still
    '''
    def move(self, direction):
        self.velocity = direction*game_constants.PLAYER_VELOCITY

    def update(self):
        mousepos = pygame.mouse.get_pos()
        delY =  mousepos[1] - self.y
        delX = mousepos[0] - self.x

        #handles division by zero
        if delX == 0:
            if delY < 0:
                self.rotation = -1*math.pi
            else:
                self.rotation = math.pi
        else:
            self.rotation = math.atan( delY / delX )
            if delX < 0:
                self.rotation += math.pi

        self.x += self.velocity * math.cos( self.rotation )
        self.y += self.velocity * math.sin( self.rotation )

    def __init__(self, x, y, width, height, weapon, sprite):
        #define position
        self.x = x
        self.y = y

        #rotation in degrees
        #90 faces top of screen 0 faces right of screen
        self.rotation = math.pi/2

        #movement
        self.velocity = 0

        #define the hitbox of the player
        self.sprite = pygame.Surface((width,height))
        self.sprite.fill(game_constants.RED) #fill sprite with red
        self.hitbox = self.sprite.get_rect()


        #spawn weapon
        if weapon == None:
            self.equipNone()
        elif weapon == "Pistol":
            self.equipPistol()

        self.extra_ammo = 0;
