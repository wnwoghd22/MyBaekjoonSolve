def dfs(d, v, s):  # 백트래킹 함수
    global ans
    if d == N:
        ans = max(ans, s)
    for i in range(N):      # 만일 v & 1 << i 라면 아직 해당 열의 과녁을 맞추지 않았다.
        if not v & 1 << i:
            dfs(d+1, v | 1 << i, s+target[d][i])
            # d = depth, v = visited, s = sum
            # 최소값이었으면 가지치기를 하려고 했는데 최대값이네요.
            # 8! = 40320이라서 연산 횟수가 많지 않아 가지치기를 하지 말라는 의도로 해석했습니다.


for tc in range(int(input())):
    N = int(input())
    target = [tuple(map(int, input().split())) for _ in range(N)]
    ans = 0
    dfs(0, 0, 0)
    print(f'#{tc+1} {ans}')

'''
11
11
100 1 1 1 1 1 1 1 1 1 1
1 100 1 1 1 1 1 1 1 1 1
1 1 100 1 1 1 1 1 1 1 1
1 1 1 100 1 1 1 1 1 1 1
1 1 1 1 100 1 1 1 1 1 1
1 1 1 1 1 100 1 1 1 1 1
1 1 1 1 1 1 100 1 1 1 1
1 1 1 1 1 1 1 100 1 1 1
1 1 1 1 1 1 1 1 100 1 1
1 1 1 1 1 1 1 1 1 100 1
1 1 1 1 1 1 1 1 1 1 100

10
8
100 1 1 1 1 1 1 1
1 100 1 1 1 1 1 1
1 1 100 1 1 1 1 1
1 1 1 100 1 1 1 1
1 1 1 1 100 1 1 1
1 1 1 1 1 100 1 1
1 1 1 1 1 1 100 1
1 1 1 1 1 1 1 100
8
100 1 1 1 1 1 1 1
1 100 1 1 1 1 1 1
1 1 100 1 1 1 1 1
1 1 1 100 1 1 1 1
1 1 1 1 100 1 1 1
1 1 1 1 1 100 1 1
1 1 1 1 1 1 100 1
1 1 1 1 1 1 1 100
8
100 1 1 1 1 1 1 1
1 100 1 1 1 1 1 1
1 1 100 1 1 1 1 1
1 1 1 100 1 1 1 1
1 1 1 1 100 1 1 1
1 1 1 1 1 100 1 1
1 1 1 1 1 1 100 1
1 1 1 1 1 1 1 100
8
100 1 1 1 1 1 1 1
1 100 1 1 1 1 1 1
1 1 100 1 1 1 1 1
1 1 1 100 1 1 1 1
1 1 1 1 100 1 1 1
1 1 1 1 1 100 1 1
1 1 1 1 1 1 100 1
1 1 1 1 1 1 1 100
8
100 1 1 1 1 1 1 1
1 100 1 1 1 1 1 1
1 1 100 1 1 1 1 1
1 1 1 100 1 1 1 1
1 1 1 1 100 1 1 1
1 1 1 1 1 100 1 1
1 1 1 1 1 1 100 1
1 1 1 1 1 1 1 100
8
100 1 1 1 1 1 1 1
1 100 1 1 1 1 1 1
1 1 100 1 1 1 1 1
1 1 1 100 1 1 1 1
1 1 1 1 100 1 1 1
1 1 1 1 1 100 1 1
1 1 1 1 1 1 100 1
1 1 1 1 1 1 1 100
8
100 1 1 1 1 1 1 1
1 100 1 1 1 1 1 1
1 1 100 1 1 1 1 1
1 1 1 100 1 1 1 1
1 1 1 1 100 1 1 1
1 1 1 1 1 100 1 1
1 1 1 1 1 1 100 1
1 1 1 1 1 1 1 100
8
100 1 1 1 1 1 1 1
1 100 1 1 1 1 1 1
1 1 100 1 1 1 1 1
1 1 1 100 1 1 1 1
1 1 1 1 100 1 1 1
1 1 1 1 1 100 1 1
1 1 1 1 1 1 100 1
1 1 1 1 1 1 1 100
8
100 1 1 1 1 1 1 1
1 100 1 1 1 1 1 1
1 1 100 1 1 1 1 1
1 1 1 100 1 1 1 1
1 1 1 1 100 1 1 1
1 1 1 1 1 100 1 1
1 1 1 1 1 1 100 1
1 1 1 1 1 1 1 100
8
100 1 1 1 1 1 1 1
1 100 1 1 1 1 1 1
1 1 100 1 1 1 1 1
1 1 1 100 1 1 1 1
1 1 1 1 100 1 1 1
1 1 1 1 1 100 1 1
1 1 1 1 1 1 100 1
1 1 1 1 1 1 1 100

'''