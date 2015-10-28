from __future__ import print_function
from pyhop import *

import driverlog_operators
print('')
print_operators()

import driverlog_methods
print('')
print_methods()

state1 = State('state1')
state1.gensym=0
state1.pos={'p1':'nw', 'p2':'nw'}
state1.now={'p1':'p1', 'p2':'p2', 't1':'t1', 't2':'t2'}
state1.trucks=['t1', 't2']
print_state(state1)
goal1 = Goal('goal1')
goal1.pos={'p1':'e', 'p2':'e'}
print_goal(goal1)
pyhop(state1,[('move_packages', goal1)], verbose=1)
