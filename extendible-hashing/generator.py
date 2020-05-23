import random, sys

x = int(sys.argv[1])

out = random.sample(range(0, x*2), x)

for i in range(x):
    print(out[i])
