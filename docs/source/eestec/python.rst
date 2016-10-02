
==============
 Python Intro
==============

We strongly advise `this introductory tutorial <http://ai.berkeley.edu/tutorial.html>`_. It's meant for students at Berkeley, but it works for you too. You can go through it at your own pace, ask questions where needed. If you're done, please let us know as well.

Note that we will be using Python 2.7 for most of the programming.

Coming from C
=============

A quick comment for those coming from C: Python is quite different, and you'll have to relearn a lot of stuff. Luckily it'll go fast, Python is an easy language. Just try to not compare everything with C.

One quick tip: C is not usually considered an Object-Oriented language, but if you've ever done something like this in C::

    struct drone {
        int x;
        int y;
    }
    
    void move(struct drone* drone, int x, int y) {
        drone->x = x;
        drone->y = y;
    }

Would translate to this in Python::

    class Drone:
        def move(self, x, y):
            self.x = x
            self.y = y

You can of course extend the Python example with an ``__init__`` function, and you get a lot more OO tricks than in C.


Coming from Java
================

You're in luck; Object-Oriented programming is Java's bread and butter and Python will happily handle any OO programming you throw at it. An interesting article discussing how you should change your coding style can be found here: `Python is not Java <http://dirtsimple.org/2004/12/python-is-not-java.html>`_.

While Python is also OO-oriented, try not to put everything in classes, free standing functions are better than static methods in ``Util`` classes. Also, MVC has no place in drones :) .
