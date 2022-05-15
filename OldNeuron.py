from math import *

class Neuron:
    
    def __init__(self, weights):
        self.weights=[]
        #inputs are values between 1 and -1
        for x in weights:
            self.weights.append(x)

    def giveInput(self, inputs):
        self.inputs=[1]
        for x in inputs:
            self.inputs.append(x)
    
    def activation(self):
        self.activ = 0
        for x in range(0, min( len(self.weights), len(self.inputs) ) ):
            self.activ += self.weights[x] * self.inputs[x]
        return self.activ

    def getOutput(self):
        self.output = ( 1/( 1 + pow( e,( -1 * self.activation() ) ) ) ) -0.5
        return self.output

