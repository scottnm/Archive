import pygame, sys, random
from pygame.locals import *

#Global Variables and Constants
WINDOW_WIDTH = 700
WINDOW_HEIGHT = 700
PADDLE_WIDTH = 10
PADDLE_LENGTH = 60
PADDLE_VELOCITY = 15
FONTSIZE = 30
BLACK = (0,0,0)
WHITE = (255,255,255)
SCORE_LIMIT=10

#create pygame window
pygame.init()
DS = pygame.display.set_mode((WINDOW_WIDTH, WINDOW_HEIGHT))
pygame.display.set_caption("Pong")
FPS = 30
FPSCLOCK = pygame.time.Clock()

#create sounds 
pygame.mixer.init()
BOOP_X = pygame.mixer.Sound('CollisionX.wav')
BOOP_Y = pygame.mixer.Sound('CollisionY.wav')
BOOP_SCORE = pygame.mixer.Sound('Score.wav')


class Ball():
    def __init__(self):
        #places the ball at the center of the screen whenever a new ball is made
        self.x = WINDOW_WIDTH/2
        self.y = WINDOW_HEIGHT/2
        self.r = 10

        #initializes the balls X and Y velocity
            #x velocity can be either -7 or 7
            #y velocity can be any value within the domain [-5,2]U[2,5]
        self.directionX = [-1,1]
        self.vx = 7 * self.directionX[random.randint(0,1)]
        self.vy = random.randint(2,5)*self.directionX[random.randint(0,1)]

    #draws the ball at its current position
    def draw(self, DS):
        pygame.draw.circle(DS, WHITE, (self.x,self.y), self.r)

    #handles when a ball collides with a paddle
    def collideX(self):
        #adds 2 to the balls x speed
        if(self.vx>0):
            self.vx=self.vx+2
        else:
            self.vx=self.vx-2
        #plays collision sound
        BOOP_X.play()

        #reverses velocity
        self.vx = -self.vx

    #handles when a ball collides with one of the walls
    def collideY(self):
        #adds 2 to the balls y speed
        if(self.vy>0):
            self.vy = self.vy+2
        else:
            self.vy = self.vy-2
        #plays collision sound
        BOOP_Y.play()

        #reverses y velocity
        self.vy = -self.vy

    #handles collision checking
    def checkCollision(self, pPLAYER, pCPU):
        #checks if a ball has hit the top or bottom wall
        if(self.y-self.r <= 0 or self.y+self.r >= WINDOW_HEIGHT):
            self.collideY()
        #checks if a ball has collided with one of the paddles
        if((self.x-self.r <= PADDLE_WIDTH and (self.y+self.r >= pPLAYER.y and self.y-self.r <= pPLAYER.y+PADDLE_LENGTH)) or (self.x+self.r >= WINDOW_WIDTH-PADDLE_WIDTH and (self.y+self.r >= pCPU.y and self.y-self.r <= pCPU.y+PADDLE_LENGTH))):
            self.collideX()

    #changes the balls position based on the balls current velocity
    def update(self):
        self.x = self.x+self.vx
        self.y = self.y+self.vy


class Paddle():
    
    def __init__(self, x, y, width, length):
        self.x=x
        self.y=y
        #sets the paddles velocity to a constant. this velocity never changes
        self.vy=PADDLE_VELOCITY

        self.WIDTH=width
        self.LENGTH=length

        #booleans to handle whether a paddle should move up or down
        self.up=False
        self.down=False

    #draws the paddles on the window
    def draw(self, DS):
        pygame.draw.rect(DS, WHITE, pygame.Rect(self.x, self.y, self.WIDTH, self.LENGTH))


class ScoreBoard():

    def __init__(self):
        #initializes the both scores to 0
        self.scorePlayer = 0
        self.scoreCPU = 0

        #creates a font to display the scores with
        self.myFont = pygame.font.SysFont("Verdana", FONTSIZE)

    def update(self, DS):
        #creates the two text images for each score
        self.scorePlayerIMG = self.myFont.render(str(self.scorePlayer), 0, WHITE)
        self.scoreCPUIMG = self.myFont.render(str(self.scoreCPU), 0, WHITE)

        #blits the scores to the windows surface
        DS.blit(self.scorePlayerIMG, (WINDOW_WIDTH/4 - FONTSIZE/2, 10))
        DS.blit(self.scoreCPUIMG, (3 * WINDOW_WIDTH/4 - FONTSIZE/2, 10))

#a definition to display the start screen
def StartScreen(DS):
    FONTSIZE=20
    DS.fill(BLACK)
    #creates a font for the start text
    startFont = pygame.font.SysFont("Verdana", FONTSIZE)

    #creates the start to text 
    startText = startFont.render("Welcome to PONG! Press spacebar key to begin!", 0 , WHITE)

    #blits the start text to the window
    DS.blit(startText, (WINDOW_WIDTH/6,WINDOW_HEIGHT/2-10))

    pygame.display.update()
    startscreen = True
    while startscreen:
        for event in pygame.event.get():
            if event.type==pygame.KEYDOWN and event.key==pygame.K_SPACE:
                startscreen=False
            elif event.type == QUIT:
                pygame.quit()
                sys.exit()
