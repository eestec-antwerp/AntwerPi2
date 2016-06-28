
# TODO 
# WTF at 4th param
# higher resoltion than double for GPS

import functools

class QgcError(Exception, object):
    def __init__(self, msg, line=None):
        self.msg = msg
        self.line = line
    
    def __str__(self):
        return "Something went wrong while handling the QGC format: " + self.msg + "(line: %d)"%self.line if self.line is not None else ""

def _assert(cond, msg="unknown", line=None):
    if not cond:
        raise QgcError(msg)

def bases_overlay(f):
    @functools.wraps(f)
    def method(self, *args, **kwargs):
        bases_dct = {}
        for b in type(self).__class__.__bases__:
            print("TRYING", b)
            try:
                bases_dct.update(getattr(b, f.__name__)(self, *args, **kwargs))
            except Exception as e:
                print(e)
                pass
        own_dct = f(self, *args, **kwargs)
        for k, v in bases_dct.items():
            if k not in own_dct:
                own_dct[k] = v
        return own_dct
    return method

class QgcFile(object):
    version = 110
    
    def __init__(self, name, read=False):
        self.name = name
        if read:
            with open(name, "r") as f:
                print("#"+f.readline()+"#")
                #assert f.readline() == "QGC WPL 110\n"
                self.commands = [QgcCommand.from_line(l.rstrip("\n")) for l in f.readlines() if len(l) > 2]
                print("Loaded", len(self.commands), "commands")
        else:
            self.commands = []
    
    def write(self):
        with open(self.name, "w") as f:
            f.write("QGC WPL 110\n")
            f.write("\n".join([str(c) for c in self.commands]))
            f.write("\n")

class para:
    INDEX = 0
    CURRENT_WP = 1
    COORD_FRAME = 2
    COMMAND = 3
    PARAM1 = 4
    RADIUS = 5
    PARAM3 = 6
    YAW = 7
    LON = 8
    LAT = 9
    ALT = 10
    AUTOCONT = 11

class QgcCommand(object):
    @classmethod
    def from_line(cls, line):
        parts = line.split("\t")
        cmd = int(parts[3])
        return cmd_dct[cmd].from_line(parts)
    
    def params(self):
        print("params called")
        return {
            para.INDEX: 12345,  # filled in later
            para.CURRENT_WP: 0,  # ???
            para.COORD_FRAME: 3,  # coord frame
            para.COMMAND: self.cmd,  # command
            #para.PARAM1: ???,
            para.PARAM3: 0,  # just is 0
            para.YAW: 0,  # yaw,
            para.AUTOCONT: 1,  # autocontinue
        }
    
    def __str__(self):
        params = self.params()
        print(params)
        return "\t".join([params[i] for i in range(12)])
    
class Waypoint(QgcCommand):
    cmd = 16
    
    def __init__(self, x, y, z=5, radius=3):
        self.x = x
        self.y = y
        self.z = z
        self.radius = radius
    
    @classmethod
    def from_line(cls, parts):
        x = float(parts[para.LON])
        y = float(parts[para.LAT])
        z = float(parts[para.ALT])
        radius = float(parts[para.RADIUS])
        return cls(x,y,z,radius)
    
    @bases_overlay
    def params(self):
        return {
            para.LON: self.x,
            para.LAT: self.y,
            para.ALT: self.z,
            para.RADIUS: self.radius,
        }


class SplineWaypoint(Waypoint):
    cmd = 82

class ReturnToLaunch(Waypoint):
    cmd = 20

class Takeoff(QgcCommand):
    cmd = 22

class Land(Waypoint):
    cmd = 21

cmd_dct = {}
for cls in [Waypoint, SplineWaypoint, ReturnToLaunch, Takeoff, Land]:
    cmd_dct[cls.cmd] = cls
print(cmd_dct)

class Drone:
    pass

if __name__ == "__main__":
    f = QgcFile("mission_test.txt", read=True)
    raw_input()
    f.write()


