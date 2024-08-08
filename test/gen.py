import random
from collections import defaultdict

COMMANDS = "+-:"
N = 50000
Q = 500000



print(N, Q)
q=0

curr=defaultdict(lambda: 0)

while 1:
    comm = random.choice(COMMANDS)
    if comm == ":":
        print(comm)
    if comm == "+":
        a = random.randint(1, N)
        b = random.randint(1, N)
        if a==b:
            continue

        p=(min(a,b), max(a,b))

        curr[p]+=1

        print(comm, a, b)
    if comm == "-":
        if len(curr)==0:
            continue
        edge = random.choice(list(curr.keys()))
        
        curr[edge]-=1
        if curr[edge]==0:
            curr.pop(edge)

        print(comm, *edge)

    q+=1
    if q==Q:
        break
