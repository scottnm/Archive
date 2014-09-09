
import pygame, sys, random, NeuralNetwork, math
from pygame.locals import *

#Global Variables and Constants
WINDOW_WIDTH = 700
WINDOW_HEIGHT = 700
PADDLE_WIDTH = 10
PADDLE_LENGTH = 60
PADDLE_VELOCITY = 15
BLACK = (0,0,0)
WHITE = (255,255,255)
DISPLAY_GAME = True


pygame.mixer.init()
BOOP_X = pygame.mixer.Sound('CollisionX.wav')
BOOP_Y = pygame.mixer.Sound('CollisionY.wav')
BOOP_SCORE = pygame.mixer.Sound('Score.wav')

class AiPaddle():
    
    def __init__(self, x, y, width, length, BrainInputWeights, BrainOutputWeights):
        #sets paddles initial position
        self.x=x
        self.y=y
        #sets dimensions of the paddle
        self.WIDTH=width
        self.LENGTH=length
        #creates a brain for the paddle
        self.brain = NeuralNetwork.NeuralNetwork(BrainInputWeights, BrainOutputWeights)

    #feeds in the current state of the game
    #returns the appropriate response
    def getMovement(self, inputs):
        self.brain.feedIn(inputs)
        return self.brain.getOut()

    def update(self, inputs):
        #gets how the paddle should move
        movement = self.getMovement(inputs)

        
        #if positive move down
        if movement > 0.0:
            self.y += 30
        #else move up
        else:
            self.y -= 30

        #keeps the paddle from going out of bounds
        if self.y<0:
            self.y = 0
        elif self.y>WINDOW_HEIGHT-PADDLE_LENGTH:
            self.y = WINDOW_HEIGHT-PADDLE_LENGTH

    #draws the paddles on the window
    def draw(self, DS):
        pygame.draw.rect(DS, WHITE, pygame.Rect(self.x, self.y, self.WIDTH, self.LENGTH))


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
        #adds 1 to the balls x speed
        if(self.vx>0):
            self.vx=self.vx+1
        else:
            self.vx=self.vx-1

        #plays collision sound
        if DISPLAY_GAME:
            BOOP_X.play()

        #reverses velocity
        self.vx = -self.vx

    #handles when a ball collides with one of the walls
    def collideY(self):
        #adds 1 to the balls y speed
        if(self.vy>0):
            self.vy = self.vy+1
        else:
            self.vy = self.vy-1
        
        #plays collision sound
        if DISPLAY_GAME:
            BOOP_Y.play()

        #reverses y velocity
        self.vy = -self.vy

    #handles collision checking
    def checkCollision(self, pLEARNER):
        #checks if a ball has hit the top or bottom wall
        if(self.y-self.r <= 0 or self.y+self.r >= WINDOW_HEIGHT):
            self.collideY()
        #checks if a ball has collided with one of the paddles
        if((self.x-self.r <= PADDLE_WIDTH and (self.y+self.r >= pLEARNER.y and self.y-self.r <= pLEARNER.y+PADDLE_LENGTH)) or (self.x+self.r >= WINDOW_WIDTH-PADDLE_WIDTH)):
            self.collideX()

    #changes the balls position based on the balls current velocity
    def update(self):
        #if moving would cause the ball to essentially jump over an obstacle
        #place block at position where obstacle could intercept it
        if self.x != PADDLE_WIDTH and self.x+self.vx < PADDLE_WIDTH:
            self.x = PADDLE_WIDTH
        elif self.x != WINDOW_WIDTH-PADDLE_WIDTH and self.x+self.vx > WINDOW_WIDTH-PADDLE_WIDTH:
            self.x = WINDOW_WIDTH-PADDLE_WIDTH
        #else update ball as normal
        else:
            self.x = self.x+self.vx

        self.y = self.y+self.vy


#plays a round and returns the fitness of the paddle in that round
def PlayRound(BrainInputWeights, BrainOutputWeights, display):
    #creates the display it intended
    global DISPLAY_GAME
    DISPLAY_GAME = display
    if(DISPLAY_GAME):
        #create pygame window
        pygame.init()
        DS = pygame.display.set_mode((WINDOW_WIDTH, WINDOW_HEIGHT))
        pygame.display.set_caption("Pong")
        FPS = 120
        FPSCLOCK = pygame.time.Clock()

        
    
    #create ai_paddle
    pLearner = AiPaddle(0, WINDOW_HEIGHT/2-PADDLE_LENGTH/2, PADDLE_WIDTH, PADDLE_LENGTH, BrainInputWeights, BrainOutputWeights)
    #creates the ball
    b = Ball()
    #initialize a beginning fitness
    fitness = 0

    #GameLoop
    while True:
        #Check for Collisions and Update
        b.checkCollision(pLearner)
        b.update()
        
        #If choosing to display, draw
        if(DISPLAY_GAME):
            #DrawsBackground
            DS.fill(BLACK)
            #Draws Median Line
            pygame.draw.line(DS, WHITE, (WINDOW_WIDTH/2,0), (WINDOW_WIDTH/2, WINDOW_HEIGHT))
            #Draws Paddles
            pLearner.draw(DS)
            #Draw Wall
            pygame.draw.rect(DS, WHITE, pygame.Rect(WINDOW_WIDTH-PADDLE_WIDTH, 0, PADDLE_WIDTH, WINDOW_HEIGHT))
            #Draw Ball
            b.draw(DS)
            #update the display
            pygame.display.update()
            FPSCLOCK.tick(FPS)
    
        #check for scoring
        if b.x < PADDLE_WIDTH:
            #if someone has scored, quit game and return score
            pygame.quit()
            return fitness
           
        bias_input = 1
        #move the AiPAddle
        pLearner.update([[math.fabs(pLearner.x - b.x)/WINDOW_WIDTH, bias_input],[(b.y-(pLearner.y+PADDLE_LENGTH/2))/WINDOW_HEIGHT, bias_input],[pLearner.y/WINDOW_HEIGHT, bias_input]])
        
        #increment fitness for every run through the loop
        fitness += 1
