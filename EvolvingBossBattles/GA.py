import npc, random
from neuron import network

#CONSTANTS
POPSIZE = 100
NUM_GENERATIONS = 100
NUM_BEST_PARENTS = 5
LOCAL_OPT = 20
bestArr=[]
stuckCount=0

#init empty population list
pop = []
#for range popsize create random individual
for x in range(POPSIZE):
    pop.append(network())
    
#initialize best to empty
best = pop[0]

#repeat
for gen in range(NUM_GENERATIONS):
    #for each indiviual
    for x in range(len(pop)):
        pop[x].getFitness(False)

    oldbest = best
    pop.sort(key = lambda x: x.fitness, reverse = True)
    if best == None or best.fitness < pop[0].fitness:
        best = pop[0]
    if(best == oldbest):
        stuckCount+=1

    if gen%100 == 0 and gen != 0:
        print 'play'
        best.play(True)
       
    #initialize empty list to hold next generation
    nextGen = []
    pop = pop[0:NUM_BEST_PARENTS]
    #pop = pop[0:POPSIZE/5]
    
    #for half the population size
    #for x in range (NUM_BEST_PARENTS):
    if(stuckCount > LOCAL_OPT):
        print "new random children"
        best.play(True)
        bestArr.append(best)
        best = None
        stuckCount=0
        for x in range(POPSIZE):
            nextGen.append(network())
    else:
        for x in range(POPSIZE/2):
            """
            for y in range(POPSIZE/(NUM_BEST_PARENTS)):
                nextGen.append(pop[x].mutate())
            """   
            #pick two parents
            p1 = random.choice(pop)
            p2 = random.choice(pop)
        
            #create two children from crossover
            c1 = p1.crossover(p2)
            c2 = p2.crossover(p1)
        
            #mutate the children
            c1 = c1.mutate()
            c2 = c2.mutate()

            nextGen.append(c1)
            nextGen.append(c2)
        
        
    #for x in range (POPSIZE/2):
    #    nextGen.append(network())
    
    #set the parentlist to the next generation list
    pop = nextGen

    if(best != None):
        print gen, best.fitness, best.toString()
    else:
        print gen, "reset"


bestArr.sort(key = lambda x: x.fitness, reverse = True)

for x in range(len(bestArr)):
    bestArr[x].play(True)
    print gen, bestArr[x].fitness, bestArr[x].toString()

for x in range(len(bestArr)):
    print gen, bestArr[x].toString()

best.play(True)

