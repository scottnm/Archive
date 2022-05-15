import pygame, sys, random
from pygame.locals import *

#CONSTANTS
WIN_WIDTH = 600
WIN_HEADER = 40
WIN_HEIGHT = WIN_WIDTH+WIN_HEADER

GAME_WIDTH = WIN_WIDTH
GAME_HEIGHT = WIN_HEIGHT - WIN_HEADER
GAME_BORDER = 5

VEL_X = 10

"""
base control class
does not move or shoot
"""
class npcBASE(object):
    def __init__(self, npc_width, npc_height):
        self.width = npc_width
        self.height = npc_height
        self.x = (WIN_WIDTH-npc_width)/2
        self.y = WIN_HEIGHT - npc_height - GAME_BORDER
        self.vx = VEL_X
        
    def update(self):
        return

    def okToShoot(self):
        return False

"""
moves back and forth
shoots every 30 frames
"""
class npc1(npcBASE):
    def __init__(self, npc_width, npc_height):
        super(npc1, self).__init__(npc_width, npc_height)
        self.shootcounter = 0
        

    def update(self):
        #keeps track of how often this npc can shoot
        self.shootcounter+=1

        #updates npc's position
        new_pos = self.x + self.vx
        if new_pos < GAME_BORDER:
            self.x = GAME_BORDER
            self.vx = -1*self.vx
        elif new_pos > WIN_WIDTH - GAME_BORDER - self.width:
            self.x = WIN_WIDTH - GAME_BORDER - self.width
            self.vx = -1*self.vx
            
        else:
            self.x = new_pos

    def okToShoot(self):
        return self.shootcounter%30 == 0

"""
move right and shoot
shoots every 30 frames
"""
class npc2(npcBASE):
    def __init__(self, npc_width, npc_height):
        super(npc2, self).__init__(npc_width, npc_height)
        self.shootcounter = 0

    def update(self):
        self.shootcounter+=1
        new_pos = self.x + self.vx
        if new_pos < GAME_BORDER:
            self.x = GAME_BORDER
        elif new_pos > WIN_WIDTH - GAME_BORDER - self.width:
            self.x = WIN_WIDTH - GAME_BORDER - self.width            
        else:
            self.x = new_pos

    def okToShoot(self):
        return self.shootcounter%30==0

"""
move left and shoot
shoots every 30 frames
"""
class npc3(npc2):
    def __init__(self, npc_width, npc_height):
        super(npc3, self).__init__(npc_width, npc_height)
        self.vx = -1*self.vx

"""
every 10 frames moves in a random direction
every 30 frames it shoots
"""

class npc4(npcBASE):
    def __init__(self, npc_width, npc_height):
        super(npc4, self).__init__(npc_width, npc_height)
        self.shootcounter = 0
        

    def update(self):
        #keeps track of how often this npc can shoot
        self.shootcounter+=1

        if self.shootcounter%10 == 0:
            if random.random() - 0.5 < 0:
                self.vx = -1*self.vx
            

        #updates npc's position
        new_pos = self.x + self.vx
        if new_pos < GAME_BORDER:
            self.x = GAME_BORDER
            self.vx = -1*self.vx
        elif new_pos > WIN_WIDTH - GAME_BORDER - self.width:
            self.x = WIN_WIDTH - GAME_BORDER - self.width
            self.vx = -1*self.vx
            
        else:
            self.x = new_pos

    def okToShoot(self):
        return self.shootcounter%30 == 0

"""
pauses for an increasing amount of time every 20 frames
decides whether to go left or right after pause
no shooting
"""
class npc5(npcBASE):
    def __init__(self, npc_width, npc_height):
        super(npc5, self).__init__(npc_width, npc_height)
        self.shootcounter = 0
        self.pausecounter = 0
        

    def update(self):
        #keeps track of how often this npc can shoot
        self.shootcounter+=1
        self.pausecounter+=1
        
        if self.shootcounter%20 == 0:
            self.vx = 0   
            
        if self.pausecounter% 50 == 0:
            if random.random()-0.5 > 0:
                self.vx = VEL_X
            else:
                self.vx = -1*VEL_X
            
        
        #updates npc's position
        new_pos = self.x + self.vx
        if new_pos < GAME_BORDER:
            self.x = GAME_BORDER
            self.vx = -1*self.vx
        elif new_pos > WIN_WIDTH - GAME_BORDER - self.width:
            self.x = WIN_WIDTH - GAME_BORDER - self.width
            self.vx = -1*self.vx
            
        else:
            self.x = new_pos

    def okToShoot(self):
        return False

"""
does not move
shoots every 10 frames
"""
class npc6(npcBASE):
    def __init__(self, npc_width, npc_height):
        super(npc6, self).__init__(npc_width, npc_height)
        self.shootcounter = 0

    def update(self):
        self.shootcounter+=1

    def okToShoot(self):
        return self.shootcounter%30 == 0

    
        
    
