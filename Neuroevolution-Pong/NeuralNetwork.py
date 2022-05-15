from Neuron import *

class NeuralNetwork:

    #assign the input neurons and output neuron to this network
    def __init__(self, iNeuronsWeights, oNeuronsWeights):
        self.iNeuronList = []
        for weight in iNeuronsWeights:
            self.iNeuronList.append(Neuron(weight))

        self.oNeuron = Neuron(oNeuronsWeights[0])

    #calculate the result of the network
    def feedIn(self, inputs):
        self.output = []
        for x in range (0, len(self.iNeuronList)):
            self.iNeuronList[x].giveInput(inputs[x])
            self.output.append(self.iNeuronList[x].getOutput())
            
        self.oNeuron.giveInput(self.output)

    #return the result of the network
    def getOut(self):
        return self.oNeuron.getOutput()
                        
            
        
    
