import pygame, sys, random, NeuralNetwork, math
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
    def checkCollision(self, pCPU, pPLAYER):
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

class AiPaddle():
    
    def __init__(self, x, y, width, length, BrainInputWeights, BrainOutputWeights):
        self.x=x
        self.y=y
        #sets the paddles velocity to a constant. this velocity never changes
        #self.vy=PADDLE_VELOCITY

        self.WIDTH=width
        self.LENGTH=length

        #booleans to handle whether a paddle should move up or down
        #self.up=False
        #self.down=False

        self.brain = NeuralNetwork.NeuralNetwork(BrainInputWeights, BrainOutputWeights)

    def getMovement(self, inputs):
        self.brain.feedIn(inputs)
        return self.brain.getOut()

    def update(self, inputs):
        movement = self.getMovement(inputs)
        if movement > 0.0:
            self.y += 15
        elif movement < 0.0:
            self.y -= 15
        if self.y<0:
            self.y = 0
        elif self.y>WINDOW_HEIGHT-PADDLE_LENGTH:
            self.y = WINDOW_HEIGHT-PADDLE_LENGTH

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

def main(BrainInputWeights, BrainOutputWeights):
    #a variable to keep track of the frames
    fps_count=0
    #a variable to track whether someone has recently scored
    has_scored = False

    #displays 
    StartScreen(DS) 

    #creates initial ball
    b = Ball()

    #creates player and computer paddles
    pPLAYER = Paddle(WINDOW_WIDTH-PADDLE_WIDTH, WINDOW_HEIGHT/2-PADDLE_LENGTH/2, PADDLE_WIDTH, PADDLE_LENGTH)
    pCPU = AiPaddle(0, WINDOW_HEIGHT/2-PADDLE_LENGTH/2, PADDLE_WIDTH, PADDLE_LENGTH, BrainInputWeights, BrainOutputWeights)

    
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
                pPLAYER = Paddle(WINDOW_WIDTH-PADDLE_WIDTH, WINDOW_HEIGHT/2-PADDLE_LENGTH/2, PADDLE_WIDTH, PADDLE_LENGTH)
                pCPU = AiPaddle(0, WINDOW_HEIGHT/2-PADDLE_LENGTH/2, PADDLE_WIDTH, PADDLE_LENGTH, BrainInputWeights, BrainOutputWeights)

                sb.scoreCPU+=1
                has_scored = True
                BOOP_SCORE.play()

                #if score limit is reached, jumps to end screen
                if(sb.scorePlayer>=SCORE_LIMIT):
                    WinnerScreen(DS, "You Win :)")
                
        elif b.x<0:
                #resets ball and paddles
                b = Ball()
                pPLAYER = Paddle(WINDOW_WIDTH-PADDLE_WIDTH, WINDOW_HEIGHT/2-PADDLE_LENGTH/2, PADDLE_WIDTH, PADDLE_LENGTH)
                pCPU = AiPaddle(0, WINDOW_HEIGHT/2-PADDLE_LENGTH/2, PADDLE_WIDTH, PADDLE_LENGTH, BrainInputWeights, BrainOutputWeights)

                sb.scorePlayer+=1
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

        #player paddle movement
        if(pPLAYER.up and pPLAYER.y > 0): 
            pPLAYER.y=pPLAYER.y-pPLAYER.vy
            
        elif(pPLAYER.down and pPLAYER.y+PADDLE_LENGTH < WINDOW_HEIGHT):
            pPLAYER.y=pPLAYER.y+pPLAYER.vy

        bias_input = 1
        pCPU.update([[math.fabs(pCPU.x - b.x)/WINDOW_WIDTH, bias_input],[(b.y-(pCPU.y+PADDLE_LENGTH/2))/WINDOW_HEIGHT, bias_input],[pCPU.y/WINDOW_HEIGHT, bias_input]])

        #FPS Count to keep track of when the ai paddle should act
        fps_count+=1


        
main([[3.3248809447847867, 1.8039809008526004], [2.5592003891390784, 1.3687348367777024], [8.77194825891814, 7.213421392745152]], [[0.9847912286111957, 9.860143520138013, 3.448344936944589, -3.108238320675574]])

