import pyhop

"pickup this package with this truck, at the earliest possible time"
def pickup_method(state, package, truck):
    prereqs = {state.now[package], state.now[truck]}
    finish = state.gensym + 1
    return [('pickup', package, truck, prereqs, finish)]

"dropoff this package from this truck at this location"
def dropoff_method(state, package, truck, location):
    prereqs = {state.now[package], state.now[truck]}
    finish = state.gensym + 1
    return [('dropoff', package, truck, location, prereqs, finish)]

"a helper for move_packages"
def round_robin(list):
    while True:
        for element in list:
            yield element
    
def move_packages(state, goal):
    truck_generator = round_robin(state.trucks)
    accum = []
    for package in goal.pos:
        dest = goal.pos[package]
        if state.pos[package] != dest:
            which_truck = next(truck_generator)
            accum.append(('pickup_method', package, which_truck))
            accum.append(('dropoff_method', package, which_truck, dest))
    return accum

pyhop.declare_methods('pickup_method', pickup_method)
pyhop.declare_methods('dropoff_method', dropoff_method)
pyhop.declare_methods('move_packages', move_packages)



