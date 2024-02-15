import sys
from collections import deque
from math import hypot, atan2
TOL = 1e-7
INPUT = sys.stdin.readline


def z(x: float):
    return abs(x) < TOL


class Pos:
    def __init__(self, x_: float, y_: float):
        self.x = x_
        self.y = y_

    def __add__(self, p: 'Pos') -> 'Pos':
        return Pos(self.x + p.x, self.y + p.y)

    def __sub__(self, p: 'Pos') -> 'Pos':
        return Pos(self.x - p.x, self.y - p.y)

    def __floordiv__(self, p: 'Pos') -> float:
        return self.x * p.y - self.y * p.x

    def __pow__(self, p: 'Pos') -> float:
        return self.x * p.x + self.y * p.y

    def mag(self) -> float:
        return hypot(self.x, self.y)

    def __lt__(self, p):
        if self.x == p.x:
            return self.y < p.y
        return self.x < p.x


class Line:
    def __init__(self, vy_: float, vx_: float, c_: float):
        self.vy = vy_
        self.vx = vx_
        self.c = c_
        self.theta = atan2(vy_, -vx_)

    def __invert__(self) -> 'Line':
        return Line(-self.vx, self.vy, self.c)

    def __floordiv__(self, s: 'Line') -> float:
        return self.vy * s.vx - self.vx * s.vy


def line(s: Pos, e: Pos) -> Line:
    dy = e.y - s.y
    dx = s.x - e.x
    c = dy * s.x + dx * s.y
    return Line(dy, dx, c)


def r90(s: Line, p: Pos) -> Line:
    v = ~s
    c = v.vy * p.x + v.vx * p.y
    return Line(v.vy, v.vx, c)


def cross(d1: Pos, d2: Pos, d3: Pos) -> int or float:
    return (d2 - d1) // (d3 - d2)


def ccw(d1: Pos, d2: Pos, d3: Pos) -> int:
    ret = (d2 - d1) // (d3 - d2)
    if z(ret):
        return 0
    return 1 if ret > 0 else -1


def intersection(l1: Line, l2: Line) -> Pos:
    det = l1 // l2
    x_ = (l1.c * l2.vx - l2.c * l1.vx) / det
    y_ = (l2.c * l1.vy - l1.c * l2.vy) / det
    return Pos(x_, y_)


def monotone_chain(candi: list[Pos]) -> list[Pos]:
    candi.sort(key=lambda p: (p.x, p.y))
    if len(candi) <= 2:
        return candi
    lower = []
    for dl in candi:
        while len(lower) > 1 and ccw(lower[-2], lower[-1], dl) <= 0:
            lower.pop()
        lower.append(dl)
    upper = []
    for du in reversed(candi):
        while len(upper) > 1 and ccw(upper[-2], upper[-1], du) <= 0:
            upper.pop()
        upper.append(du)
    return lower[:-1] + upper[:-1]


def area(h_: list[Pos]) -> float:
    o = Pos(0, 0)
    a = 0
    for i in range(len(h_)):
        cur, nxt = h_[i - 1], h_[i]
        a += cross(o, cur, nxt)
    return a * .5


def bad(l1: Line, l2: Line, t: Line) -> bool:
    if l1 // l2 < TOL:
        return False
    p = intersection(l1, l2)
    return t.vy * p.x + t.vx * p.y > t.c - TOL


def half_plane_intersection(hp_: list[Line], hpi: list[Pos]) -> bool:
    dq = deque()
    hp_.sort(key=lambda s: s.theta)
    for hp in hp_:
        if dq and z(dq[-1] // hp):
            continue
        while len(dq) >= 2 and bad(dq[-2], dq[-1], hp):
            dq.pop()
        while len(dq) >= 2 and bad(dq[0], dq[1], hp):
            dq.popleft()
        dq.append(hp)
    while len(dq) >= 3 and bad(dq[-2], dq[-1], dq[0]):
        dq.pop()
    while len(dq) >= 3 and bad(dq[-1], dq[0], dq[1]):
        dq.popleft()
    for i in range(len(dq)):
        cur, nxt = dq[i - 1], dq[i]
        print(i)
        if cur // nxt < TOL:
            hpi.clear()
            return False
        hpi.append(intersection(cur, nxt))
    return True


def mirror(c: list[Pos], s1: Pos, s2: Pos) -> None:
    s = line(s1, s2)
    for i in range(len(c)):
        inx = intersection(s, r90(s, c[i]))
        d = inx - c[i]
        c[i] += d + d


if __name__ == '__main__':  # boj30447
    w, h = map(int, INPUT().split())
    x1, y1 = map(int, INPUT().split())
    x2, y2 = map(int, INPUT().split())
    p1 = Pos(x1, y1)
    p2 = Pos(x2, y2)
    if p2 < p1:
        p1, p2 = p2, p1
    box = [Pos(0, 0), Pos(w, 0), Pos(w, h), Pos(0, h)]
    CL, CR = [], []
    for b in box:
        if ccw(p1, p2, b) > 0:
            CL.append(b)
        if ccw(p1, p2, b) < 0:
            CR.append(b)

    mirror(CR, p1, p2)
    CL += [p1, p2]
    CR += [p1, p2]
    HL = monotone_chain(CL)
    HR = monotone_chain(CR)
    HP = [line(HL[i - 1], HL[i]) for i in range(len(HL))] + [line(HR[i - 1], HR[i]) for i in range(len(HR))]
    HPI, overlap = [], 0
    if not half_plane_intersection(HP, HPI):
        overlap = 0
    else:
        overlap = area(HPI)
    print(int(w * h - overlap))
