
from __future__ import print_function
import sys
from heapq import *
import time
import random
import sys

numberOfSamples = 500


class Query:
    
    def __init__(self):
        

        self.queryVariables = []
        

        self.evidenceVariables = []
        

        self.DictAssign = dict()
        

        self.method = ''
        

        self.probability = -1
         


    def getProbability(self, resultDict):
      
        Variables = 'variables'
        cpts = "CPTs"
        rejectionSampling = "rs"
        variableElimination = "ve"
        prob = self.probability
        method = self.method
        assDict = self.DictAssign
        qVar = self.queryVariables
        eVar = self.evidenceVariables
        
        if prob != -1:
            return

        if method == variableElimination:
            
            print('Implemented in C++')
            
        elif method == rejectionSampling:

            queryRandomVariableDict = dict()            
            
            for randomVariable in qVar:
                assVar = assDict[randomVariable]
                queryRandomVariableDict[randomVariable] = assVar

            startingRandomVariableList = []
            
            rDict = resultDict[Variables]
            for randomVariable in rDict:
                length = len(randomVariable.parentList)
                
                if length == 0:
                    startingRandomVariableList.append(randomVariable)
            
            leng = len(startingRandomVariableList)

            if leng < 1:
                print("RS is not possible!")

            randomVariablesDict = dict()

            rDict = resultDict[Variables]
            
            for randomVariable in rDict:
                rdict = resultDict[cpts]
                
                for cpt in rdict:
                    length = len(cpt.queryVariableList)
                    
                    
                    if length != 1:
                        print("RS not possible!")

                    if cpt.queryVariableList[0] == randomVariable:
                        randomVariablesDict[randomVariable] = cpt

            evidenceVariableDict = dict()
            
            assDict = self.DictAssign
            
            for randomVariable in eVar:
                evidenceVariableDict[randomVariable] = assDict[randomVariable]

            self.probability = rejectionSample(queryRandomVariableDict, startingRandomVariableList,
                                                       randomVariablesDict, evidenceVariableDict, numberOfSamples)

        

        return
        
class cpTable:
    

    def __init__(self):
        
        self.queryVariableList = []        

        self.eVarList = []        

        self.table = []
        

    def getEntry(self, DictAssign):
        table = self.table
        assKeys = set(DictAssign.keys())
        eVariable = set(self.eVarList)
        
        
        inVar = assKeys.intersection(eVariable)
        intVar = inVar
        newAssg = dict()      

        retList = []
        
        for var in inVar:
            newAssg[var] = DictAssign[var]
        
        for entry in table:

            cptEntryNewDict = dict()
            assKey = set(entry.DictAssign.keys())
            inVars = assKey.intersection(intVar)
            cptEntryIntersectionVars = inVars

            for var in cptEntryIntersectionVars:
                cptEntryNewDict[var] = entry.DictAssign[var]

            Dict = dict(entry.DictAssign)
            Prob = entry.probability
            flag = False
            
            if cptEntryNewDict!= newAssg:
                flag = False
            else:
                flag = True
                newtableClass = tableClass()
                newtableClass.DictAssign = Dict
                newtableClass.probability = Prob
                
            if flag:
                retList.append(entry)
                
            

        return retList
  
class tableClass:
    
    def __init__(self):
        
        self.DictAssign = dict()       

        self.probability = 0




class RandomVariable:
    
    def addParent(self, parent):
        Listt = self.parentList
        parentSet = set(Listt)
        tempSet = parentSet
        tempSet.add(parent)
        List = list(tempSet)
        self.parentList = List
        parent.addChild(self)
        
    def __init__(self, name):
        
        self.childrenList = []    


        self.name = name
        

        self.parentList = []      


    def addChild(self, child):
        Listt = self.childrenList
        childSet = set(Listt)
        tempSet = childSet
        tempSet.add(child)
        List = list(tempSet)
        self.childrenList = List


