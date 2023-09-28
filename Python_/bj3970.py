import sys
from math import atan2, asin, pi
from functools import cmp_to_key


def cal_dist(d1, d2):  # get c = (a^2 + b^2)^.5
    return ((d1[0] - d2[0])**2 + (d1[1] - d2[1])**2)**.5


def comp(c1, c2):  # abs-y coord - x-coord - r
    if c1[1]-c1[2] > c2[1]-c2[2]:
        return 1
    elif c1[1]-c1[2] < c2[1]-c2[2]:
        return -1
    elif c1[1]-c1[2] == c2[1]-c2[2]:
        if c1[0] > c2[0]:
            return 1
        elif c1[0] < c2[0]:
            return -1
        elif c1[0] == c2[0]:
            if c1[2] < c2[2]:
                return 1
            elif c1[2] > c2[2]:
                return -1
            elif c1[2] == c2[2]:
                return 0


def is_c_in_c(c1, c2):  # c_n = (cx, cy, cr)
    x1, y1, r1 = c1
    x2, y2, r2 = c2
    vx, vy = x2 - x1, y2 - y1
    c2c = (vx * vx + vy * vy) ** .5
    if r1 >= c2c + r2 or r2 >= c2c + r1:
        return 1
    else:
        return 0


def get_slope(c1, c2):  # c_n = (cx, cy, cr)
    x1, y1, r1 = c1
    x2, y2, r2 = c2
    vx, vy = x2 - x1, y2 - y1
    c2c = (vx * vx + vy * vy) ** .5
    th = atan2(vy, vx)
    if th < 0:
        th += 2*pi
    p = asin((r2-r1)/c2c)
    th -= p
    if abs(th) < 0.00000001:
        th = 0.0
    elif th >= 2*pi:
        th -= 2*pi
    elif th < 0:
        th += 2*pi
    return th


def cal_hull_r(hull):
    r_h = 0.0
    lh = len(hull)
    for i in range(lh):
        c1, r, sp = hull[i-1]
        c2, _, sc = hull[i]
        t = sc - sp
        if t < 0:
            t += 2*pi
        r_h += r * t
        x1, y1, r1 = circles[c1]
        x2, y2, r2 = circles[c2]
        r_h += (cal_dist((x1, y1), (x2, y2))**2 - (r1-r2)**2)**.5
    return r_h


def dfs():
    hull = []
    min_s = 3*pi
    nxt = None
    p = 0
    for w in range(1, n):
        if not is_c_in_c(circles[p], circles[w]):
            tmp_s = get_slope(circles[p], circles[w])
            if min_s > tmp_s:
                min_s = tmp_s
                nxt = (w, circles[w][2], tmp_s)
    if nxt is None:
        return circles[0][2] * pi * 2
    hull.append(nxt)
    last = (0, nxt[0])
    while 1:
        min_s = 3 * pi
        # print(nxt)
        # print(last)
        p, s_p = nxt[0], nxt[2]
        for w in range(n):
            if w == p:
                continue
            if not is_c_in_c(circles[p], circles[w]):
                s_c = get_slope(circles[p], circles[w])
                tmp_s = s_c - s_p
                if tmp_s < 0:
                    tmp_s += 2*pi
                if min_s > tmp_s:
                    min_s = tmp_s
                    nxt = (w, circles[w][2], s_c)
        # print(nxt, (p, nxt[0]), last)
        if (p, nxt[0]) == last:
            # print(hull)
            return cal_hull_r(hull)
        hull.append(nxt)


for tc in range(int(sys.stdin.readline().strip())):
    n = int(sys.stdin.readline().strip())
    circles = sorted([tuple(map(float, sys.stdin.readline().strip().split())) for _ in range(n)], key=cmp_to_key(comp))
    # print(tc, circles)
    print(f"{dfs():.12f}")


