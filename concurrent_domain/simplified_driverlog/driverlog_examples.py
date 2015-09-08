from __future__ import print_function
from pyhop import *

import driverlog_operators
print('')
print_operators()

import driverlog_methods
print('')
print_methods()

state1 = State('state1')
state1.pos={'a':'nw', 'b':'nw'}
state1.trucks=['t1', 't2']
print_state(state1)
goal1 = Goal('goal1')
goal1.pos={'a':'e', 'b':'e'}
print_goal(goal1)
pyhop(state1,[('move_packages', goal1)], verbose=1)
