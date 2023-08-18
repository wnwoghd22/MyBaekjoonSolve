# import sys
# sys.stdin = open('input_maze.txt', 'r')
from collections import deque


def dfs(r, c):
    stack.append((r, c))
    visited[r][c] = 1
    while stack:
        r, c = stack[-1]
        temp = 1
        for k in range(4):
            nr = r + dr[k]
            nc = c + dc[k]
            if 0 <= nr < n and 0 <= nc < n and maze[nr][nc] == 3:
                return r, c
            if 0 <= nr < n and 0 <= nc < n and not maze[nr][nc] and not visited[nr][nc]:
                stack.append((nr, nc))
                visited[nr][nc] = visited[r][c] + 1
                temp = 0
                break
        if temp:
            stack.pop()
    return 0


for tc in range(1, int(input())+1):
    n = int(input())
    maze = [list(map(int, list(input()))) for _ in range(n)]
    visited = [[0]*n for _ in range(n)]
    stack = []
    dr = (1, 0, -1, 0)
    dc = (0, 1, 0, -1)
    flag = 0
    for i in range(n):
        for j in range(n):
            if maze[i][j] == 2:
                goal = dfs(i, j)
                if goal:
                    x, y = goal
                    print(f'#{tc} {visited[x][y]-1}')
                else:
                    print(f'#{tc} 0')
                flag = 1
                break
        if flag:
            break