class inputClass:
    

    def __init__(self, input, output):
        
        self.outFileName = output
        self.inFileNames = input
         
        

    def inputFile(self):
        
        
        Variables = 'variables'
        Cpt = 'CPTs'
        Queries = 'queries'
        
        returnDict = {}
        filename = self.inFileNames[0]
        Open = open(filename)
        with Open as inFile:

            lines = inFile.readlines()
            linesRead = 0
            noOfVariables = -1

            noOfVariables = int(lines[linesRead])
            linesRead += 1

            

            randomVariableList = []
            cptList = []
            
            size = None
            size = range(noOfVariables)
            
            for i in size:
                object = RandomVariable(i+1)
                randomVariableList.append(object)

            for i in size:
                variables = lines[linesRead].split()
                linesRead += 1

                thisVarName = -1

                
                thisVarName = int(variables[0])
                index = thisVarName-1
                
                

                thisVar = randomVariableList[index]

                thisCPT = cpTable()
                thisCPT.queryVariableList.append(thisVar)
                
                var = variables[1:]
                for varName in var:

                    
                    varName = int(varName)
                    index = varName-1
                    
                    parent = randomVariableList[index]
                    thisVar.addParent(parent)
                    
                pList = thisVar.parentList
                thisCPT.eVarList = pList

                linesRead = self.getCpt(thisCPT, lines, linesRead)

                cptList.append(thisCPT)
        
        fileName = self.inFileNames[1]
        Open = open(fileName)
        
        with Open as FILE:


            queryList = []

            for line in FILE.readlines():

                words = line.split()
                
                words_2 = words[2]
                words_0 = words[0]
                words_1 = words[1]
                
                newQuery = Query()
                newQuery.method = words_0
                if words_1 != 'q':
                    continue

                word = words_2
                index = 2

                while word != 'e':

                    isTrue = True
                    newWord = word[1:]
                    if word[0] != '~':
                        isTrue = True
                    else:
                        isTrue = False
                        word = newWord
                        
                    

                    try:
                        qVar = newQuery.queryVariables
                        Assgn = newQuery.DictAssign
                        sWord = int(word)-1
                        Obj = randomVariableList[sWord]
                        aObj = randomVariableList[sWord]
                        
                        qVar.append(Obj)
                        Assgn[aObj] = isTrue
                       
                    except ValueError:
                        print("format wrong!")

                    index += 1

                    try:
                        word = words[index]

                    except IndexError:
                        print("format wrong!")
                
                tWords = words[index + 1:]
                
                for word in tWords:

                    isTrue = True
                    newWord = word[1:]
                    if word[0]!= '~':
                        isTrue = True
                    else:
                        isTrue = False
                        word = newWord                        
                    

                    try:
                        eVar = newQuery.evidenceVariables
                        Assg = newQuery.DictAssign
                        
                        size = int(word)-1
                        obj = randomVariableList[size]
                        
                        eVar.append(obj)
                        Assg[obj] = isTrue
                        
                    except ValueError:
                        print("wrong format!")

                    index += 1

                queryList.append(newQuery)
        returnDict[Cpt] = cptList
              
        returnDict[Queries] = queryList
        
        returnDict[Variables] = randomVariableList
        
        

        return returnDict

    def getCpt(self, thisCPT, lines, linesRead, nextIndex=0, assignedtableClass=tableClass()):
        
        length = len(thisCPT.eVarList)
        if nextIndex < length:
            newtableClass = tableClass()
            dictionary = dict(assignedtableClass.DictAssign)
            
            newtableClass.DictAssign = dictionary
            obj = thisCPT.eVarList[nextIndex]
            newtableClass.DictAssign[obj] = True
            linesRead = self.getCpt(thisCPT, lines, linesRead, nextIndex + 1, newtableClass)

            newtableClass = tableClass()
            dictionary = dict(assignedtableClass.DictAssign)
            
            newtableClass.DictAssign = dictionary 
            obj = thisCPT.eVarList[nextIndex]
            newtableClass.DictAssign[obj] = False
            linesRead = self.getCpt(thisCPT, lines, linesRead, nextIndex + 1, newtableClass)
        else:
            stringValues = lines[linesRead].split()
            linesRead += 1

            newtableClass = tableClass()
            dictionary = dict(assignedtableClass.DictAssign)
            newtableClass.DictAssign = dictionary
            obj = thisCPT.queryVariableList[0]
            newtableClass.DictAssign[obj] = True

            
            newtableClass.probability = float(stringValues[0])
            thisCPT.table.append(newtableClass)

            

            newtableClass = tableClass()
            
            dictionary = dict(assignedtableClass.DictAssign)
            newtableClass.DictAssign = dictionary
            obj = thisCPT.queryVariableList[0]
            newtableClass.DictAssign[obj] = False

            prob = float(stringValues[1])
            newtableClass.probability = prob
            thisCPT.table.append(newtableClass) 
                
                
        
        return linesRead

    def printOutput(self, dictRes):
        

        retString = ""
        f = open(self.outFileName, 'w')
        
        queries = dictRes["queries"]  
        
        for query in queries:
            strng = str(query.probability)+ "\n"            
            retString = retString + strng

        retString.rstrip("\n")
        f.write(retString)
        f.close()


