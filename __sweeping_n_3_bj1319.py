import sys
from math import gcd
from functools import cmp_to_key


def cross(d1, d2, d3, d4):  # cross product / get CCW / get Torque / get area
    return (d2[0] - d1[0]) * (d4[1] - d3[1]) - (d2[1] - d1[1]) * (d4[0] - d3[0])


def comp(d1, d2):  # compare by slope
    return (d1[1] * d2[0] > d1[0] * d2[1]) - (d1[1] * d2[0] < d1[0] * d2[1])


N = int(sys.stdin.readline().strip())
trees = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(N)]
if N == 1:
    print(trees[0][2])
    exit(0)
if N == 2:
    print(abs(trees[0][2] - trees[1][2]))
    exit(0)

total = 0
for i in trees:
    total += i[2]
# print(total)

p = (0, 0)
diff = int(12e12)
for i in trees:
    dic = {}
    value_i = i[2]
    # print(value_i)
    for j in trees:
        if i == j:
            continue
        else:
            x, y = j[0] - i[0], j[1] - i[1]
            gcd_ = gcd(abs(x), abs(y))
            temp = (x // gcd_, y // gcd_)

            if temp in dic:
                dic[temp] += j[2]
            else:
                dic[temp] = j[2]

    tree_ = sorted(list(dic), key=cmp_to_key(comp))
    # print(tree_)
    # print(dic)
    for j in tree_:
        half = 0
        bound = value_i
        for k in tree_:
            if cross(p, j, j, k) > 0:
                half += dic[k]
            if cross(p, j, j, k) == 0:
                bound += dic[k]
        diff = min(abs(half - (total - half)), abs(half + dic[j] - (total - half - dic[j])),
                   abs(half + bound - dic[j] - (total - half - bound + dic[j])), diff)
        # print(diff)

print(diff)

'''
50
0 0 1
1 0 1
2 0 1
3 0 1
4 0 1
5 0 1
6 0 1
7 0 1
8 0 1
9 0 1
10 0 2
1 1 1
2 1 1
3 1 1
4 1 1
5 1 1
6 1 1
7 1 1
8 1 1
9 1 1
10 1 1
1 2 1
2 2 1
3 2 1
4 2 1
5 2 1
6 2 1
7 2 1
8 2 1
9 2 1
10 2 1
1 3 1
2 3 1
3 3 1
4 3 1
5 3 1
6 3 1
7 3 1
8 3 1
9 3 1
10 3 1
1 4 1
2 4 1
3 4 1
4 4 1
5 4 1
6 4 1
7 4 1
8 4 1
9 4 1
10 4 1
'''