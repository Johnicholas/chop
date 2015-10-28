import pyhop

"pickup <package> with <truck>"
"dynamic properties in o's effects:"
"position of package, the truck"
"dynamic properties that appear in o"
def pickup(state, package, truck, prereqs, finish):
    state.gensym = finish
    state.now[package] = finish
    state.now[truck] = finish
    state.pos[package] = truck
    return state

"dropoff <package>, which is currently in <truck>, at <location>"
def dropoff(state, package, truck, location, prereqs, finish):
    if state.pos[package] == truck:
        state.gensym = finish
        state.now[package] = finish
        state.now[truck] = finish
        state.pos[package] = location
        return state
    else:
        return False

"tell pyhop what the operators are"
pyhop.declare_operators(pickup, dropoff)

