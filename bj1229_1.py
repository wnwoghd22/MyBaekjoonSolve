import sys

hexagon_no = [i*(i*2-1) for i in range(1, 708)]

# N = 1000000
#
# dp = [1000001] * (N+1)
# for i in range(1, N+1):
#     for h in hexagon_no:
#         if i == h:
#             dp[i] = 1
#         if i < h:
#             break
#         dp[i] = min(dp[i], dp[i-h]+1)
#
# four = []
# five = []
# six = []
# for i in range(1, 1000001):
#     if dp[i] == 4:
#         four.append(i)
#     if dp[i] == 5:
#         five.append(i)
#     if dp[i] == 6:
#         six.append(i)
# print(four)
# print(five)
# print(six)

four = [4, 9, 14, 19, 23, 24, 32, 33, 37, 41, 42, 48, 50, 53, 55, 59, 63, 64, 69, 76, 77, 80, 83, 85, 86, 89, 99, 102, 104, 108, 110, 113, 115, 116, 123, 124, 128, 129, 131, 140, 143, 144, 145, 146, 152, 161, 162, 167, 170, 173, 175, 178, 179, 184, 189, 194, 195, 200, 203, 207, 208, 215, 216, 221, 222, 228, 229, 230, 242, 249, 251, 253, 254, 258, 266, 267, 269, 270, 275, 286, 290, 293, 294, 295, 299, 300, 308, 313, 314, 315, 317, 320, 324, 329, 330, 333, 345, 356, 361, 362, 365, 369, 374, 375, 377, 383, 389, 400, 403, 404, 405, 410, 414, 418, 420, 428, 432, 439, 440, 443, 448, 452, 453, 454, 455, 476, 483, 485, 488, 492, 505, 509, 518, 519, 534, 538, 540, 545, 548, 550, 551, 566, 572, 575, 578, 579, 585, 599, 605, 608, 614, 620, 623, 638, 639, 641, 648, 657, 662, 663, 671, 674, 683, 685, 689, 698, 706, 713, 723, 725, 730, 734, 735, 744, 753, 764, 767, 768, 785, 790, 804, 830, 833, 845, 850, 854, 855, 859, 869, 878, 888, 896, 905, 910, 918, 920, 923, 924, 929, 944, 960, 963, 964, 968, 969, 986, 988, 995, 1001, 1004, 1016, 1031, 1033, 1049, 1055, 1088, 1104, 1115, 1118, 1163, 1168, 1175, 1180, 1193, 1208, 1235, 1238, 1251, 1262, 1269, 1273, 1274, 1283, 1300, 1301, 1303, 1307, 1320, 1329, 1340, 1343, 1349, 1350, 1364, 1383, 1394, 1395, 1397, 1400, 1403, 1439, 1440, 1473, 1478, 1490, 1493, 1496, 1499, 1505, 1509, 1518, 1520, 1538, 1539, 1543, 1548, 1554, 1580, 1581, 1610, 1614, 1615, 1619, 1620, 1628, 1640, 1645, 1678, 1692, 1700, 1724, 1733, 1748, 1763, 1765, 1790, 1797, 1805, 1819, 1838, 1863, 1868, 1886, 1895, 1904, 1940, 1945, 1970, 1977, 1991, 2000, 2013, 2015, 2024, 2025, 2054, 2065, 2078, 2079, 2090, 2093, 2105, 2130, 2138, 2143, 2144, 2153, 2183, 2224, 2228, 2230, 2238, 2245, 2246, 2255, 2315, 2333, 2355, 2366, 2373, 2390, 2408, 2438, 2493, 2504, 2508, 2528, 2533, 2544, 2573, 2610, 2615, 2624, 2636, 2654, 2673, 2687, 2693, 2705, 2750, 2760, 2783, 2786, 2813, 2814, 2888, 2915, 2973, 2984, 3014, 3035, 3039, 3074, 3083, 3089, 3128, 3158, 3177, 3219, 3258, 3275, 3293, 3305, 3363, 3368, 3380, 3386, 3390, 3403, 3425, 3479, 3506, 3533, 3599, 3608, 3616, 3620, 3650, 3719, 3725, 3740, 3770, 3773, 3815, 3850, 3855, 3905, 3953, 3974, 3989, 4003, 4043, 4067, 4070, 4088, 4128, 4148, 4155, 4178, 4199, 4239, 4250, 4262, 4301, 4310, 4328, 4370, 4454, 4469, 4488, 4495, 4533, 4568, 4583, 4640, 4660, 4670, 4673, 4703, 4805, 4860, 4968, 5090, 5105, 5183, 5228, 5288, 5339, 5360, 5375, 5387, 5445, 5485, 5523, 5528, 5585, 5651, 5774, 5849, 5885, 5888, 5903, 5951, 6004, 6035, 6081, 6140, 6173, 6224, 6234, 6344, 6380, 6383, 6389, 6449, 6515, 6525, 6548, 6554, 6590, 6605, 6613, 6635, 6675, 6998, 7065, 7160, 7175, 7241, 7298, 7343, 7360, 7370, 7395, 7481, 7560, 7565, 7778, 7820, 7835, 7898, 8036, 8060, 8120, 8180, 8183, 8258, 8289, 8303, 8483, 8595, 8853, 9095, 9104, 9110, 9149, 9158, 9218, 9243, 9293, 9329, 9395, 9458, 9575, 9815, 9843, 9848, 9860, 9908, 10004, 10060, 10095, 10205, 10436, 10485, 10523, 10913, 11058, 11165, 11297, 11339, 11400, 11480, 11493, 11645, 11948, 11960, 12048, 12113, 12195, 12353, 12548, 12668, 12950, 13110, 13148, 13388, 13448, 13920, 13938, 14108, 14348, 14795, 14945, 15167, 15191, 15803, 16115, 16433, 16508, 16610, 16965, 17000, 17025, 17135, 17603, 17933, 17963, 18323, 18821, 18968, 19268, 19280, 19478, 19520, 19763, 20463, 20570, 21335, 21398, 21635, 22028, 22100, 22280, 22760, 22880, 23939, 24011, 24170, 24350, 24585, 24695, 25340, 25695, 25892, 26270, 26753, 26990, 29733, 30743, 31394, 31568, 32075, 32915, 32975, 34085, 35783, 35883, 36230, 36908, 37244, 38390, 40388, 41105, 43755, 44195, 51623, 52550, 55658, 61403, 62168, 64413, 78585, 109250, 146858]
five = [5, 10, 20, 25, 38, 39, 54, 65, 70, 114, 130]
six = [11, 26]
dp2 = [0] * 1000001
for i in range(707):
    h1 = hexagon_no[i]
    dp2[h1] = 1
    for j in range(i, 707):
        h2 = hexagon_no[j]
        if h1 + h2 > 1000000:
            break
        dp2[h1 + h2] = 2
for i in four:
    dp2[i] = 4
for i in five:
    dp2[i] = 5
for i in six:
    dp2[i] = 6
N = int(sys.stdin.readline().strip())
print(dp2[N] if dp2[N] else 3)
