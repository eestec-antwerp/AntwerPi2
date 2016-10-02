
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


TODO configuration etc
   
Ground
======

TODO GCS etc

Development environment
=======================

TODO

SITL
----

TODO
