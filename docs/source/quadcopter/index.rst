
=================
 Getting Started
=================

About
=====

This quadcopter was originally made for EESTEC Antwerp's workshop "AntwerPi 2.0" in end of June 2016. Hopefully it can find more usecases through this documentation.

The quad was built from scratch, and used rather common (and trustworthy) parts, with the exception of the Navio2 which is rather new in the world of DIY avionics. The combination of a Navio2 and a Raspberry Pi allows for a much greater flexibility than either a traditional DIY drone which lacks a proper way of programming it, or a commercial drone, which lacks an open API (like DJI's drones), or extensibility for sensors or networks (the RPi and Navio2 provide analog and digital (USB, UART, Ethernet) input).

Guide
=====

You may have a great application for drones, but before you can get your hands dirty with experiments or the implementation, you need to know something about drones. Roughly, 4 things need to be done:

  1. Learn a bit about drones in general, and our drone in particular (mainly hardware).
  2. Learn about the different software components that drive the drone.
  3. Learn how to maintain and set up and your drone, either for manual or programmatic flight, and take care of safety.
  4. Learn to fly (in case you need to take over)

The focus of this documentation is on 2 and 3. Apart from this, depending on what you want to do with the drone you may need more knowledge. Examples:

  - Adding a sensor: is it through UART? Analog? How to connect it and read values?
  - Replacing (broken) components: how to replace? What to buy? What to replace?
  - Unexpected behaviour: shaking? unstable? GPS drifts? How to fix?
  - Software testing: how to set up SITL (Software-in-the-loop)?

In this documentation, we will try to get you in the air as soon as possible, while providing the tools to solve any issues that might arise.

About Drones
------------

I will not go into too much detail here, as I already make a presentation about it. You can find it on :ref:`links`.

More info on this drone can also be found in the presentation. However, there's also a big schematic that should answer a lot of questions: :ref:`schematic`.

Software
--------

If you ever want to program your drone, you will need a good understanding of the different components that drive a drone. This is explained in :ref:`software`.

Setup and :ref:`maintenance`
----------------------------

Roughly speaking, there are 3 ways to fly:

  - Manual: The drone reacts to the transmitter and possible controls on a laptop or smartphone.
  - Guided: You plan a flight in advance, and the drone executes it. You just press play.
  - Programmatic: The drone may react to it's environment while flying. You can't predict where it will go.

The first two require virtually the same setup, this is discussed in :ref:`manual-setup`. The third one is different, this is discussed in :ref:`programming-setup`, along with ways to easily test your software and safety guidelines.

Flying
------

Learning to fly is essential for maintaining safety, since you must be able to take over at all times. Learning how to fly is out of the scope of this documentation, but you can get the basics using `this video <https://www.youtube.com/watch?v=6btEFJJD4_o>`_ and a cheap commercial quad (that can take a beating) to train with.

Adding/replacing parts
----------------------

If you want to add something that requires some form of communication, a pinout is useful. Navio's official `pinout <https://www.youtube.com/watch?v=6btEFJJD4_o>`_ is rather puzzling, but luckily all the pins are labeled under the board. All connectors on the side of the Navio2 are DF-13 sockets with either 6 or 4 pins. Covering all possible ways of extending the drone is outside the scope of this documentation.

Replacing is rather tricky without good knowledge of how the drone works. Some parts are soldered on, some parts require a specific voltage, etc. Just one tip: whenever you replace or add something, make sure you calibrate the drone (see :ref:`calibration`).

When searching the internet for solutions or parts; the :ref:`glossary` may be useful.
