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
FPS = 120
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
        if self.x != PADDLE_WIDTH and self.x+self.vx < PADDLE_WIDTH:
            self.x = PADDLE_WIDTH
        elif self.x != WINDOW_WIDTH-PADDLE_WIDTH and self.x+self.vx > WINDOW_WIDTH-PADDLE_WIDTH:
            self.x = WINDOW_WIDTH-PADDLE_WIDTH
        else:
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
        self.scoreCPU1 = 0
        self.scoreCPU2 = 0

        #creates a font to display the scores with
        self.myFont = pygame.font.SysFont("Verdana", FONTSIZE)

    def update(self, DS):
        #creates the two text images for each score
        self.scoreCPU1IMG = self.myFont.render(str(self.scoreCPU1), 0, WHITE)
        self.scoreCPU2IMG = self.myFont.render(str(self.scoreCPU2), 0, WHITE)

        #blits the scores to the windows surface
        DS.blit(self.scoreCPU1IMG, (WINDOW_WIDTH/4 - FONTSIZE/2, 10))
        DS.blit(self.scoreCPU2IMG, (3 * WINDOW_WIDTH/4 - FONTSIZE/2, 10))

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

def main(BrainInputWeights1, BrainOutputWeights1, BrainInputWeights2, BrainOutputWeights2):
    #a variable to keep track of the frames
    fps_count=0
    #a variable to track whether someone has recently scored
    has_scored = False

    #displays 
    StartScreen(DS) 

    #creates initial ball
    b = Ball()

    #creates player and computer paddles
    pCPU1 = AiPaddle(0, WINDOW_HEIGHT/2-PADDLE_LENGTH/2, PADDLE_WIDTH, PADDLE_LENGTH, BrainInputWeights1, BrainOutputWeights1)
    pCPU2 = AiPaddle(WINDOW_WIDTH-PADDLE_WIDTH, WINDOW_HEIGHT/2-PADDLE_LENGTH/2, PADDLE_WIDTH, PADDLE_LENGTH, BrainInputWeights2, BrainOutputWeights2)

    
    sb = ScoreBoard()
    
    #Game Loop
    while True:

        #DrawsBackground
        DS.fill(BLACK)
        #Draws Median Line
        pygame.draw.line(DS, WHITE, (WINDOW_WIDTH/2,0), (WINDOW_WIDTH/2, WINDOW_HEIGHT))

        sb.update(DS)
        
        #Draws Paddles
        pCPU1.draw(DS)
        pCPU2.draw(DS)

        #Check for Collisions and Update
        b.checkCollision(pCPU1, pCPU2)
        b.update()
        
        #Draw Ball
        b.draw(DS)
        if(has_scored):
            pygame.time.delay(700)
            has_scored=False
        #update the display
        pygame.display.update()
        FPSCLOCK.tick(FPS)
        
            

        #Checks for scoring
        if b.x>WINDOW_WIDTH:
                #resets ball and paddles
                b = Ball()
                pCPU1 = AiPaddle(WINDOW_WIDTH-PADDLE_WIDTH, WINDOW_HEIGHT/2-PADDLE_LENGTH/2, PADDLE_WIDTH, PADDLE_LENGTH, BrainInputWeights1, BrainOutputWeights1)
                pCPU2 = AiPaddle(0, WINDOW_HEIGHT/2-PADDLE_LENGTH/2, PADDLE_WIDTH, PADDLE_LENGTH, BrainInputWeights2, BrainOutputWeights2)

                sb.scoreCPU1+=1
                has_scored = True
                BOOP_SCORE.play()

                #if score limit is reached, jumps to end screen
                if(sb.scoreCPU1>=SCORE_LIMIT):
                    WinnerScreen(DS, "Player 1 Wins")
                
        elif b.x<0:
                #resets ball and paddles
                b = Ball()
                pCPU1 = AiPaddle(WINDOW_WIDTH-PADDLE_WIDTH, WINDOW_HEIGHT/2-PADDLE_LENGTH/2, PADDLE_WIDTH, PADDLE_LENGTH, BrainInputWeights1, BrainOutputWeights1)
                pCPU2 = AiPaddle(0, WINDOW_HEIGHT/2-PADDLE_LENGTH/2, PADDLE_WIDTH, PADDLE_LENGTH, BrainInputWeights2, BrainOutputWeights2)

                sb.scoreCPU2+=1
                has_scored = True
                BOOP_SCORE.play()

                #if score limit is reached, jumps to end screen
                if(sb.scoreCPU2>=SCORE_LIMIT):
                    WinnerScreen(DS, "Player 2 Wins")
        bias_input = 1
        pCPU1.update([[math.fabs(pCPU1.x - b.x)/WINDOW_WIDTH, bias_input],[(b.y-(pCPU1.y+PADDLE_LENGTH/2))/WINDOW_HEIGHT, bias_input],[pCPU1.y/WINDOW_HEIGHT, bias_input]])

        bias_input = 1
        pCPU2.update([[math.fabs(pCPU2.x - b.x)/WINDOW_WIDTH, bias_input],[(b.y-(pCPU2.y+PADDLE_LENGTH/2))/WINDOW_HEIGHT, bias_input],[pCPU2.y/WINDOW_HEIGHT, bias_input]])

        #FPS Count to keep track of when the ai paddle should act
        fps_count+=1


        
main([[-3.9233667573223263, 2.0802775723310645], [5.203673196883955, 5.485696727600113], [-2.9255378433533803, 5.856079006642252]], [[0.3681877520970689, 6.187791566985626, -2.151425254324362, 7.176196910217507]],[[-0.9416323283556023, 2.1905187873468304], [-7.021492669643725, -2.9020883459155478], [6.978178066912278, 3.6835213012032346]], [[-2.2607579003791196, -2.7122720005926935, 0.826685873736906, 2.2159869011187685]])

