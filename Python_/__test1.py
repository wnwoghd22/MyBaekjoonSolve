K, R, N = input().split()     # 킹, 돌, 횟수

KX = int(ord(K[0]) - 65)        # 킹의 x축 좌표
KY = int(K[1]) - 1         # 킹의 y축 좌표

RX = int(ord(R[0]) - 65)         # 돌의 x축 좌표
RY = int(R[1]) - 1         # 돌의 y축 좌표

P = { 'R':(0, 1),'L':(0, -1),'B':(-1, 0),'T':(1, 0),
      'RT':(1, 1),'LT':(1, -1),'RB':(-1, 1), 'LB':(-1, -1)}
# 8방향

for i in range(int(N)):
    U = input()
    KNX = KX + P[U][1]
    KNY = KY + P[U][0]
    if 0 <= int(KNX) < 8 and 0 <= int(KNY) < 8:
        # 체스판 밖으로 나갔는지 if문으로 확인해준다.
        if KNX == RX and KNY == RY:
            # 그 움직인 자리에 돌이 있는지 확인해주고 , 돌도 똑같이 더해준다.
            RNX = RX + P[U][1]
            RNY = RY + P[U][0]
            if 0 <= int(RNX) < 8 and 0 <= int(RNY) < 8:
                # 안 나갔다면 킹은 돌 자리에 돌은 더한 자리에 각각 움직임.
                KX = KNX, KY = KNY, RX = RNX, RY = RNY
                # 근데 만약에 그 움직인 돌이 체스판 밖으로 나간 것이면, continue
            else:
                continue
        else:
            KX = KNX
            KY = KNY

K_result = chr(KX+65)
R_result = chr(RX+65)

print(K_result + str(KY+1))
print(R_result + str(RY+1))


######

