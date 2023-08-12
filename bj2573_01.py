import sys
from collections import deque
# sys.stdin = open('___2573_input.txt', 'r')


def bfs(x, y, p):
    queue.append((x, y))
    visited[x][y] = p
    while queue:
        r, c = queue.popleft()
        for z in range(4):
            nr = r + dr[z]
            nc = c + dc[z]
            if 0 <= nr < N and 0 <= nc < M and grid[nr][nc] and visited[nr][nc] != p:
                queue.append((nr, nc))
                visited[nr][nc] = p
            if 0 <= nr < N and 0 <= nc < M and not grid[nr][nc]:
                melt.append((r, c))


N, M = map(int, sys.stdin.readline().strip().split())
grid = [list(map(int, sys.stdin.readline().strip().split())) for _ in range(N)]
visited = [[-1] * M for _ in range(N)]
queue = deque()
melt = []  # stack
dr = (1, 0, -1, 0)
dc = (0, 1, 0, -1)

ans = 0
day = 0
while True:
    # for row in grid:
    #     print(*row)
    # print()

    cnt = 0
    for i in range(N):
        for j in range(M):
            if grid[i][j] and visited[i][j] != day:
                bfs(i, j, day)
                cnt += 1
    if cnt > 1:
        ans = day
        break
    if not cnt:
        # print(day)
        ans = 0
        break
    for i, j in melt:
        if grid[i][j] > 0:
            grid[i][j] -= 1
    melt.clear()
    day += 1
print(ans)

'''
8 10
4 3 2 2 1 2 5 5 6 6
3 3 3 2 1 2 4 5 5 6
2 2 2 2 1 2 3 3 3 3
2 1 4 3 3 2 2 3 4 5
1 1 0 3 2 3 2 2 2 3
0 0 0 1 2 2 0 1 1 1
0 1 2 1 1 1 0 1 0 1
0 1 1 1 2 1 0 0 0 0

12 12
0 0 0 0 0 0 0 0 0 0 0 0
0 10 10 10 10 0 10 10 10 10 10 0
0 10 10 10 10 1 10 10 10 10 10 0
0 10 10 10 10 1 10 10 10 10 10 0
0 10 10 10 10 1 10 10 10 10 10 0
0 10 10 10 10 1 10 10 10 10 10 0
0 10 10 10 10 10 1 10 10 10 10 0
0 10 10 10 10 10 1 10 10 10 10 0
0 10 10 10 10 10 1 10 10 10 10 0
0 10 10 10 10 10 1 10 10 10 10 0
0 10 10 10 10 10 0 10 10 10 10 0
0 0 0 0 0 0 0 0 0 0 0 0

12 12
0 0 0 0 0 0 0 0 0 0 0 0
0 10 10 10 10 0 10 10 10 10 10 0
0 10 10 10 10 9 10 10 10 10 10 0
0 10 10 10 10 10 10 10 10 10 10 0
0 10 10 10 10 10 10 10 10 10 10 0
0 10 10 10 10 10 10 10 10 10 10 0
0 10 10 10 10 10 10 10 10 10 10 0
0 10 10 10 10 10 10 10 10 10 10 0
0 10 10 10 10 10 10 10 10 10 10 0
0 10 10 10 10 10 9 10 10 10 10 0
0 10 10 10 10 10 0 10 10 10 10 0
0 0 0 0 0 0 0 0 0 0 0 0

15 15
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 9 9 9 9 9 8 9 9 9 9 9 9 9 0
0 9 9 9 9 9 9 9 9 9 9 9 9 9 0
0 9 9 9 9 9 9 9 9 9 9 9 9 9 0
0 9 9 9 9 9 9 9 9 9 9 9 9 9 0
0 9 9 9 9 9 9 9 9 9 9 9 9 9 0
0 9 9 9 9 9 9 9 9 9 9 9 9 9 0
0 9 9 9 9 9 9 9 9 9 9 9 9 9 0
0 9 9 9 9 9 9 9 9 9 9 9 9 9 0
0 9 9 9 9 9 9 9 9 9 9 9 9 9 0
0 9 9 9 9 9 9 9 9 9 9 9 9 9 0
0 9 9 9 9 9 9 9 9 9 9 9 9 9 0
0 9 9 9 9 9 9 9 9 9 9 9 9 9 0
0 9 9 9 9 9 9 9 8 9 9 9 9 9 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0

8 10
1 2 3 2 1 2 5 5 6 6
2 2 3 2 1 2 4 5 5 6
3 3 3 2 1 2 3 3 3 3
3 2 1 1 1 2 2 3 4 5
2 1 0 0 0 1 2 2 2 3
1 0 0 1 1 1 0 1 1 1
0 0 2 1 1 1 0 1 0 1
0 1 1 1 2 1 0 0 0 0
'''
