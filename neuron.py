import random, math, GameEnvironment, npc



class neuron:
    
    #num_inputs = total number of inputs + 1 for the bias input
    def __init__(self, num_inputs):
        #generating an initially random set of weights
        self.weights = []
        for x in range(num_inputs):
            self.weights.append(random.uniform(-5, 5)) #-1, 1
                           
    def process(self, inputs):
        iSum = 0
        #inputs.append(1)
        for x in range(len(inputs)):
            iSum += self.weights[x] * inputs[x]

        return 1 / (1 + math.e ** (-1*iSum))

    #mutation with more variance
    def mutate1(self):
        newNeuron = self.clone()
        for x in newNeuron.weights:
            if random.random() > 0.5:
                x+=random.uniform(-5, 5) #-.3, .3
        return newNeuron

    #mutation with less variance
    def mutate2(self):
        newNeuron = self.clone()
        index = random.randint(0, len(newNeuron.weights)-1)
        newNeuron.weights[index]+=random.uniform(-5, 5)
        return newNeuron

    def crossover1(self, otherNeuron):
        newNeuron = self.clone()
        for x in range( len(newNeuron.weights) ):
            if random.random() > 0.5:
                newNeuron.weights[x] = otherNeuron.weights[x]
        return newNeuron

    def crossover2(self, otherNeuron):
        newNeuron = self.clone()
        index = random.randint(0, len(newNeuron.weights)-1)
        newNeuron.weights[index] = otherNeuron.weights[index]
        return newNeuron

    def clone(self):
        newNeuron = neuron(len(self.weights))
        newNeuron.weights = self.weights[:]
        return newNeuron

class network:

    def __init__(self):
        self.moveNeuron = neuron(5)
        self.shootNeuron = neuron(5)
        self.neuronList = [self.moveNeuron, self.shootNeuron]
        self.fitness = -1000

    def processNetwork(self, inputs):
        moveOutput = self.moveNeuron.process(inputs)
        shootOutput = self.shootNeuron.process(inputs)
        return [moveOutput, shootOutput]

    def mutate(self):
        newNetwork = self.clone()
        for neuron in newNetwork.neuronList:
            if random.random() > 0.5:
                neuron = neuron.mutate2()
        return newNetwork

    def crossover(self, otherNetwork):
        newNetwork = self.clone()
        for x in range( len(newNetwork.neuronList) ):
            if random.random() > 0.5:
                newNetwork.neuronList[x] = newNetwork.neuronList[x].crossover2(otherNetwork.neuronList[x])

        return newNetwork

    def play(self, display):
        
        #GameEnvironment.game(display, npc, self)
        #print '1'

        GameEnvironment.game(display, npc.npc1(20,20), self)
        """
        #print '2'
        GameEnvironment.game(display, npc.npc2(20,20), self)
        #print '3'
        GameEnvironment.game(display, npc.npc3(20,20), self)
        #print '4'
        GameEnvironment.game(display, npc.npc4(20,20), self)
        #print '5'
        GameEnvironment.game(display, npc.npc5(20,20), self)
        #print '6'
        GameEnvironment.game(display, npc.npc6(20,20), self)
        """

    def getFitness(self, display):
        
        self.fitness = 0
        #print '1'
        self.fitness += GameEnvironment.game(display, npc.npc1(20,20), self)
        
        #print '2'
        self.fitness += GameEnvironment.game(display, npc.npc2(20,20), self)
        #print '3'
        self.fitness += GameEnvironment.game(display, npc.npc3(20,20), self)
        #print '4'
        self.fitness += GameEnvironment.game(display, npc.npc4(20,20), self)
        #print '5'
        self.fitness += GameEnvironment.game(display, npc.npc5(20,20), self)
        #print '6'
        self.fitness += GameEnvironment.game(display, npc.npc6(20,20), self)
        self.fitness = self.fitness/6
        #print "fit: ",self.fitness
        
        #self.fitness = GameEnvironment.game(display, npc, self)
        

    def clone(self):
        newNetwork = network()
        newNetwork.moveNeuron = self.moveNeuron.clone()
        newNetwork.shootNeuron = self.shootNeuron.clone()
        newNetwork.neuronList = [newNetwork.moveNeuron, newNetwork.shootNeuron]
        return newNetwork

    def toString(self):
        return "move: "+str(self.moveNeuron.weights)+"\tshoot: "+str(self.shootNeuron.weights)
        
        
        

    
        
