import pygame, Weapon, math


class Player( object ):

    #players will move in all directions at a CONSTANT velocity of ____
    VELOCITY = 5

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
    def move(self, walk, direction):
        if( walk ):
            self.move_velocity = direction*Player.VELOCITY
        else:
            self.strafe_velocity = direction*Player.VELOCITY

    #positive rotation goes counterclockwise
    #direction << (-1, 0, 1)
    def rotate(self, direction):
        self.rotation_vel=direction * Player.VELOCITY

    def update(self):
        xmove_factor = math.cos(math.radians(self.rotation))
        ymove_factor = math.sin(math.radians(self.rotation))
        xstrafe_factor = math.cos(math.radians(self.rotation - 90))
        ystrafe_factor = math.sin(math.radians(self.rotation - 90))

        self.x_pos += xmove_factor * self.move_velocity + xstrafe_factor * self.strafe_velocity
        self.y_pos += ymove_factor * self.move_velocity + ystrafe_factor * self.strafe_velocity
        self.rotation+=self.rotation_vel

    #dictionary of equip functions
    equip_select = {None:equipNone, "Pistol":equipPistol}

    def __init__(self, x, y, width, height, weapon, sprite):
        #define position
        self.x_pos = x
        self.y_pos = y

        #rotation in degrees
        #90 faces top of screen 0 faces right of screen
        self.rotation = 90
        self.rotation_vel = 0

        #movement
        self.move_velocity = 0
        self.strafe_velocity = 0

        #define the hitbox of the player
        self.sprite = pygame.Surface((width,height))
        self.sprite.fill((255,0,0)) #fill sprite with red
        self.hitbox = self.sprite.get_rect()


        #spawn weapon
        if weapon == None:
            self.equipNone()
        elif weapon == "Pistol":
            self.equipPistol()

        self.extra_ammo = 0;
