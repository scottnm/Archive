import pygame, sys, PR
from pygame.locals import *
from npc import *

#CONSTANTS
WIN_WIDTH = 600
WIN_HEADER = 40
WIN_HEIGHT = WIN_WIDTH+WIN_HEADER

GAME_WIDTH = WIN_WIDTH
GAME_HEIGHT = WIN_HEIGHT - WIN_HEADER
GAME_BORDER = 5

BLACK = (0,0,0)
WHITE = (255, 255, 255)
BOSS_GREEN = (0, 200, 122)
RED = (255, 0, 0)

MAX_FRAMES = 1500
DISPLAY = True
DISPLAY_SURF = None

BOSS_VX = 6
BULLET_VY = 12



class bullet():
    def __init__(self, init_x, init_y, init_vy):
        #hardcoded bullet dimensions
        self.width = 5
        self.height = 10
        if DISPLAY:
            self.shape = pygame.Surface((self.width, self.height)).convert()
            
        self.Rect = PR.Rect((init_x, init_y), (init_x+self.width, init_y+self.height))
        self.vy = init_vy
        self.vx = 0
        self.x  = init_x
        self.y = init_y
        
    def update(self):
        self.y+=self.vy
        self.Rect = PR.Rect((self.x, self.y), (self.x+self.width, self.y+self.height))

    def checkInBounds(self):
        return self.y > WIN_HEADER and self.y < WIN_HEIGHT

    def draw(self, DS):
        self.shape.fill(WHITE)
        DS.blit(self.shape, (self.x, self.y))
        
        
class npcChar():
    def __init__(self, player):
        
        self.x = player.x
        self.y = player.y
        self.brain = player
        if DISPLAY:
            self.shape = pygame.Surface((player.width, player.height)).convert()
        self.Rect = PR.Rect((self.x, self.y), (self.x+player.width, self.y+player.height))
        self.health = 100

    def update(self):
        self.brain.update()
        self.x = self.brain.x
        self.Rect = PR.Rect((self.x, self.y), (self.x+self.brain.width, self.y+self.brain.height))


    def shoot(self, bulletlist):
        if self.brain.okToShoot():
            bulletlist.append( bullet( self.x+self.brain.width/2, self.brain.y, -1*BULLET_VY ) )

    def checkCollisions(self, bulletlist):
        for bullet in bulletlist:
            #if bullet.x >= self.x and bullet.x <= self.x+self.brain.width and bullet.y+bullet.height >= self.y+self.brain.height:
            if self.Rect.overlaps(bullet.Rect):
                self.health-=10
                bulletlist.remove(bullet)

    def draw(self, DS):
        self.shape.fill(RED)
        DS.blit(self.shape, (self.x, self.y))
        
  
class BossChar():
    def __init__(self, boss_width, boss_height):
        self.width = boss_width
        self.height = boss_height
        self.x = (WIN_WIDTH - self.width)/2
        self.y = WIN_HEADER+GAME_BORDER
        if DISPLAY:
            self.shape = pygame.Surface((self.width, self.height)).convert()
        self.Rect = PR.Rect((self.x, self.y), (self.x+self.width, self.y+self.height))
        self.health = 100

    def checkCollisions(self, bulletlist):
        for bullet in bulletlist:
            #if bullet.x >= self.x and bullet.x <= self.x+self.width and bullet.y <= self.y+self.height:
            if self.Rect.overlaps(bullet.Rect):
               self.health -= 10
               bulletlist.remove(bullet)

    def update(self, movement):
        #2 cases for input
        if(movement > 0.5):
           vx = BOSS_VX
        else:
            vx = -1 * BOSS_VX
             
        new_pos = self.x + vx
        if new_pos < GAME_BORDER:
            self.x = GAME_BORDER
        elif new_pos > WIN_WIDTH - GAME_BORDER - self.width:
            self.x = WIN_WIDTH - GAME_BORDER - self.width
        else:
            self.x = new_pos

        self.Rect = self.Rect = PR.Rect((self.x, self.y), (self.x+self.width, self.y+self.height))
        #1 case moves it to the left
        #other case moves it to the right

    def shoot(self, trigger, bulletlist):
        if len(bulletlist) < 5 and trigger:
            bulletlist.append(bullet(self.x+self.width/2, self.y+self.height, BULLET_VY))
    
    def draw(self, DS):
        self.shape.fill(BOSS_GREEN)
        DS.blit(self.shape, (self.x, self.y))

        
