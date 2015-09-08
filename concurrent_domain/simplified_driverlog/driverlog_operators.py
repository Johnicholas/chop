import pyhop

"pickup <package> with <truck>"
def pickup(state, package, truck):
    state.pos[package] = truck
    return state

"dropoff <package>, which is currently in <truck>, at <location>"
def dropoff(state, package, truck, location):
    if state.pos[package] == truck:
        state.pos[package] = location
        return state
    else:
        return False

"tell pyhop what the operators are"
pyhop.declare_operators(pickup, dropoff)

