# chop
This is a C port of Dana Nau's pyhop, a very simple hierarchical ordered planner.

To build it: ./build
To run it: ./blocks_world_example

Pyhop is here: https://bitbucket.org/dananau/pyhop/src/

Pyhop is in turn a cut-down version of SHOP, which has several variants: https://www.cs.umd.edu/projects/shop/

I am using an object-oriented C idiom described here: http://nullprogram.com/blog/2014/10/21/

It leaks memory because it mallocs things and does not free anything.

