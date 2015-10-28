import pyhop
import sys

"pickup this package with this truck, at the earliest possible time"
def pickup_method(state, package, truck):
    start = max(state.now[package], state.now[truck])
    return [('pickup', package, truck, start, 1)]


"dropoff this package from this truck at this location"
def dropoff_method(state, package, truck, location):
    start = max(state.now[package], state.now[truck])
    return [('dropoff', package, truck, location, start, 1)]

def move_packages(state, goal):
    for package in goal.pos:
        if state.pos[package] != goal.pos[package]:
            which_truck = state.trucks[0]
            best_so_far = state.now[which_truck]
            for truck in state.trucks:
                if state.now[truck] < best_so_far:
                    which_truck = truck
                    best_so_far = state.now[truck]
            return [
                ('pickup_method', package, which_truck),
                ('dropoff_method', package, which_truck, goal.pos[package]),
                ('move_packages', goal)
            ]
    return []

pyhop.declare_methods('pickup_method', pickup_method)
pyhop.declare_methods('dropoff_method', dropoff_method)
pyhop.declare_methods('move_packages', move_packages)



