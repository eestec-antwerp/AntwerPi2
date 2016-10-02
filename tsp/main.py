import turtle
import random
import math


"""
exercises:
    1) pick up all cargo with one drone
        - endcondition = all cargo picked up
        - score is distance (smallest score wins)
    2) each picked up cargo has a destination
        - endcondition is all cargo at it's destination
        - score is distance
    3) each cargo has a weight and a drone has a maximum carrying weight
        - endcondition is all cargo at its destination
        - score is total distance
"""


class Field:
    scaling_factor = 5
    alex = turtle.Turtle()

    def __init__(self, _input="1_10.txt"):
        print(_input)
        # initialize
        maxX, maxY = 100, 100
        turtle.setup(50 + Field.scaling_factor * maxX,
                     50 + Field.scaling_factor * maxY)
        self.screen = turtle.Screen()
        self.fieldmatrix = []
        self.height, self.width = maxX, maxY
        self.counter = 0
        # read cargo from input
        self.cargos = {}
        with open(_input) as f:
            lines = f.readlines()
            for i in lines:
                j = eval(i)
                self.cargos[j[0]] = Cargo(j, self)
        # draw cargo
        Field.alex = turtle.Turtle()
        Field.alex.pu()
        Field.alex.speed(0)
        self.screen.register_shape("box.gif")
        self.screen.register_shape("boxpu.gif")
        self.screen.register_shape("boxdo.gif")
        self.screen.register_shape("drone.gif")
        Field.alex.shape("box.gif")
        for c in self.getCargoList():
            x, y = c.getLocation()
            _x, _y = Field.scaling_factor * x - 5 * \
                (self.width / 2), Field.scaling_factor * y - 5 * (self.height / 2)
            Field.alex.goto(_x, _y)
            Field.alex.stamp()

    def endCondition(self, scenario=0):
        self.counter += 1
        if scenario == 1:
            for cargo in self.getCargoList():
                if not cargo.pickedUp:
                    return False
            return True
        elif scenario == 2:
            for cargo in self.getCargoList():
                if not cargo.droppedOf:
                    return False
            return True
        elif scenario == 3:
            pass
        else:
            return self.counter > 5

    def getCargoList(self):
        return self.cargos.values()

    def getCargo(self, x, y=None):
        if not y:
            x, y = x
        if (x, y) in self.cargos:
            if not self.cargos[(x, y)].pickedUp:
                return self.cargos[(x, y)]
            else:
                print("Drone already picked up")
                return None
        else:
            return None


class Cargo:

    def __init__(self, creation, field):
        self.w = creation[2]
        self.destination = creation[1]
        self.location = creation[0]
        self.pickedUp = False
        self.droppedOf = False
        self.field = field

    def getWeight(self):
        return self.w

    def getLocation(self):
        return self.location

    def getDestination(self):
        return self.destination

    def __str__(self):
        to_return = "--------------\n"
        to_return += "weight = " + str(self.w) + "\n"
        to_return += "location = " + str(self.location) + "\n"
        to_return += "destination = " + str(self.destination) + "\n"
        return to_return

    def pickup(self):
        print("picked up cargo at " + str(self.location))
        self.pickedUp = True

        # render
        Field.alex.shape("boxpu.gif")
        Field.alex.pu()
        Field.alex.speed(0)
        x, y = self.location
        _x, _y = Field.scaling_factor * x - 5 * \
            (self.field.width / 2), Field.scaling_factor * \
            y - 5 * (self.field.height / 2)
        Field.alex.goto(_x, _y)
        Field.alex.stamp()

    def dropOff(self):
        print("dropped of cargo at" + str(self.destination))
        self.droppedOf = True

        # render
        Field.alex.shape("boxdo.gif")
        Field.alex.pu()
        Field.alex.speed(0)
        x, y = self.destination
        _x, _y = Field.scaling_factor * x - 5 * \
            (self.field.width / 2), Field.scaling_factor * \
            y - 5 * (self.field.height / 2)
        Field.alex.goto(_x, _y)
        Field.alex.stamp()


class Route:

    def __init__(self):
        self.points = []

    def addWayPoint(self, x=0, y=1):
        _x, _y = x, y
        self.points.append((x, y))

    def calculateDistance(self):
        total_distance = 0
        prev_x = 50
        prev_y = 50
        for x, y in self.points:
            a = ((prev_x - x)**2)
            b = ((prev_y - y)**2)
            total_distance += math.sqrt((a + b))
            prev_x, prev_y = x, y
        return total_distance


class Drone:
    drones = []

    def __init__(self, field, flyspeed):
        Drone.drones.append(self)
        self.route = Route()
        self.field = field
        self.alex = turtle.Turtle()
        self.alex.shape("drone.gif")
        self.alex.speed(flyspeed)
        self.location = 50, 50
        self.capacity = 1
        self.cargo = {}

    def flyTo(self, x, y=None):
        if not y:
            x, y = x
        _x, _y = Field.scaling_factor * x - 5 * \
            (self.field.width / 2), Field.scaling_factor * \
            y - 5 * (self.field.height / 2)

        self.alex.goto(_x, _y)
        self.route.addWayPoint(x, y)
        self.location = x, y

    def score(self):
        return self.route.calculateDistance()

    def pickupCargo(self):
        cargo_at_location = self.field.getCargo(self.location)
        if cargo_at_location:
            if cargo_at_location.getWeight() < self.capacity:
                cargo_at_location.pickup()
                self.capacity -= cargo_at_location.getWeight()
                self.cargo[cargo_at_location.getDestination()
                           ] = cargo_at_location
                return cargo_at_location
            else:
                print(
                    "WARNING: not enough capacity for pickup cargo at location: " + str(self.location))
                return None
        else:
            print("WARNING: no cargo at: " + str(self.location))
            return None

    def dropOffCargo(self):
        if self.location in self.cargo:
            self.cargo[self.location].dropOff()
            self.capacity += self.cargo[self.location].getWeight()
            del self.cargo[self.location]
        else:
            print("NO cargo to dropoff at " + str(self.location))


if __name__ == "__main__":

    #######################
    # INITIALISE
    #######################

    level = 1  # can be 1, 2 or 3
    nr_cargo = 100  # can be 10, 100 or 1000

    field = Field(_input=str(level) + "_" + str(nr_cargo) + ".txt")
    # flyspeed can be "slowest", "slow", "fast", "fastest"
    droney = Drone(field, flyspeed="fast")

    #######################
    # GAMELOOP (with a random approach)
    #######################

    # preparation (hint: it might be smart to calculate some things before you
    # start flying)
    cargos = list(field.getCargoList())
    index = 0

    # argument for endcondition is the scenario: so 1, 2 or 3
    while not field.endCondition(level):

        # drone coordination (this implementation works, but is very bad. Yours
        # should be better :D)
        droney.flyTo(cargos[index].getLocation())
        cargo = droney.pickupCargo()

        if level > 1:  # ignore this if you're still at level 1
            #(this implementation works, but is very bad. Yours should be better :D)
            droney.flyTo(cargo.getDestination())
            droney.dropOffCargo()

        index += 1

    #######################
    # SCORE
    #######################

    print("\n=============================\nSCORE (lower is better)")
    print(droney.score())