#a definition to display the end screen
def WinnerScreen(DS, s):
    DS.fill(BLACK)
    winnerFont = pygame.font.SysFont("Verdana", FONTSIZE)
    winner = winnerFont.render(s, 0, WHITE)
    DS.blit(winner, (WINDOW_WIDTH/3, WINDOW_HEIGHT/2-10))

    pygame.display.update()
    while True:
        for event in pygame.event.get():
            if event.type==QUIT:
                pygame.quit()
                sys.exit()

def main():
    #a variable to keep track of the frames
    fps_count=0
    #a variable to track whether someone has recently scored
    has_scored = False

    #displays 
    StartScreen(DS) 

    #creates initial ball
    b = Ball()

    #creates player and computer paddles
    pPLAYER = Paddle(0, WINDOW_HEIGHT/2-PADDLE_LENGTH/2, PADDLE_WIDTH, PADDLE_LENGTH)
    pCPU = Paddle(WINDOW_WIDTH-PADDLE_WIDTH, WINDOW_HEIGHT/2-PADDLE_LENGTH/2, PADDLE_WIDTH, PADDLE_LENGTH)
    
    sb = ScoreBoard()
    
    #Game Loop
    while True:

        #DrawsBackground
        DS.fill(BLACK)
        #Draws Median Line
        pygame.draw.line(DS, WHITE, (WINDOW_WIDTH/2,0), (WINDOW_WIDTH/2, WINDOW_HEIGHT))

        sb.update(DS)
        
        #Draws Paddles
        pPLAYER.draw(DS)
        pCPU.draw(DS)

        #Check for Collisions and Update
        b.checkCollision(pPLAYER, pCPU)
        b.update()
        
        #Draw Ball
        b.draw(DS)

        #update the display
        pygame.display.update()
        FPSCLOCK.tick(FPS)
        if(has_scored):
            has_scored=False
            pygame.time.delay(700)

        #Checks for scoring
        if b.x>WINDOW_WIDTH:
                #resets ball and paddles
                b = Ball()
                pPLAYER = Paddle(0, WINDOW_HEIGHT/2-PADDLE_LENGTH/2, PADDLE_WIDTH, PADDLE_LENGTH)
                pCPU = Paddle(WINDOW_WIDTH-PADDLE_WIDTH, WINDOW_HEIGHT/2-PADDLE_LENGTH/2, PADDLE_WIDTH, PADDLE_LENGTH)

                sb.scorePlayer+=1
                has_scored = True
                BOOP_SCORE.play()

                #if score limit is reached, jumps to end screen
                if(sb.scorePlayer>=SCORE_LIMIT):
                    WinnerScreen(DS, "You Win :)")
                
        elif b.x<0:
                #resets ball and paddles
                b = Ball()
                pPLAYER = Paddle(0, WINDOW_HEIGHT/2-PADDLE_LENGTH/2, PADDLE_WIDTH, PADDLE_LENGTH)
                pCPU = Paddle(WINDOW_WIDTH-PADDLE_WIDTH, WINDOW_HEIGHT/2-PADDLE_LENGTH/2, PADDLE_WIDTH, PADDLE_LENGTH)

                sb.scoreCPU+=1
                has_scored = True
                BOOP_SCORE.play()

                #if score limit is reached, jumps to end screen
                if(sb.scoreCPU>=SCORE_LIMIT):
                    WinnerScreen(DS, "You Lose :(")

        #Checks for input from user
        for event in pygame.event.get():
            
            #checks for if a key is pressed
            if event.type==pygame.KEYDOWN and event.key==pygame.K_w:
                pPLAYER.up=True
            if event.type==pygame.KEYDOWN and event.key==pygame.K_s:
                pPLAYER.down=True

            #checks for if a key is released
            if event.type==pygame.KEYUP and event.key==pygame.K_w:
                pPLAYER.up=False
            if event.type==pygame.KEYUP and event.key==pygame.K_s:
                pPLAYER.down=False
            
            #checks if the window is being closed
            if event.type==QUIT:
                pygame.quit()
                sys.exit()

        #ai paddle movement state
        #ai paddle only looks whether to change movement every 1/15 of a second
        #ai paddle checks whether the ball's y position is higher or lower and will plan to move accordingly
        if(fps_count % 3 == 0 and b.y < pCPU.y-10):
            pCPU.up = True
            pCPU.down = False

        elif(fps_count % 3 == 0 and b.y > pCPU.y+PADDLE_LENGTH+10):
            pCPU.up = False
            pCPU.down = True

        #ai paddle only looks to pause every 2/15 of a second
        elif(fps_count % 12 == 0):
            pCPU.up = False
            pCPU.down = False


        #player paddle movement
        if(pPLAYER.up and pPLAYER.y > 0): 
            pPLAYER.y=pPLAYER.y-pPLAYER.vy
            
        elif(pPLAYER.down and pPLAYER.y+PADDLE_LENGTH < WINDOW_HEIGHT):
            pPLAYER.y=pPLAYER.y+pPLAYER.vy

        #ai paddle movement
        if(pCPU.up and pCPU.y > 0):
            pCPU.y = pCPU.y-pCPU.vy
        elif(pCPU.down and pCPU.y+PADDLE_LENGTH < WINDOW_HEIGHT):
            pCPU.y = pCPU.y+pCPU.vy

        #FPS Count to keep track of when the ai paddle should act
        fps_count+=1


        
main()

