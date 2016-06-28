
===========
 Waypoints
===========

We will be exporting files in the QGC format::

    QGC WPL <VERSION>
    <INDEX> <CURRENT WP> <COORD FRAME> <COMMAND> <PARAM1> <PARAM2> <PARAM3> <PARAM4> <PARAM5/X/LONGITUDE> <PARAM6/Y/LATITUDE> <PARAM7/Z/ALTITUDE> <AUTOCONTINUE>

Where:

  - ``<VERSION>``: just use 110
  - ``<INDEX>``: starts at 1, count up
  - ``<CURRENT WP>``: leave this at 0
  - ``<COORD FRAME>``: leave this at 3
  - ``<COMMAND>``:
    - 16: waypoint (also home?)
    - 82: spline waypoint
    - 20: Return To Launch (location doesn't matter)
    - 21: Land
    - 22: Takeoff
  - ``<PARAM1>``: ??????????????
  - ``<PARAM2>``: acceptance radius
  - ``<PARAM3>``: leave it at 0
  - ``<PARAM4>``: yaw at waypoint
  - ``<PARAM5/X/LONGITUDE>``: longitude, for example 51.1836405291789518
  - ``<PARAM6/Y/LATITUDE>``: latitude, for example 4.41937923431396484
  - ``<PARAM7/Z/ALTITUDE>``: height < 10m !
  - ``<AUTOCONTINUE>``: leave it at 1

