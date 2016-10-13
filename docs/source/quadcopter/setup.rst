.. _setup:

=======================
 Setup and maintenance
=======================

Preparation
===========

Charging LiPo's
---------------

The IMAC charger we use is in fact a so-called 4-button charger with a built-in PSU. If used with the standard batteries, it should already be set to the right settings (4S LiPo). There are 3 modes that might be of interest:

  - Balanced Charge: The charger will fully charge your battery, and balance the cells. This is the most used.
  - Fast Charge: The charger will almost fully charge your battery, but will **not** balance the cells. This will save time, but is bad if done too often. Therefore, only do one subsequent fast charge, after which you should use a mode *with* balancing.
  - Storage: The charger will empty or fill the battery until about 40%, and balance the cell. As the name implies, this is best for storage. A fully charged battery may damage itself over time.

**ALWAYS** first plug in the two banana plugs, this will prevent the two plugs from accidentally hitting each other and short circuiting and exploding etc. In fact, it's best if you strictly follow this order:

  1. connect charger to AC
  2. connect cable (JUST THE CABLE banana plugs -> XT60) to charger
  3. connect balancing plugs to charger
  4. connect battery to cable's XT60 end

LiPo's can explode or catch fire. Therefore, whenever you can, put them in the safety bag. Never store an unbalanced battery for too long!

More info about 4-button chargers can be found here: https://www.youtube.com/watch?v=kvr-25yGeVk. More info about LiPo's can found on wikipedia or so.

Charging receiver
-----------------

The RC transmitter has it's own Li-ion battery, which is charged through the transmitter itself using a Micro-USB connection (a standard smartphone charger works). While I never had an empty battery, it's not possible to check the level. Therefore, make sure you charge it before every day of flying, since the RC connection is your last controllable fallback.

Flight
======

Before
------

  1. Place drone on a flat surface. The red arms should point AWAY from you.
  2. Install propellors (a CW motor should have a CW prop, same for CCW). The props are self-tightening, this means you have to screw them on in the opposite direction of how they'll spin in flight.
  3. Connect battery (the ESC's will start beeping because they don't get a valid signal)
  4. Connect smartphone or laptop to Drone's WiFi network
  5. Start Arducopter (the ESC's will -- thankfully -- stop beeping)
  6. Check for any anomalies in your GCS

Takeoff
-------

There are `a lot of different modes <http://ardupilot.org/copter/docs/flight-modes.html#overview>`_ of flying. I will explain the steps assuming you use *Stabilize*, which is the most difficult of them.

  1. Arm the drone either through your GCS or by holding both sticks in the bottom right corner for 3 seconds.
  2. (not possible in Alt Hold or similar) Give a little bit of throttle, check again for any anomalies (visually).
  3. Give it enough throttle to take off (50%). Don't do it too slow, the drone may tilt. Afterwards, the drone will hover at about 40%.

Tower or other GCS may provide you with a button to take off.
  
During flight
-------------

The upper left switch can be used for BREAK mode (fully upwards). The drone will stay where it is, and not respond to any input, unless you switch modes or pull the switch down. Turning the knob on the right WILL make you switch modes! The BREAK mode is GPS-dependent, so if GPS fails this is not a safe fallback. Flying in STABILIZE is a better option then, provided you are a good pilot.

Switching modes can in theory be done through the knob, but this is a bad idea. Some of them might be modes with altitude hold, in which case the throttle should be at 50% to hover, and others may be without altitude hold, in which case the drone will go in a reasonably fast climb at 50% throttle. To prevent "cycling" through the modes, use your GCS to switch modes.

WiFi is not particularly strong. The drone uses the built-in WiFi of the Raspberry Pi, and the GCS may be a smartphone. Do **not** go too far with the drone, and **definitely** never out of LOS.

Landing
-------

Slowly. Tower or other GCS may again provide a button for this.

After
-----

In DroneControl, first stop Arducopter (the beeping will start again), and then shut down the Raspberry Pi. Then, you can disconnect the battery.

Safety
======

Controlling
-----------

The person holding the RC transmitter must be kept in sync with possible warnings the GCS is giving. Either he has a smartphone mounted on top of the transmitter, or someone else must hold the smartphone or laptop. That other person should not be distracted by the drone itself, instead the focus must lie on the GCS.

If someone else is responsible for the GCS, mode switches will also be much easier.

Fallbacks
---------

It is wise to provide as many fallbacks as possible. These may include loss of signal (both Wifi and RC), geofences, and more. If your drone is totally uncontrollable, and the GPS is failing too, consider using the kill switch. It'll crash the drone, but hopefully it won't crash into a window ;) .

More information about this can be found on the Arducopter wiki.

Running a program
-----------------

When running code on your drone, guaranteeing safety is more complex. 

TODO: A way to remove the program from the output list of MAVProxy at the push of a button...


Maintenance
===========

Calibration
-----------

Whenever something physically is added, removed or moved from the drone, or the drone is behaving weird, you should consider calibrating the sensors. More information here: http://ardupilot.org/copter/docs/configuring-hardware.html.

Calibrating the ESC's is not needed for our drone.

**Maintaining software** --> see :ref:`maintaining_software`.