'''
1
4
-71 -94 18
69 -15 97 
62 1 14
52 88 35



35
1
10.000 10.000 10.000
4
10.000 10.000 5.000
30.000 10.000 5.000
30.000 30.000 5.000
10.000 30.000 5.000
6
10.000 20.000 10.000
20.000 50.000 5.000
30.000 40.000 2.000
1.000 2.000 3.000
10.000 20.000 20.000
20.000 40.000 1.500
3
1.0 1.0 1.0
3.0 1.0 1.0
5.0 1.0 1.0
3
1.0 1.0 1.0
1.0 3.0 1.0
1.0 5.0 1.0
3
2 2 1
8 2 1
5 6 1
2
6 4 2
2 4 1
4
2 2 2
6 1 1
5 5 2
1 6 1
2
2 4 1
6 4 2
4
2 2 1
2 4 1
4 2 1
4 4 1
4
3 3 2
5 1 1
7 4 3
3 7 1
1
41.804 58.397 39.516
2
10.514 44.154 30.718
48.638 44.154 1.111
6
16.334 15.977 18.955
15.894 15.139 5.525
78.100 12.869 15.894
43.313 76.779 15.923
56.941 85.016 6.186
37.758 36.361 5.881
15
46.021 52.852 10.510
46.021 52.852 35.916
46.021 52.852 37.721
46.021 52.852 10.293
46.021 52.852 41.778
46.021 52.852 32.512
46.021 52.852 41.219
46.021 52.852 43.450
46.021 52.852 46.011
46.021 52.852 43.552
46.021 52.852 65.362
46.021 52.852 53.216
46.021 52.852 60.900
46.021 52.852 13.067
46.021 52.852 13.923
21
45.711 40.948 32.686
72.680 59.415 0.748
25.195 66.393 0.671
35.778 72.088 0.680
30.022 12.273 0.538
78.395 41.335 0.877
13.873 33.552 0.621
17.591 24.285 0.748
51.544 8.786 0.535
36.104 9.705 0.638
13.115 38.526 0.604
75.782 28.137 0.557
65.392 14.851 0.846
70.166 19.260 0.785
78.314 38.621 0.509
78.396 40.618 0.514
37.500 72.585 0.538
27.726 13.654 0.759
13.304 36.688 0.883
72.305 59.952 0.576
76.660 51.462 0.540
100
13.450 58.886 12.166
71.904 21.033 5.545
80.750 91.215 15.842
31.978 17.262 13.925
46.239 2.560 9.875
34.703 73.772 9.536
36.882 73.612 10.614
65.895 82.998 0.366
30.771 35.126 28.963
40.636 54.219 0.622
50.690 67.568 18.216
91.005 39.473 24.481
9.298 20.222 21.852
61.893 52.101 26.988
8.185 98.239 27.725
40.845 32.943 19.875
72.373 69.725 11.977
7.601 35.619 6.888
8.491 66.290 17.371
5.026 6.926 3.669
6.769 57.516 23.862
67.297 48.521 5.746
48.859 57.719 11.769
21.635 19.612 27.333
11.573 27.697 26.806
3.971 68.442 6.735
70.041 40.815 27.574
9.823 48.317 8.305
32.503 56.708 28.116
90.209 61.634 0.259
2.233 68.303 17.443
81.626 35.600 2.036
0.591 84.359 19.281
39.579 5.994 25.121
30.667 17.467 3.481
19.994 21.338 23.936
42.164 91.279 6.222
34.059 1.102 20.653
61.473 33.504 7.696
55.177 23.713 26.113
55.707 25.846 16.626
13.753 7.472 27.251
20.221 7.964 22.569
84.307 47.443 24.333
68.007 78.010 29.531
79.303 97.905 5.987
59.041 40.069 3.377
79.494 74.028 3.677
48.265 35.501 13.675
73.643 90.578 20.742
60.654 46.285 28.448
15.969 59.938 0.755
6.783 80.059 3.108
81.855 64.366 17.278
62.922 32.373 10.697
61.354 11.675 10.069
81.025 70.616 22.032
91.974 50.111 14.259
4.025 98.275 24.854
49.381 71.918 22.034
18.449 32.572 5.957
13.264 48.441 23.867
15.451 55.124 17.898
25.503 36.979 7.233
82.897 99.801 16.892
18.401 61.155 20.357
51.710 42.180 11.562
25.088 34.154 26.531
72.394 38.079 26.014
55.201 87.360 17.609
28.586 5.809 27.328
48.155 18.973 11.897
27.663 34.324 28.365
87.129 59.727 9.503
11.062 42.623 9.444
67.167 60.925 27.717
34.948 12.635 10.412
73.246 37.624 20.604
61.654 10.018 2.071
48.338 65.119 28.188
6.939 93.604 29.897
97.913 41.759 5.646
37.428 69.322 15.889
31.967 56.451 3.835
63.384 67.413 16.562
94.603 34.580 4.867
86.976 69.429 8.619
21.529 42.675 19.850
90.036 4.329 22.818
96.623 52.566 12.378
90.570 59.405 10.464
90.226 57.318 22.933
8.855 94.646 13.751
61.608 26.613 0.717
74.087 89.897 20.863
29.190 84.500 1.283
45.118 71.475 22.033
73.581 92.904 4.876
39.668 82.941 6.141
15.672 79.564 21.845
40
56.696 70.133 9.695
91.324 60.359 26.820
67.711 69.114 25.218
13.420 30.722 3.253
15.481 4.809 0.229
84.854 33.899 25.490
88.807 78.917 16.952
62.189 52.498 14.829
78.146 92.065 9.723
98.331 7.738 3.606
71.082 64.334 24.567
3.239 55.657 12.703
92.514 23.368 3.459
76.535 36.689 12.624
87.070 52.069 14.033
87.501 36.923 24.149
72.431 25.730 17.839
28.837 87.819 3.621
78.048 65.965 1.246
10.299 64.295 3.532
22.013 35.378 22.757
3.861 38.517 9.486
45.969 31.031 16.450
57.190 7.566 27.401
99.033 94.536 13.055
45.684 82.036 24.076
26.135 54.468 1.847
85.401 83.205 28.101
97.166 61.253 17.915
1.096 71.452 7.228
12.564 93.365 17.787
88.266 97.126 29.285
19.724 43.095 8.642
74.351 0.285 10.877
65.667 99.217 9.242
9.051 44.902 3.865
89.159 70.936 20.137
94.996 56.338 15.111
88.653 53.504 3.514
48.274 54.500 24.869
16
72.090 66.964 22.883
31.284 55.230 22.023
28.895 74.854 4.991
57.436 49.206 5.046
93.444 14.872 4.812
24.088 23.824 18.221
36.667 12.983 9.522
3.715 7.979 26.354
53.867 96.532 12.438
65.272 44.805 28.720
48.129 16.895 18.832
24.351 48.080 5.433
97.598 76.875 27.807
14.040 34.311 12.604
30.567 27.755 17.025
46.311 51.743 24.126
5
6.957 88.310 27.981
38.439 91.925 0.440
26.258 45.792 29.302
67.481 11.064 12.782
63.204 59.093 17.809
100
83.344 7.347 3.516
43.709 80.942 3.516
84.121 36.381 3.516
94.882 18.433 3.516
78.159 25.449 3.516
46.088 34.808 3.516
71.660 97.731 3.516
15.182 78.517 3.516
86.041 8.438 3.516
16.956 77.966 3.516
9.572 43.114 3.516
23.758 7.238 3.516
10.595 34.722 3.516
49.610 73.699 3.516
93.715 8.877 3.516
99.491 77.059 3.516
16.124 43.200 3.516
58.000 0.245 3.516
79.481 52.882 3.516
18.578 57.640 3.516
78.231 64.566 3.516
92.348 49.891 3.516
62.297 7.530 3.516
28.408 48.338 3.516
15.867 45.264 3.516
26.304 25.339 3.516
88.278 49.962 3.516
32.478 98.772 3.516
84.584 81.988 3.516
72.471 78.299 3.516
90.765 71.962 3.516
55.358 6.889 3.516
15.161 13.358 3.516
7.034 94.542 3.516
66.140 25.512 3.516
52.182 44.372 3.516
89.978 44.530 3.516
94.163 52.275 3.516
51.960 22.571 3.516
0.613 67.727 3.516
67.736 26.817 3.516
92.966 56.013 3.516
76.679 25.444 3.516
54.785 61.263 3.516
7.432 27.256 3.516
39.562 98.097 3.516
99.118 94.820 3.516
4.986 14.279 3.516
8.178 11.920 3.516
8.822 74.219 3.516
37.333 60.904 3.516
18.590 27.311 3.516
5.434 12.753 3.516
79.486 57.293 3.516
35.224 79.999 3.516
25.020 2.960 3.516
6.817 17.986 3.516
58.873 83.396 3.516
43.330 13.658 3.516
44.660 50.662 3.516
40.815 84.122 3.516
48.759 39.932 3.516
78.943 53.645 3.516
54.112 87.021 3.516
65.465 62.833 3.516
61.239 2.798 3.516
23.737 79.729 3.516
30.008 29.070 3.516
92.382 9.495 3.516
86.264 27.607 3.516
89.394 11.283 3.516
30.467 96.111 3.516
29.169 89.240 3.516
79.507 72.399 3.516
2.898 24.167 3.516
23.061 43.613 3.516
8.289 71.720 3.516
83.445 87.132 3.516
25.365 37.557 3.516
74.153 90.730 3.516
0.390 35.392 3.516
93.428 24.027 3.516
15.121 23.436 3.516
52.998 7.504 3.516
32.831 39.261 3.516
35.011 22.225 3.516
50.444 65.377 3.516
18.335 79.514 3.516
54.617 97.743 3.516
51.913 57.416 3.516
21.910 74.874 3.516
1.029 30.099 3.516
46.594 84.374 3.516
17.231 71.859 3.516
21.931 91.284 3.516
62.589 22.222 3.516
26.676 56.016 3.516
46.149 41.697 3.516
79.352 99.046 3.516
49.101 12.183 3.516
40
84.011 34.307 3.765
88.652 49.389 3.765
52.543 68.165 3.765
4.006 50.286 3.765
20.078 61.322 3.765
72.095 94.852 3.765
62.251 2.194 3.765
41.446 46.625 3.765
19.325 13.305 3.765
68.456 10.609 3.765
75.793 90.578 3.765
37.185 31.809 3.765
36.727 78.782 3.765
11.162 35.773 3.765
27.883 23.245 3.765
73.981 11.894 3.765
57.452 62.633 3.765
61.183 9.995 3.765
30.798 65.089 3.765
60.180 50.776 3.765
26.412 32.276 3.765
45.628 88.562 3.765
34.370 86.974 3.765
35.188 53.596 3.765
0.279 3.644 3.765
64.105 75.972 3.765
94.122 1.290 3.765
7.781 30.849 3.765
79.972 18.843 3.765
66.522 7.854 3.765
41.987 40.503 3.765
19.648 99.339 3.765
3.136 80.731 3.765
9.334 33.834 3.765
45.821 69.415 3.765
84.510 72.132 3.765
1.691 30.139 3.765
60.695 35.961 3.765
17.113 95.782 3.765
89.456 17.292 3.765
16
53.561 93.164 4.987
93.449 54.751 4.987
0.945 24.298 4.987
34.722 19.688 4.987
90.720 42.477 4.987
61.576 31.223 4.987
62.025 60.915 4.987
34.259 42.756 4.987
70.150 67.993 4.987
88.477 39.564 4.987
52.503 60.610 4.987
41.155 82.542 4.987
21.305 77.016 4.987
99.556 17.087 4.987
66.472 16.848 4.987
16.414 20.034 4.987
5
9.862 74.685 3.198
10.857 34.060 3.198
9.407 30.446 3.198
24.780 51.784 3.198
91.921 55.903 3.198
100
52.837 90.062 71.372
56.465 22.986 71.372
58.055 44.942 71.372
62.451 10.558 71.372
5.552 3.606 71.372
93.000 26.757 71.372
80.521 92.556 71.372
43.744 46.994 71.372
9.404 60.057 71.372
66.927 19.315 71.372
69.820 41.612 71.372
30.073 3.879 71.372
50.919 60.418 71.372
28.559 2.702 71.372
52.340 84.361 71.372
16.411 5.176 71.372
74.423 72.776 71.372
28.063 32.478 71.372
17.719 90.413 71.372
42.936 23.171 71.372
93.919 35.937 71.372
49.828 74.440 71.372
28.493 93.472 71.372
21.434 37.796 71.372
53.529 88.261 71.372
57.012 23.349 71.372
29.873 86.984 71.372
27.128 80.691 71.372
47.403 55.587 71.372
83.294 99.642 71.372
39.948 99.604 71.372
4.819 14.371 71.372
72.381 32.782 71.372
46.749 89.999 71.372
23.195 89.585 71.372
13.170 17.114 71.372
25.522 62.898 71.372
91.454 53.915 71.372
56.369 12.888 71.372
91.611 9.898 71.372
1.149 48.623 71.372
33.147 30.922 71.372
35.607 60.175 71.372
11.613 82.910 71.372
15.761 94.807 71.372
82.552 55.609 71.372
94.411 87.271 71.372
69.881 66.792 71.372
20.053 16.630 71.372
56.791 43.148 71.372
6.215 69.861 71.372
60.162 31.637 71.372
32.759 51.616 71.372
85.451 89.028 71.372
64.404 77.062 71.372
98.826 65.453 71.372
25.685 31.974 71.372
96.275 61.192 71.372
92.049 7.888 71.372
44.102 7.810 71.372
2.695 26.654 71.372
63.319 97.006 71.372
13.926 33.200 71.372
63.797 33.879 71.372
49.730 20.588 71.372
76.927 55.845 71.372
90.349 37.089 71.372
87.382 23.108 71.372
88.605 72.833 71.372
12.135 53.009 71.372
49.896 10.962 71.372
18.462 75.481 71.372
42.835 14.737 71.372
36.673 34.884 71.372
22.526 80.675 71.372
42.594 25.120 71.372
7.329 5.913 71.372
22.126 21.155 71.372
39.014 85.823 71.372
54.934 88.643 71.372
6.411 31.861 71.372
44.488 96.660 71.372
68.849 31.870 71.372
19.768 57.454 71.372
4.703 31.803 71.372
10.463 54.499 71.372
42.665 28.825 71.372
29.979 85.401 71.372
43.463 66.552 71.372
20.285 65.888 71.372
47.227 62.779 71.372
90.908 54.456 71.372
68.592 13.035 71.372
75.512 7.606 71.372
98.758 30.446 71.372
96.149 5.169 71.372
62.206 40.637 71.372
1.830 31.055 71.372
72.407 21.498 71.372
88.409 77.010 71.372
40
98.772 31.509 75.315
95.767 27.597 75.315
61.389 81.168 75.315
70.959 27.941 75.315
1.452 36.848 75.315
75.068 64.131 75.315
27.756 29.524 75.315
32.724 40.691 75.315
5.036 40.229 75.315
39.449 35.381 75.315
36.378 44.518 75.315
97.487 76.916 75.315
46.248 28.543 75.315
49.323 67.646 75.315
16.952 26.333 75.315
20.847 15.723 75.315
57.743 16.614 75.315
43.220 19.131 75.315
97.681 14.180 75.315
46.972 99.034 75.315
50.927 22.040 75.315
63.165 78.584 75.315
51.464 95.788 75.315
19.274 56.400 75.315
36.018 58.623 75.315
91.681 72.296 75.315
3.141 89.169 75.315
49.212 49.288 75.315
17.711 98.435 75.315
16.934 34.563 75.315
24.768 37.681 75.315
50.187 82.411 75.315
54.194 93.307 75.315
1.542 51.876 75.315
7.487 48.414 75.315
50.909 58.314 75.315
70.354 14.074 75.315
36.898 21.818 75.315
9.863 56.072 75.315
78.118 45.780 75.315
16
69.799 18.077 71.461
17.735 58.968 71.461
67.188 66.924 71.461
76.579 65.623 71.461
83.758 11.143 71.461
90.291 21.439 71.461
61.229 72.702 71.461
75.533 54.536 71.461
74.144 27.409 71.461
61.923 22.558 71.461
78.218 20.237 71.461
92.812 92.192 71.461
57.034 14.630 71.461
2.055 13.106 71.461
92.648 47.736 71.461
27.701 62.448 71.461
5
45.337 21.416 76.568
32.901 12.261 76.568
97.895 98.424 76.568
95.918 9.038 76.568
88.715 17.357 76.568
16
7.044 6.176 5.985
7.044 30.816 5.985
7.044 55.455 5.985
7.044 80.094 5.985
31.683 6.176 5.985
31.683 30.816 5.985
31.683 55.455 5.985
31.683 80.094 5.985
56.322 6.176 5.985
56.322 30.816 5.985
56.322 55.455 5.985
56.322 80.094 5.985
80.961 6.176 5.985
80.961 30.816 5.985
80.961 55.455 5.985
80.961 80.094 5.985
4
3.614 2.092 7.319
3.614 26.417 7.319
27.940 2.092 7.319
27.940 26.417 7.319
12
19.832 10.667 8.620
42.371 10.667 8.620
19.832 33.206 8.620
42.371 33.206 8.620
15.485 10.667 4.273
19.832 6.320 4.273
46.718 10.667 4.273
42.371 6.320 4.273
15.485 33.206 4.273
19.832 37.553 4.273
46.718 33.206 4.273
42.371 37.553 4.273
10
17.153 18.668 8.067
27.308 27.677 8.067
18.108 19.514 8.067
45.766 44.051 8.067
40.791 39.637 8.067
38.855 37.920 8.067
53.458 50.875 8.067
39.460 38.456 8.067
37.858 37.035 8.067
50.876 48.584 8.067
20
57.834 75.060 17.529
43.773 52.573 9.710
62.437 70.350 10.955
52.889 75.012 20.726
39.797 51.144 11.230
60.910 62.228 5.809
61.869 78.866 17.770
54.798 69.451 15.270
22.245 47.374 19.855
58.528 63.590 8.397
38.933 47.049 8.696
40.408 65.675 21.824
41.772 66.467 21.531
48.778 52.845 6.644
31.801 42.792 10.139
37.321 42.662 6.431
60.414 77.119 17.399
29.787 53.794 19.780
68.944 70.747 7.000
52.871 62.881 11.560
20
79.363 48.711 6.056
77.813 58.499 6.056
73.314 67.328 6.056
66.307 74.336 6.056
57.477 78.834 6.056
47.689 80.385 6.056
37.901 78.834 6.056
29.072 74.336 6.056
22.064 67.328 6.056
17.565 58.499 6.056
16.015 48.711 6.056
17.565 38.923 6.056
22.064 30.093 6.056
29.072 23.086 6.056
37.901 18.587 6.056
47.689 17.037 6.056
57.477 18.587 6.056
66.307 23.086 6.056
73.314 30.093 6.056
77.813 38.923 6.056
16
77.031 42.063 5.658
75.343 52.723 5.658
70.443 62.339 5.658
62.812 69.971 5.658
53.196 74.870 5.658
42.536 76.559 5.658
31.876 74.870 5.658
22.260 69.971 5.658
14.629 62.339 5.658
9.729 52.723 5.658
8.041 42.063 5.658
9.729 31.404 5.658
14.629 21.788 5.658
22.260 14.156 5.658
31.876 9.257 5.658
42.536 7.568 5.658
'''