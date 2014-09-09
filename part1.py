class neuron:
    
    def __init__(self, weights, inputs):
        self.weights=[]
        self.inputs=[1]
        for x in weights:
            self.weights.append(x)
        for x in inputs:
            self.inputs.append(x)
    
    def activationFunc(self):
        activation = 0
        for x in range(0, min( len(self.weights), len(self.inputs) ) ):
            activation += self.weights[x] * self.inputs[x]
        return activation

    def getOutput(self):
        return self.activationFunc() >= 1
            
    
def createInputs(inputs):
    nBools=[]
    for x in inputs:
        if x==False:
            nBools.append(0)
        elif x==True:
            nBools.append(1)
    return nBools

def test():
    
    inputList = [createInputs([False, False, False]),
        createInputs([False, False, True]),
        createInputs([False, True, True]),
        createInputs([True, True, True]),
        createInputs([False, True, False]),
        createInputs([True, False, False]),
        createInputs([True, True, False]),
        createInputs([True, False, True])]

    expListNAND = [True,
           True,
           True,
           False,
           True,
           True,
           True,
           True]
    
    expListOR = [False,
           True,
           True,
           True,
           True,
           True,
           True,
           True]

    

    print 'testing OR'
    bias = 0
    weightList=[bias]
    print 'enter 3 weights to test OR: '
    for x in range(3):
        weightList.append(int(raw_input()))
            
    for x in range (len(inputList)):
        neuronOr = neuron(weightList, inputList[x])
        if(neuronOr.getOutput() == expListOR[x]):
            print 'These weights work when the input is ' + str(inputList[x])
        else:
            print 'These weights ***FAIL*** when the input is ' + str(inputList[x])


    print 'testing NAND'
    
    bias = 3.5
    weightList=[bias]
    print 'enter 3 weights to test NAND: '
    for x in range(3):
        weightList.append(int(raw_input()))
            
    for x in range (len(inputList)):
        neuronNand = neuron(weightList, inputList[x])
        if(neuronNand.getOutput() == expListNAND[x]):
            print 'These weights work when the input is ' + str(inputList[x])
        else:
            print 'These weights ***FAIL*** when the input is ' + str(inputList[x])