def randomAss(entryList, varSel):
    
    length = len(entryList)
    
    if length != 2:
        return None
    object = entryList[0].DictAssign
    key = object.keys()
    
    if varSel not in key:
        return None

    lesserIndex = 1
    prob_0 = entryList[0].probability
    prob_1 = entryList[1].probability
    
    if prob_1 >= prob_0:
        lesserIndex = 0
        
    
    Rndm = random.random()
    cptEntry = entryList[lesserIndex].probability
    
    retObj = entryList[lesserIndex].DictAssign[varSel]
    
    if Rndm < cptEntry:
        return retObj

    else:
        return not retObj





def rejectionSample(qVarDict, VariableList,VarDict, eVarDict, limit):
    

    size = 0
    Limit = xrange(limit)
    for i in Limit:
        currentAssignments = sampleAssign(VariableList,VarDict, eVarDict)

        flag = True

        for randomVariable in qVarDict:
            qVAR = qVarDict[randomVariable]
            cuAss = currentAssignments[randomVariable]
            
            if qVAR == cuAss:
                flag = True
            else:
                flag = False
                break   
            

        if flag:
            size += 1
    
    
    retValue = size / (limit* 1.0)
    return retValue


def main():
    
    args = len(sys.argv)
    file = "output.txt"
    query = "queries"
    if args != 3:
        print("insufficient input arguments! ")
        
    file_1 = str(sys.argv[1])
    file_2 = str(sys.argv[2])
    outFileName = "output.txt"
    inFiles = [file_1, file_2]
    
    writer = inputClass(inFiles, outFileName)

    Dict = writer.inputFile()
    queries = Dict[query]

    
    for i,obj in enumerate(queries):        
        obj.getProbability(Dict)
        

    writer.printOutput(Dict)

    return
def sampleAssign(VariableList, VariablesDict, evidenceVarDict):
    
    
    currentAssignments = dict()
    queueBfs = list(VariableList)
    length = len(queueBfs)
    while length>0:

        varPop = queueBfs.pop(0)
        currentCPT = VariablesDict[varPop]

        Flag = True
        currentKeys = currentAssignments.keys()

        eVar = currentCPT.eVarList
        
        for randomVariableIter in eVar:
            
            if randomVariableIter in currentKeys:
                Flag = True
            else:
                Flag = False
                break
        

        if not Flag:
            queueBfs.append(varPop)
            continue

        filteredCPT = currentCPT.getEntry(currentAssignments)
        newAssignmentVal = randomAss(filteredCPT, varPop)
        
        evKey = evidenceVarDict.keys()
        flag = True
        

            
        if varPop in evidenceVarDict.keys() \
                and newAssignmentVal != evidenceVarDict[varPop]:
            currentAssignments = dict()
            Llist = list(VariableList)
            queueBfs = Llist
            
            continue

        currentAssignments[varPop] = newAssignmentVal
        child = varPop.childrenList
        queueBfs.extend(child)
        length = len(queueBfs)

    return currentAssignments


main()
