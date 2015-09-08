import pyhop

def move_packages(state, goal):
    truck = state.trucks[0]
    accum = []
    for package in goal.pos:
        if state.pos[package] != goal.pos[package]:
            accum.append(('pickup', package, truck))
            accum.append(('dropoff', package, truck, goal.pos[package]))
    return accum

pyhop.declare_methods('move_packages', move_packages)



