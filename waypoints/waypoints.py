
# TODO 
# WTF at 4th param
# higher resolution than double for GPS

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

#def bases_overlay(f):
    #@functools.wraps(f)
    #def method(self, *args, **kwargs):
        #bases_dct = {}
        #print(self.__class__, self.__class__.__bases__)
        #for b in self.__class__.__bases__:
            #print("TRYING", b)
            #try:
                #print("trying func", getattr(b, f.__name__))
                #bases_dct.update(super(b, self).__getattribute__()(*args, **kwargs))
            #except Exception as e:
                #print(e)
                #pass
        #own_dct = f(self, *args, **kwargs)
        #for k, v in bases_dct.items():
            #if k not in own_dct:
                #own_dct[k] = v
        #return own_dct
    #return method

def merge(*dicts):
    total = {}
    for d in dicts:
        total.update(d)
    return total

class AttrDict(dict):
    def __init__(self, *args, **kwargs):
        super(AttrDict, self).__init__(*args, **kwargs)
        self.__dict__ = self

class Point(object):
    @classmethod
    def tup(t):
        return Point(t[0], t[1])
    
    def __init__(self, x, y):
        self.x = x
        self.y = y
    
    __add__ = lambda a, b: Point(a.x+b.x, a.y+b.y)
    __sub__ = lambda a, b: Point(a.x-b.x, a.y-b.y)
    
    def __mul__(a, b):
        if isinstance(a, Point):
            return Point(a.x*b, a.y*b)
        else:
            return Point(b.x*a, b.y*a)
    
    __rmul__ = lambda a,b: b.__mul__(a)
    
    def __div__(a, b):
        if isinstance(a, Point):
            return Point(a.x/b, a.y/b)
        else:
            return Point(b.x/a, b.y/a)
    
    __rdiv__ = lambda a,b: b.__div__(a)
        


# Reference fields
# ----------------

UAntwerpen = AttrDict({
    "a": Point(51.184084, 4.419922), # left upper
    "b": Point(51.184160, 4.420502), # right upper
    "c": Point(51.183812, 4.420029), # left down
    # "d": Point(51.183812, 4.420029)
})


class WaypointFile(object):
    version = 110
    
    def __init__(self, name, read=False):
        self.name = name
        if read:
            with open(name, "r") as f:
                #print("#"+f.readline()+"#")
                #assert f.readline() == "QGC WPL 110\n"
                self.commands = [QgcCommand.from_line(l.rstrip("\n")) for l in f.readlines() if len(l) > 2]
                print("Loaded", len(self.commands), "commands")
        else:
            self.commands = []
    
    def write(self):
        #print("\n".join([str(c) for c in self.commands]))
        with open(self.name, "w") as f:
            f.write("QGC WPL 110\n")
            f.write("\n".join([c.to_str(i+1) for i, c in enumerate(self.commands)]))
            f.write("\n")
    
    @classmethod
    def import_route(cls, course, name, reference=UAntwerpen):
        wp = []
        base_x = reference.b - reference.a
        base_y = reference.c - reference.a
        print "BASE", base_x, base_y
        for _twp in course.points:
            x, y = float(_twp[0])/100.0, float(_twp[1])/100.0
            assert 0 <= x <= 1
            assert 0 <= y <= 1
            print x, y
            twp = Point(x, y)
            
            offset_x = base_x*x
            offset_y = base_y*y
            offset = offset_x+offset_y
            
            wp.append(Waypoint(reference.a.x + offset.x, reference.a.y + offset.y))
        f = WaypointFile(name)
        f.commands = wp
        return f
            

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
        return {
            para.INDEX: 12345,  # filled in later
            para.CURRENT_WP: 0,  # ???
            para.COORD_FRAME: 3,  # coord frame
            para.COMMAND: self.cmd,  # command
            para.PARAM1: 0,  # still not sure what it is; but just leave it at 0
            para.PARAM3: 0,  # just is 0
            para.YAW: 0,  # yaw,
            para.AUTOCONT: 1,  # autocontinue
        }
    
    def to_str(self, i):
        params = self.params()
        params[para.INDEX] = i
        return "\t".join([str(params[i]) for i in range(12)])


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
    
    def params(self):
        return merge(QgcCommand.params(self), {
            para.LON: self.x,
            para.LAT: self.y,
            para.ALT: self.z,
            para.RADIUS: self.radius,
        })


# TODO make these usable

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


if __name__ == "__main__":
    f = WaypointFile("mission_test.txt", read=True)
    f.write()

