import pygame, math, random, game_constants

class Enemy( object ):
    #movement skeleton needed for each enemy
    def move( self ):
        raise Exception("All enemies must implement 'move()'")

    #update skeleton
    def update( self ):
        raise Exception("All enemies must implement 'update()'")

    def __init__( self ):
        raise Exception("All enemies must implement '__init__()'")


class BasicEnemy( Enemy ):

    def move( self, target_x, target_y ):
        if self.x > target_x:
            self.x_vel = -1*game_constants.BASIC_ENEMY_VELOCITY
        elif self.x < target_x:
            self.x_vel = game_constants.BASIC_ENEMY_VELOCITY
        else:
            self.x_vel = 0

        if self.y > target_y:
            self.y_vel = -1*game_constants.BASIC_ENEMY_VELOCITY
        elif self.y < target_y:
            self.y_vel = game_constants.BASIC_ENEMY_VELOCITY
        else:
            self.x_vel = 0

    def update( self ):
        self.x += self.x_vel
        self.y += self.y_vel
        

    def __init__( self ):
        self.x = random.randint(0, game_constants.WINDOW_WIDTH - game_constants.BASIC_ENEMY_WIDTH)
        self.y = random.randint(0, game_constants.WINDOW_HEIGHT - game_constants.BASIC_ENEMY_HEIGHT)

        self.x_vel = 0
        self.y_vel = 0

        self.sprite = pygame.Surface((game_constants.BASIC_ENEMY_WIDTH, game_constants.BASIC_ENEMY_HEIGHT))
        self.sprite.fill(game_constants.GREEN)
        self.hitbox = self.sprite.get_rect()
