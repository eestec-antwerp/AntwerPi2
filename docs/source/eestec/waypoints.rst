
===========
 Waypoints
===========

We can transform a route you programmed into an actual route for the drone, using the file `wayppoints.py <https://raw.githubusercontent.com/eestec-antwerp/AntwerPi2/master/waypoints/waypoints.py>`_. Download it and put it next to your ``main.py``.

To use it, you need to import it first, add the following line at the **top** of your file::
    
    from waypoints import WaypointFile
    
So now it should look like this::

    from waypoints import WaypointFile

    import turtle
    import random
    import math
        
And then you can generate a waypoints file with these lines (somewhere at the bottom of your file)::

    f = WaypointFile.import_route(droney.route, "waypoints.txt")
    f.write()

So it should look like this::

    #######################
    # SCORE
    #######################
    
    print("\n=============================\nSCORE (lower is better)")
    print(droney.score())
    
    f = WaypointFile.import_route(droney.route, "waypoints.txt")
    f.write()

As you can see, the only important method is ``import_route(route, filename)``.
    
Each time you execute it, you will get also generate a ``waypoints.txt`` file. You can use it with `APM Planner <http://planner.ardupilot.com/planner2/docs/installing-apm-planner-2.html>`_.


QGC format
==========

**(not important, skip this unless you're really bored and want to extend the functionality of ``waypoints.py``)**


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

