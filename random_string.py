import random

if __name__ == "__main__":

    string = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
    print(len(string))
    finalString = ''

    for j in range(26):
        tempString = ''
        thisString = (string + '.')[:-1]
        for i in range(26):
            pick = random.randint(0, len(thisString) - 1)
            tempString += thisString[pick]
            thisString = thisString[:pick] + thisString[pick + 1:]
        finalString += tempString + '\n'

    print(finalString)
