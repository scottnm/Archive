from math import *

class Neuron:
    
    def __init__(self, weights):
        self.weights=[]
        for x in weights:
            self.weights.append(x)

    #assign each input to this neuron
    def giveInput(self, inputs):
        #inputs are values between 1 and -1
        self.inputs=[]
        for x in inputs:
            self.inputs.append(x)
    
    #gets the sum of the inputs multiplied by their weights
    def activation(self):
        activ = 0
        for x in range(0, min( len(self.weights), len(self.inputs) ) ):
            activ += self.weights[x] * self.inputs[x]
        return activ

    #use a sigmoidal curve to evaluate the activation
    #returns the result on a sigmoidal curve translated down 0.5
        #all values returned will be between -0.5 and 0.5
    def getOutput(self):
        activ = self.activation()
        self.output = ( 1/( 1 + pow( e,( -1 * activ ) ) ) )
        return self.output - 0.5
