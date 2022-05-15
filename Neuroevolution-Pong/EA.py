
import pong_trainer, random, time

#CONSTANTS
POPSIZE = 10
NUM_GEN = 30
NUM_BESTPARENTS = 2

def emptyPopulate(x):
    emptyPop = [None]*x
    return emptyPop

def main():
    #weights to evolve for the input neurons
    iWeights1 = emptyPopulate(POPSIZE)
    iWeights2 = emptyPopulate(POPSIZE)
    iWeights3 = emptyPopulate(POPSIZE)

    #weights to evolve for the output neuron
    oWeights = emptyPopulate(POPSIZE)

    #fill weight containers with random starting values
    for x in range(POPSIZE):
        iWeights1[x] = [random.uniform(-1.0,1.0), random.uniform(-1.0,1.0)]
        iWeights2[x] = [random.uniform(-1.0,1.0), random.uniform(-1.0,1.0)]
        iWeights3[x] = [random.uniform(-1.0,1.0), random.uniform(-1.0,1.0)]
        oWeights[x] = [random.uniform(-1.0,1.0), random.uniform(-1.0,1.0), random.uniform(-1.0,1.0), random.uniform(-1.0,1.0)]

    #initiate an arbitrary best
    best = [[iWeights1[0], iWeights2[0], iWeights3[0]], [oWeights[0]]]
    bestFitness = pong_trainer.PlayRound(best[0], best[1], False)

    for genNum in range(NUM_GEN):
        #get the fitness of the current generation
        fitness=[]
        for x in range(POPSIZE):
            inputWeights = [iWeights1[x], iWeights2[x], iWeights3[x]]
            outputWeights = [oWeights[x]]
            #get the fitness that corresponds to weights of the neural network
            fitness.append(pong_trainer.PlayRound(inputWeights, outputWeights, False))

        #get the best parents
        bestParents=[]
        bestParentsFitness=[]
        #fill the best parent list with the first NUM_BESTPARENT children
        #so that you can have values to compare to later
        for x in range(NUM_BESTPARENTS):
            inputWeights = [iWeights1[x], iWeights2[x], iWeights3[x]]
            outputWeights = [oWeights[x]]
            bestParents.append([inputWeights, outputWeights])
            bestParentsFitness.append(fitness[x])


        for x in range(POPSIZE):
            index = 0
            Finished = False
            #fills the best parents list with the most fit children in sorted descending order
            while(not Finished and index<NUM_BESTPARENTS):
                if(fitness[x] >= bestParentsFitness[index]):
                    inputWeights = [iWeights1[x], iWeights2[x], iWeights3[x]]
                    outputWeights = [oWeights[x]]
                    bestParents[index] = [inputWeights, outputWeights]
                    bestParentsFitness[index] = fitness[x]
                    Finished = True
                index+=1

            #checks to see if the bestparent is better than the overall best
            if(bestParentsFitness[0] >= bestFitness):
                best = bestParents[0]
                bestFitness = bestParentsFitness[0]

        #after the best children are found report the best child
        #and then demonstrate it in the trainer
        print "Gen: ",genNum,"\tfit: ",bestFitness
        pong_trainer.PlayRound(best[0], best[1], False)

        #weights to evolve for the input neurons
        iWeights1 = emptyPopulate(POPSIZE)
        iWeights2 = emptyPopulate(POPSIZE)
        iWeights3 = emptyPopulate(POPSIZE)
        #weights to evolve for the output neuron
        oWeights = emptyPopulate(POPSIZE)

        index = 0

        for x in range(NUM_BESTPARENTS):
            currentParent = bestParents[x]

            for y in range(POPSIZE/NUM_BESTPARENTS):
                currIWeights1=[]
                currIWeights2=[]
                currIWeights3=[]
                currOWeights=[]
                
                #mutate each of the children
                currIWeights1 = [currentParent[0][0][0] + random.uniform(-1.0, 1.0), currentParent[0][0][1] + random.uniform(-1.0, 1.0)]
                currIWeights2 = [currentParent[0][1][0] + random.uniform(-1.0, 1.0), currentParent[0][1][1] + random.uniform(-1.0, 1.0)]
                currIWeights3 = [currentParent[0][2][0] + random.uniform(-1.0, 1.0), currentParent[0][2][1] + random.uniform(-1.0, 1.0)]
                currOWeights = [currentParent[1][0][0] + random.uniform(-1.0, 1.0), currentParent[1][0][1] + random.uniform(-1.0, 1.0), currentParent[1][0][2] + random.uniform(-1.0, 1.0), currentParent[1][0][3] + random.uniform(-1.0, 1.0)]

                #add mutated child to the new list of children
                iWeights1[index] = currIWeights1
                iWeights2[index] = currIWeights2
                iWeights3[index] = currIWeights3
                oWeights[index] = currOWeights

                index+= 1

        
        #elitism
        #keeps the best child around in every generation      
        iWeights1[0] = best[0][0]
        iWeights2[0] = best[0][1]
        iWeights3[0] = best[0][2]
        oWeights[0] = best[1][0]
        
    return best

        
#finds the best fitness out of all the generations             
best = main()
#print the best child of all the generations
#and then demonstrate it in the trainer
print "input_weights: ",best[0],"\touput_weights: ",best[1]
pong_trainer.PlayRound(best[0], best[1], True)
