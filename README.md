CHOP was going to be something like Dana Nau's Pyhop,
which is a cut-down version of Dana Nau's SHOP,
which stands for "simple hierarchical ordered planner".
Pyhop is a minimalistic version of SHOP, in python.

Driverlog is a planning domain, described here: 
https://www.cs.cmu.edu/afs/cs/project/jair/pub/volume20/long03a-html/node39.html

The idea is that there are drivers, trucks, packages, and locations,
and you need to create a plan for the drivers to walk to the trucks,
get in the trucks, drive them to the packages, pick up the packages,
drive the packages to their destinations, and drop off the packages.

This is a super simplified version with just trucks and packages,
and trucks can teleport to the packages, so there's no geometry involved.

The main function creates an initial state and a goal,
and delegates to chop.
Chop follows a straightforward procedure,
it round-robins through the available trucks,
picking up packages and putting them in their destinations.
This is a sequential construction,
but the trucks can execute the plan concurrently.

After the plan is constructed, Ceu is in control of the plan execution.

