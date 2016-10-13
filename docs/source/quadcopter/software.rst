
.. _software:

==========
 Software
==========

Drone
=====

The drone runs the official image from Emlid, which runs Raspbian. On top of Raspbian, you will usually run one or more of the following software:

   - **ArduPilot**: Absolutely necessary, as it provides all to logic for flying. Sometimes referred to as APM, but that is just a popular flight controller running ArduPilot. The software project ArduPilot is split in some parts, of which we only use *Copter* (sometimes referred to as ArduCopter, or APM:Copter). Backed by DroneCode.
   - **MAVProxy**: A popular ground station, but in a drone it will be used primarily for proxying.
   - **DroneControl**: A small web application developed by Evert, so you can more easily start/stop ArduPilot and shutdown the Raspberry Pi. Source here: https://github.com/evertheylen/DroneControl

Ground
======

On the ground you may run the following software:

  - **MAVProxy**: Both as a proxy, or a GCS. It looks promising as a GCS but has a steeper learning curve.
  - **QGroundControl**: A decent GCS, works in all major OS's (including Android, but not recommended). More info in `their site<http://qgroundcontrol.com/>`_.
  - **Tower**: A very stable GCS, and the de facto standard for Android. Get it `here <https://play.google.com/store/apps/details?id=org.droidplanner.android>`_ (you'll also need `3DR Services <https://play.google.com/store/apps/details?id=org.droidplanner.services.android>`_)

There are a lot of other GCS's, but these are tested with our drone (MAVProxy not so much, but their proxy functionality is solid). More GCS's can be found `here <http://ardupilot.org/copter/docs/common-choosing-a-ground-station.html>`_.

Communication
=============

The holy grail of open-source drone software is the MAVLink protocol. However, some flight stacks (like PX4) speak a different dialect, so watch out for incompatibilities with MAVLink-based drones. MAVLink is binary and can be sent over special telemetry modules, but we just use WiFi, usually encapsulated in UDP.

Proxying
--------

As suggested above, the king of proxying in the drone world is MAVProxy. The proxy is bidirectional. I won't provide instructions for using it as a full GCS, only for using it as a proxy. Let's say Arducopter is running (locally) at port 14550 (as is usual when you want to proxy), and you would like to use **two** Ground Control Stations, one at 192.168.1.2:14550, and another at 192.168.1.3:14550. The corresponding MAVProxy command would be::

	mavproxy.py --master localhost:14550 --out 192.168.1.2:14550 --out 192.168.1.3:14550

Keep this process running or all GCS will lose control of the drone!

Alternatively, you can use the ``output`` command in the MAVProxy shell to add or remove outputs::

	MAV> output list
	0 outputs
	
	MAV> output add 192.168.1.2:14550
	Adding output 192.168.1.2:14550
	
	MAV> output add 192.168.1.3:14550
	Adding output 192.168.1.3:14550
	
	MAV> output list
	2 outputs
	0: 192.168.1.2:14550
	1: 192.168.1.3:14550
	
	MAV> output remove 0
	Removing output 192.168.1.2:14550
	
	MAV> output list
	1 outputs
	0: 192.168.1.3:14550
	
	MAV> 

Of course, you can also do the proxying on your own laptop, rather then on the Raspberry Pi itself. In that case the ``master`` may be an external IP address, but the ``outputs`` may be local.

**NOTE**: The DroneControl program does not use MAVProxy.

Programming
===========

The usual way of programming a drone would be to write a program that sends MAVLink commands to the drone, while the code runs on another PC. However, with the Raspberry Pi as our main flight controller, we can run that program on the drone itself, and theoretically achieve a fully autonomous drone (of course, not recommended for safety).

Manually crafting and sending MAVLink packets is rather tedious. If you're programming in Python, you're in luck: 3DR created `DroneKit <https://github.com/dronekit/dronekit-python>`_. Sadly, it's Python 2 only. (Originally Dronekit spanned much more than the Python component but the other parts (like the cloud part) never gained much traction).

It is important to note that a program written with dronekit acts as a full-blown MAVLink receiver/transmitter. To run your program while also being able to use a GCS (*highly* recommended), using MAVProxy is essential.


SITL
----

TODO

http://python.dronekit.io/develop/sitl_setup.html

.. _maintaining_software:

Maintaining
===========

While I am a fan of bleeding-edge software, it may mean your drone falls out of the sky. Therefore, only install updates to flight-critical software if it's encouraged by Emlid. After major updates, you should recalibrate the drone.
