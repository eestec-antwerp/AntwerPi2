import random
import sys

nr_of_cargo = 10
lvl = 1
filename = "my_level.txt"


def generateWeight():
    if lvl < 3:
        return 0
    return random.uniform(0, 1)


def generateLocation():
    return (random.randint(0, 100), random.randint(0, 100))


def generateDestination():
    if lvl < 2:
        return (0, 0)
    return (random.randint(0, 100), random.randint(0, 100))

stringy = ""
for i in range(nr_of_cargo):
    stringy += str(generateLocation()) + "," + \
        str(generateDestination()) + "," + str(generateWeight()) + "\n"
    #print( generateLocation(), ",", generateDestination(),",", generateWeight() , file=open(filename))
with open(filename, "w") as f:
    f.write(stringy)