def drawBackground(DISPLAY_SURF):
    DISPLAY_SURF.fill(BLACK)
    pygame.draw.rect(DISPLAY_SURF, WHITE, pygame.Rect(0, WIN_HEADER, GAME_WIDTH, GAME_HEIGHT), GAME_BORDER)

def checkBullets(bulletlist):
    for bullet in bulletlist:
        if(bullet.checkInBounds()):
            if DISPLAY:
                bullet.draw(DISPLAY_SURF)
            bullet.update()
        else:
            bulletlist.remove(bullet)


  
def game(doDisplay, player, network):

    frame_counter = 0
    
    global DISPLAY
    global DISPLAY_SURF
    DISPLAY = doDisplay
    if DISPLAY:
        pygame.init()
        DISPLAY_SURF = pygame.display.set_mode((WIN_WIDTH, WIN_HEIGHT))
        pygame.display.set_caption("boss simulator")
        FPS = 60
        FPSCLOCK = pygame.time.Clock()

    npc = npcChar(player)
    npc_bulletlist=[]
    boss = BossChar(40, 40)
    boss_bulletlist=[]
    score = 0
    
    #GameLoop
    while frame_counter < MAX_FRAMES and boss.health > 0 and npc.health > 0:
        """
        send inputs to neural network
        """
        #list of output: [move, shoot] ouputs
        inputs = [(npc.x*1.0 - boss.x*1.0)/WIN_WIDTH]
        if len( npc_bulletlist ) > 0:
            closest_bullet_pos = npc_bulletlist[0].x
        else:
            closest_bullet_pos = -1
        inputs.append(boss.x*1.0/WIN_WIDTH)
        inputs.append(closest_bullet_pos*1.0/WIN_WIDTH)
        inputs.append(boss.health*1.0/100)
        inputs.append(1)#bias
        output = network.processNetwork(inputs)

        """
        use output of neural network to move
        """
        npc.update()
        boss.update(output[0])
        npc.shoot(npc_bulletlist)
        boss.shoot(output[1] > 0.5, boss_bulletlist)
        
        
        #check bullet collisions
        boss.checkCollisions(npc_bulletlist)
        npc.checkCollisions(boss_bulletlist)
        
          
        if DISPLAY:
            #update screen
            drawBackground(DISPLAY_SURF)
            npc.draw(DISPLAY_SURF)
            boss.draw(DISPLAY_SURF)

            #draw the score
            scoreboard(DISPLAY_SURF, boss.health, npc.health)                  
            

        checkBullets(npc_bulletlist)
        checkBullets(boss_bulletlist)

        
            
        if DISPLAY:
            for event in pygame.event.get():
                if event.type == QUIT:
                    pygame.quit()
                    return
            pygame.display.update()

        
            
        if npc.health <= 0:
            if DISPLAY:
                pygame.quit()
            return (MAX_FRAMES-frame_counter) + (100-boss.health)
        
        if boss.health <= 0:
            if DISPLAY:
                pygame.quit()
            return -1 * (MAX_FRAMES-frame_counter) + (100-npc.health)
        
        frame_counter+=1
    
    #return score
    if DISPLAY:
        pygame.quit()
    return -1*MAX_FRAMES
          

def scoreboard(DISPLAY_SURF, bosshealth, npchealth):
    bosshealth_text = pygame.font.Font('freesansbold.ttf', 40)
    bosshealth_surf = bosshealth_text.render("BOSS: "+str(bosshealth), True, WHITE)
    bosshealth_rect = bosshealth_surf.get_rect()
    bosshealth_rect.topleft = (0,0)
            
            
    npchealth_text = pygame.font.Font('freesansbold.ttf',40)
    npchealth_surf = npchealth_text.render("NPC: "+str(npchealth), True, WHITE)
    npchealth_rect = npchealth_surf.get_rect()
    npchealth_rect.topright = (WIN_WIDTH, 0)

    DISPLAY_SURF.blit(bosshealth_surf, bosshealth_rect)
    DISPLAY_SURF.blit(npchealth_surf, npchealth_rect)

def main():
    #plays a round of the simulation displayed
    print game(False, npc6(20,20), network(), 1000)
    sys.exit()
    

if __name__ == "__main__":
    main()
