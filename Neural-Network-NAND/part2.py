import part1, math, random
from part1 import *

#boolean to check whether testing for xor or nand
testXOR = False

#list of possible 3 boolean input sets
inputList = [createInputs([False, False, False]),
                 createInputs([False, False, True]),
                 createInputs([False, True, True]),
                 createInputs([True, True, True]),
                 createInputs([False, True, False]),
                 createInputs([True, False, False]),
                 createInputs([True, True, False]),
                 createInputs([True, False, True])]

#list of the expected outputs
expListNAND = [True,
           True,
           True,
           False,
           True,
           True,
           True,
           True]


#list of expected outputs
expListXOR = [False,
           True,
           False,
           False,
           True,
           True,
           False,
           False]

def getFitness(weights, inputList, expList):

    fitness=0

    for x in range(0, len(inputList)):
        aNeuron = part1.neuron(weights, inputList[x])
        if(aNeuron.getOutput() == expList[x]):
            fitness+=1

    return fitness
    

def fitness(y):
    
    fit = [None]*len(y)
    for x in range(0, len(y)):
        if(testXOR):
            fit[x] = getFitness(y[x],inputList, expListXOR)
        else:
            fit[x] = getFitness(y[x],inputList, expListNAND)

    return fit


def populate(x):
    p = [None]*x
    return p

def main():
    POPSIZE = 100
    NUM_BEST_PARENTS = 5 #mu value
    NUM_GENERATIONS = 100
    if(testXOR):
        expList = expListXOR
    else:
        expList = expListNAND

    #create initial population
    pop = populate(POPSIZE)

    #fill initial population with random values
    for x in range(0, POPSIZE):
        pop[x] = [random.random()*4 - 2, random.random()*4 - 2, random.random()*4 - 2, random.random()*4 - 2]

    best = pop[0]

    for x in range(0, NUM_GENERATIONS):
        print('Generation: '+str(x))
        #gets the fitness of current generation
        fitnessGen = fitness(pop)

        #sets initial best parts to two arbitrary values
        bestParents = []
        for y in range(0, NUM_BEST_PARENTS):
            bestParents.append(pop[y])

        
            
            
            
        for y in range(0, POPSIZE):
            index = 0
            notFinished = True
            while(notFinished and index < NUM_BEST_PARENTS):
                if(fitnessGen[y] > getFitness(bestParents[index], inputList, expList)):
                    bestParents[index] = pop[y]
                    notFinished = False
                index+=1

            """if(fitnessGen[y] > getFitness(THRESHOLD, bestParents[0], inputList, expList)):
                bestParents[0] = pop[y]
            elif(fitnessGen[y] > getFitness(THRESHOLD, bestParents[1], inputList, expList)):
                bestParents[1] = pop[y]"""

            #for better editable code implement a while loop with boolean that checks
            #to see if a value has been set to a best parent
            
            if(getFitness(bestParents[0], inputList, expList) > getFitness(best, inputList, expList)):
                best = bestParents[0]

        pop = populate(POPSIZE)

        index = 0

        for y in range(0, NUM_BEST_PARENTS):
            currentParent = bestParents[y]
            
            #print('Parent Fitness ' +str(y)+ ': '+ str(getFitness(currentParent, inputList, expList) ) +'\n'+ str(currentParent) +'\n')
                  
            for z in range(0, POPSIZE/NUM_BEST_PARENTS):
                currentWeights=[]
                for w in range(0, len(currentParent)):
                        currentWeights.append(currentParent[w] + random.random()*4 - 2)
                    #if (currentParent[w] >= 0):
                        #currentWeights.append(currentParent[w] + random.random()*-2)
                    #elif (currentParent[w] < 0):
                        #currentWeights.append(currentParent[w] + random.random()*2)"""
                   

                pop[index] = currentWeights
                index += 1

    print('Best: '+ str(getFitness(best, inputList, expList) ) +'\n'+ str(best) +'\n')

#call main
print('// NAND test 1')
main()
print('// NAND test 2')
main()
print('// NAND test 3')
main()
    
