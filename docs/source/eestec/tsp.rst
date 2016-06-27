==================
 Drone simulation
==================

In this exercise, we will try to optimize the path of the drone, given a set of positions and other constraints. For this you should download `tsp.zip <tsp.zip>`_ and extract it. The rest of this project will assume that the files are in the current directory.

Efficient paths
===============

Drones fly. Well that?s an obvious statement. But they shouldn?t just fly around randomly, they often has a purpose.

Level 1
=======

Consider as a purpose: ?picking up cargo?. We can assume this cargo is weightless, meaning that a drone can carry infinite amounts of cargo. The only thing the drone is picking up every cargo and doing this as efficient as possible.

In the simulation you?ll get a drone and you?ll be able to coordinate this drone. The purpose of your coordination is to reduce the distance needed to collect all cargo.

In the implementation, we already provided some code that already picks up every cargo, but it isn?t efficient at all. 

When you run main.py, you can see some cargo appearing and then you see a drone flying around and ?picking up cargo?. Picking up is visualised as an upward arrow appearing on the cargo.
In the code fragment you can see there are three main parts of the code (that you need to know of). The initialisation part, where you can select the level and the amount of cargo. I?d recommend to change the amount of cargo to 10 while experimenting, and the speed to slow or even slowest. This way it?s easy to see what happens.

The actual coordination happens in the gameloop part, and you can immediatly see you have some methods at your disposal.

  - ``field.getCargoList()`` returns a list with all cargo to be picked up.
  - ``droney.flyTo(x,y)`` will fly to the x and y coordinate on the screen. (note that both are both are between 0 and 100)
  - ``droney.pickupCargo()`` will pick up cargo at the current location if available.
  - ``field.endCondition(level)`` is a method that for a certain level can help you decide when you?re done.

You see things inside an if, but you can actually ignore the largest part of this code.
Then you have the score part, which will simply print your score when the simulation is over. The lower the score, the better your coordination is!

Level 2
=======

*(only read after completing level 1)*

Level 2 is an extension to level 1. The main difference is that each cargo has a destination. Changing the level to 2 in the initialisation part and you will see ``main.py`` dropping of some cargo to certain places. (symbolised by a cargo with a downwards arrow).

Notice that you should use the ``droney.dropOffCargo()`` method now as seen in the example code.

Level 3
=======

Level 3 is another extension on level 2. The difference now is that cargo isn?t weightless anymore. Every piece of cargo has a weight between 0kg and 1kg, and a drone has a carrying capability of 1kg.
