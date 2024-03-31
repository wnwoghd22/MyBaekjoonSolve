#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <queue>
#include <deque>
#include <random>
#include <array>
#include <tuple>
#include <complex>
typedef long long ll;
//typedef long double ld;
typedef double ld;
const ld INF = 1e17;
const ld TOL = 1e-7;
const ld PI = acos(-1);
const int LEN = 1e3;
int N, M, T, Q;
bool V[LEN + 1][LEN + 1];
bool zero(const ld& x) { return std::abs(x) < TOL; }
int dcmp(const ld& x) { return std::abs(x) < TOL ? 0 : x > 0 ? 1 : -1; }
int dcmp(const ll& x) { return !x ? 0 : x > 0 ? 1 : -1; }
ld norm(ld th) {
	while (th < -TOL) th += PI * 2;
	while (th > PI * 2) th -= PI * 2;
	return th;
}
ld flip(ld lat) {
	if (zero(lat - PI * .5) || zero(lat + PI * .5)) return 0;
	if (zero(lat)) return PI * .5;
	if (lat > 0) return PI * .5 - lat;
	if (lat < 0) return -(PI * .5) - lat;
	return INF;
}
ll gcd(ll a, ll b) { return !b ? a : gcd(b, a % b); }
//ll gcd(ll a, ll b) {
//	while (b) {
//		ll tmp = a % b;
//		a = b;
//		b = tmp;
//	}
//	return a;
//}
struct Seq { int x, y; Seq(int X = 0, int Y = 0) : x(X), y(Y) {} };
std::vector<Seq> seq;

//Euler characteristic : v - e + f == 1
//Pick`s Theorem : A = i + b/2 - 1
//2D============================================================================//
//2D============================================================================//
//2D============================================================================//
struct Pos {
	ld x, y;
	Pos(ld X = 0, ld Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	bool operator != (const Pos& p) const { return !zero(x - p.x) || !zero(y - p.y); }
	bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ld& scalar) const { return { x * scalar, y * scalar }; }
	Pos operator / (const ld& scalar) const { return { x / scalar, y / scalar }; }
	ld operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
	ld operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
	Pos operator ~ () const { return { -y, x }; }
	ld operator ! () const { return x * y; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const ld& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const ld& scale) { x /= scale; y /= scale; return *this; }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return hypot(x, y); }
	Pos unit() const { return *this / mag(); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = { 0, 0 };
struct Vec {
	ld vy, vx;
	Vec(ld Y = 0, ld X = 0) : vy(Y), vx(X) {}
	bool operator == (const Vec& v) const { return (zero(vy - v.vy) && zero(vx - v.vx)); }
	bool operator < (const Vec& v) const { return zero(vy - v.vy) ? vx < v.vx : vy < v.vy; }
	ld operator * (const Vec& v) const { return vy * v.vy + vx * v.vx; }
	ld operator / (const Vec& v) const { return vy * v.vx - vx * v.vy; }
	Vec operator ~ () const { return { -vx, vy }; }
	Vec& operator *= (const ld& scalar) { vy *= scalar; vx *= scalar; return *this; }
	Vec& operator /= (const ld& scalar) { vy /= scalar; vx /= scalar; return *this; }
	ld mag() const { return hypot(vy, vx); }
}; const Vec Zero = { 0, 0 };
struct Line {//ax + by = c
	Vec s;
	ld c;
	Line(Vec V = Vec(0, 0), ld C = 0) : s(V), c(C) {}
	bool operator < (const Line& l) const {
		bool f1 = Zero < s;
		bool f2 = Zero < l.s;
		if (f1 != f2) return f1;
		ld CCW = s / l.s;
		return zero(CCW) ? c * hypot(l.s.vy, l.s.vx) < l.c * hypot(s.vy, s.vx) : CCW > 0;
	}
	ld operator * (const Line& l) const { return s * l.s; }
	ld operator / (const Line& l) const { return s / l.s; }
	Line operator + (const ld& scalar) const { return Line(s, c + hypot(s.vy, s.vx) * scalar); }
	Line operator - (const ld& scalar) const { return Line(s, c - hypot(s.vy, s.vx) * scalar); }
	Line operator * (const ld& scalar) const { return Line({ s.vy * scalar, s.vx * scalar }, c * scalar); }
	Line& operator += (const ld& scalar) { c += hypot(s.vy, s.vx) * scalar; return *this; }
	Line& operator -= (const ld& scalar) { c -= hypot(s.vy, s.vx) * scalar; return *this; }
	Line& operator *= (const ld& scalar) { s *= scalar, c *= scalar; return *this; }
	ld dist(const Pos& p) const { return s.vy * p.x + s.vx * p.y; }
	ld above(const Pos& p) const { return s.vy * p.x + s.vx * p.y - c; }
	ld mag() const { return s.mag(); }
	friend std::ostream& operator << (std::ostream& os, const Line& l) { os << l.s.vy << " " << l.s.vx << " " << l.c; return os; }
};
const Line Xaxis = { { 0, -1 }, 0 };
const Line Yaxis = { { 1, 0 }, 0 };
Line L(const Pos& s, const Pos& e) {
	ld dy, dx, c;
	dy = e.y - s.y;
	dx = s.x - e.x;
	c = dy * s.x + dx * s.y;
	return { { dy, dx } , c };
}
Line rotate(const Line& l, const Pos& p, ld the) {
	Vec s = l.s;
	ld x = -s.vx, y = s.vy;
	ld vx = -(x * cos(the) - y * sin(the));
	ld vy = x * sin(the) + y * cos(the);
	ld c = vy * p.x + vx * p.y;
	return Line(Vec(vy, vx), c);
}
Line rotate90(const Line& l, const Pos& p) {
	Vec s = ~l.s;
	ld c = s.vy * p.x + s.vx * p.y;
	return { s, c };
}
Pos intersection(const Line& l1, const Line& l2) {
	Vec v1 = l1.s, v2 = l2.s;
	ld det = v1 / v2;
	return {
		(l1.c * v2.vx - l2.c * v1.vx) / det,
		(l2.c * v1.vy - l1.c * v2.vy) / det,
	};
}
ld get_theta(const Line& b, const Line& l) {
	ld x = (b * l) / b.mag();//dot
	ld y = (b / l) / b.mag();//cross
	return atan2(y, x);
}
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = cross(d1, d2, d3);
	return zero(ret) ? 0 : ret > 0 ? 1 : -1;
}
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = dot(d1, d3, d2);
	return zero(cross(d1, d2, d3)) && (ret > 0 || zero(ret));
}
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = dot(d1, d3, d2);
	return zero(cross(d1, d2, d3)) && ret > 0;
}
ld projection(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d1) / (d2 - d1).mag(); }
bool collinear(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
	return !ccw(d1, d2, d3) && !ccw(d1, d2, d4);
}
bool inner_check(Pos H[], const int& sz, const Pos& p) {//concave
	int cnt{ 0 };
	for (int i = 0; i < sz; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % sz];
		if (on_seg_strong(cur, nxt, p)) return 1;
		if (zero(cur.y - nxt.y)) continue;
		if (nxt.y < cur.y) std::swap(cur, nxt);
		if (nxt.y - TOL < p.y || cur.y > p.y) continue;
		cnt += ccw(cur, nxt, p) > 0;
	}
	return cnt & 1;
}
int inner_check_bi_search(Pos H[], const int& sz, const Pos& p) {//convex
	if (!sz) return -1;
	if (sz == 1) return p == H[0] ? 0 : -1;
	if (sz == 2) return on_seg_strong(H[0], H[1], p) ? 0 : -1;
	if (cross(H[0], H[1], p) < 0 || cross(H[0], H[sz - 1], p) > 0) return -1;
	if (on_seg_strong(H[0], H[1], p) || on_seg_strong(H[0], H[sz - 1], p)) return 0;
	int s = 0, e = sz - 1, m;
	while (s + 1 < e) {
		m = s + e >> 1;
		if (cross(H[0], H[m], p) > 0) s = m;
		else e = m;
	}
	if (cross(H[s], H[e], p) > 0) return 1;
	else if (on_seg_strong(H[s], H[e], p)) return 0;
	else return -1;
}
int inner_check_bi_search(std::vector<Pos>& H, const Pos& p) {//convex
	int sz = H.size();
	if (!sz) return -1;
	if (sz == 1) return p == H[0] ? 0 : -1;
	if (sz == 2) return on_seg_strong(H[0], H[1], p) ? 0 : -1;
	if (cross(H[0], H[1], p) < 0 || cross(H[0], H[sz - 1], p) > 0) return -1;
	if (on_seg_strong(H[0], H[1], p) || on_seg_strong(H[0], H[sz - 1], p)) return 0;
	int s = 0, e = sz - 1, m;
	while (s + 1 < e) {
		m = s + e >> 1;
		if (cross(H[0], H[m], p) > 0) s = m;
		else e = m;
	}
	if (cross(H[s], H[e], p) > 0) return 1;
	else if (on_seg_strong(H[s], H[e], p)) return 0;
	else return -1;
}
bool intersect(const Pos& s1, const Pos& s2, const Pos& d1, const Pos& d2) {
	bool f1 = ccw(s1, s2, d1) * ccw(s2, s1, d2) > 0;
	bool f2 = ccw(d1, d2, s1) * ccw(d2, d1, s2) > 0;
	return f1 && f2;
	//bool f3 = on_seg_strong(s1, s2, d1) ||
	//	on_seg_strong(s1, s2, d2) ||
	//	on_seg_strong(d1, d2, s1) ||
	//	on_seg_strong(d1, d2, s2);
	//return (f1 && f2) || f3;
}
std::vector<Pos> graham_scan(std::vector<Pos>& C) {
	std::vector<Pos> H;
	if (C.size() < 3) {
		std::sort(C.begin(), C.end());
		return C;
	}
	std::swap(C[0], *min_element(C.begin(), C.end()));
	std::sort(C.begin() + 1, C.end(), [&](const Pos& p, const Pos& q) -> bool {
		int ret = ccw(C[0], p, q);
		if (!ret) return (C[0] - p).Euc() < (C[0] - q).Euc();
		return ret > 0;
		}
	);
	C.erase(unique(C.begin(), C.end()), C.end());
	int sz = C.size();
	for (int i = 0; i < sz; i++) {
		while (H.size() >= 2 && ccw(H[H.size() - 2], H.back(), C[i]) <= 0)
			H.pop_back();
		H.push_back(C[i]);
	}
	return H;
}
std::vector<Pos> monotone_chain(std::vector<Pos>& C, std::vector<Pos>& H) {
	std::sort(C.begin(), C.end());
	if (C.size() <= 2) { for (const Pos& p : C) H.push_back(p); }
	else {
		for (int i = 0; i < C.size(); i++) {
			while (H.size() > 1 && ccw(H[H.size() - 2], H[H.size() - 1], C[i]) <= 0)
				H.pop_back();
			H.push_back(C[i]);
		}
		H.pop_back();
		int s = H.size() + 1;
		for (int i = C.size() - 1; i >= 0; i--) {
			while (H.size() > s && ccw(H[H.size() - 2], H[H.size() - 1], C[i]) <= 0)
				H.pop_back();
			H.push_back(C[i]);
		}
		H.pop_back();
	}
	return H;
}
ld area(std::vector<Pos>& H) {
	Pos P = { 0, 0 };
	ld ret = 0;
	int h = H.size();
	for (int i = 0; i < h; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % h];
		ret += cross(P, cur, nxt);
	}
	return ret / 2;
}
ld ternary_search(const Pos& d1, const Pos& v1, const Pos& d2, const Pos& v2, const ld& len) {
	auto diagonal = [&](const ld& s) -> ld {
		return ((d1 + v1 * s) - (d2 + v2 * s)).mag();
		};
	ld s = 0, e = len, l, r;
	int cnt = 20;
	while (cnt--) {
		l = (s + s + e) / 3;
		r = (s + e + e) / 3;
		if (diagonal(l) < diagonal(r)) e = r;
		else s = l;
	}
	return diagonal((s + e) * .5);
}
std::vector<Pos> C, H;
void init() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(7);
	std::cin >> N;
	C.resize(N);
	for (int i = 0; i < N; i++) std::cin >> C[i];
	H = graham_scan(C);
	return;
}
void hull_round_divide() {
	init();
	ld R = 0;
	int sz = H.size();
	//if (sz == 2) { std::cout << "0\n"; return; }
	for (int i = 0; i < sz; i++) R += (H[i] - H[(i + 1) % sz]).mag();

	Pos S = H[0], E = H[0];
	int s = 0, e = 0;
	ld cur = 0;
	while (1) {
		ld seg = (H[(e + 1) % sz] - H[e]).mag();
		if (cur + seg < R * .5 + TOL) {
			cur += seg;
			e = (e + 1) % sz;
			E = H[e];
		}
		else {
			ld rem = R * .5 - cur;
			E += (H[(e + 1) % sz] - H[e]) * (rem / seg);
			break;
		}
	}

	ld ret = (S - E).mag();
	int cnt = sz << 1;
	while (cnt--) {
		Pos v1 = H[(s + 1) % sz] - S;
		Pos v2 = H[(e + 1) % sz] - E;
		ld l = v1.mag();
		ld r = v2.mag();
		ret = std::min(ret, ternary_search(S, v1 / l, E, v2 / r, std::min(l, r)));
		if (zero(l - r)) {
			s = (s + 1) % sz;
			e = (e + 1) % sz;
			S = H[s], E = H[e];
		}
		else if (l < r) {
			s = (s + 1) % sz;
			S = H[s];
			E += v2 * (l / r);
		}
		else {
			e = (e + 1) % sz;
			E = H[e];
			S += v1 * (r / l);
		}
		if (!e) break;
	}
	std::cout << ret << "\n";
	return;
}
bool cw(const Line& l1, const Line& l2, const Line& target) {
	if (l1.s / l2.s < TOL) return 0;
	Pos p = intersection(l1, l2);
	return target.s.vy * p.x + target.s.vx * p.y > target.c - TOL;
	//return target.above(p) > -TOL;
}
bool half_plane_intersection(std::vector<Line>& HP, std::vector<Pos>& hull) {
	std::deque<Line> dq;
	std::sort(HP.begin(), HP.end());
	for (const Line& l : HP) {
		if (!dq.empty() && zero(dq.back() / l)) continue;
		while (dq.size() >= 2 && cw(dq[dq.size() - 2], dq.back(), l)) dq.pop_back();
		while (dq.size() >= 2 && cw(l, dq.front(), dq[1])) dq.pop_front();
		dq.push_back(l);
	}
	while (dq.size() >= 3 && cw(dq[dq.size() - 2], dq.back(), dq.front())) dq.pop_back();
	while (dq.size() >= 3 && cw(dq.back(), dq.front(), dq[1])) dq.pop_front();
	for (int i = 0; i < dq.size(); i++) {
		Line cur = dq[i], nxt = dq[(i + 1) % dq.size()];
		if (cur / nxt < TOL) {
			hull.clear();
			return 0;
		}
		hull.push_back(intersection(cur, nxt));
	}
	return 1;
}
struct Circle {
	Pos c;
	ld r;
	Circle(Pos C = Pos(0, 0), ld R = 0) : c(C), r(R) {}
	bool operator == (const Circle& C) const { return c == C.c && std::abs(r - C.r) < TOL; }
	bool operator != (const Circle& C) const { return !(*this == C); }
	bool operator > (const Pos& p) const { return r > (c - p).mag(); }
	bool operator >= (const Pos& p) const { return r + TOL > (c - p).mag(); }
	bool operator < (const Pos& p) const { return r < (c - p).mag(); }
	Circle operator + (const Circle& C) const { return { c + C.c, r + C.r }; }
	Circle operator - (const Circle& C) const { return { c - C.c, r - C.r }; }
	ld H(const ld& th) const { return sin(th) * c.x + cos(th) * c.y + r; }// coord trans | check right
	ld A() const { return r * r * PI; }
	friend std::istream& operator >> (std::istream& is, Circle& c) { is >> c.c.x >> c.c.y >> c.r; return is; }
	friend std::ostream& operator << (std::ostream& os, const Circle& c) { os << c.c.x << " " << c.c.y << " " << c.r; return os; }
} INVAL = { { 0, 0 }, -1 };
struct Arc {
	ld lo, hi;// [lo, hi] - radian range of arc
	Circle c; // c.r - radius of arc
};
std::vector<Pos> get_node(const Circle& a, const Circle& b) {
	Pos ca = a.c, cb = b.c;
	Pos vec = cb - ca;
	ld ra = a.r, rb = b.r;
	ld distance = vec.mag();

	if (distance > ra + rb + TOL) return {};
	if (distance < std::abs(ra - rb) - TOL) return {};

	//2nd hyprblc law of cos
	ld X = (ra * ra - rb * rb + distance * distance) / (2 * distance);
	ld Y = sqrt(ra * ra - X * X);
	Pos m = ca + vec * (X / distance);
	if (Y > 0) {
		ld ratio = Y / distance;
		return { m + ~vec * ratio, m - ~vec * ratio };
	}
	else return { m };
}
Circle enclose_circle(const Pos& u, const Pos& v) {
	Pos c = (u + v) * .5;
	return Circle(c, (c - u).mag());
}
Circle enclose_circle(const Pos& u, const Pos& v, const Pos& w) {
	Line l1 = rotate90(L(u, v), (u + v) * .5);
	Line l2 = rotate90(L(v, w), (v + w) * .5);
	if (zero(l1 / l2)) return { { 0, 0 }, -1 };
	Pos c = intersection(l1, l2);
	ld r = (c - u).mag();
	return Circle(c, r);
}
Circle enclose_circle(std::vector<Pos> R) {
	if (R.size() == 0) return Circle(O, -1);
	else if (R.size() == 1) return Circle(R[0], 0);
	else if (R.size() == 2) return enclose_circle(R[0], R[1]);
	else return enclose_circle(R[0], R[1], R[2]);
}
Circle welzl(std::vector<Pos>& P, int i, std::vector<Pos> R) {
	if (i == P.size() || R.size() == 3) return enclose_circle(R);
	else {
		Circle D = welzl(P, i + 1, R);
		if (D < P[i]) {
			R.push_back(P[i]);
			D = welzl(P, i + 1, R);
		}
		return D;
	}
}
Circle welzl(std::vector<Pos> P) {
	P.erase(unique(P.begin(), P.end()), P.end());
	shuffle(P.begin(), P.end(), std::mt19937(0x14004));
	return welzl(P, 0, std::vector<Pos>());
}
//Circle enclose_circle(const Pos& u, const Pos& v, const Pos& w) {
//	Pos B = v - u, C = w - u;
//	Line B_ = Line({ B.x, B.y }, B.Euc() * .5);
//	Line C_ = Line({ C.x, C.y }, C.Euc() * .5);
//	if (zero(B_ / C_)) return { { 0, 0 }, -1 };
//	Pos inx = intersection(B_, C_);
//	return Circle(inx + u, inx.mag());
//}
//bool valid_check(const Circle& c, const std::vector<Pos>& P) {
//	for (const Pos& p : P) if (c < p) return 0;
//	return 1;
//}
//Circle get_min_circle(std::vector<Pos>& P) {
//	int sz = P.size();
//	assert(sz <= 3);
//	if (!sz) return Circle(Pos(0, 0), 0);
//	if (sz == 1) return Circle(P[0], 0);
//	if (sz == 2) return enclose_circle(P[0], P[1]);
//	for (int i = 0; i < 2; i++) {
//		for (int j = i + 1; j < 3; j++) {
//			Circle ec = enclose_circle(P[i], P[j]);
//			if (valid_check(ec, P)) return ec;
//		}
//	}
//	return enclose_circle(P[0], P[1], P[2]);
//}
//Circle welzl(std::vector<Pos>& P, std::vector<Pos> R, int sz) {
//	if (!sz || R.size() == 3) return get_min_circle(R);
//	int idx = rand() % sz;
//	Pos p = P[idx];
//	std::swap(P[idx], P[sz - 1]);
//	Circle mec = welzl(P, R, sz - 1);
//	if (mec >= p) return mec;
//	R.push_back(p);
//	return welzl(P, R, sz - 1);
//}
//Circle welzl(std::vector<Pos>& P) {
//	shuffle(P.begin(), P.end(), std::mt19937(0x14004));
//	return welzl(P, {}, P.size());
//}
Circle minimum_enclose_circle(std::vector<Pos> P) {
	shuffle(P.begin(), P.end(), std::mt19937(0x14004));
	Circle mec = INVAL;
	int sz = P.size();
	for (int i = 0; i < sz; i++) {
		if (mec.r < -1 || mec < P[i]) {
			mec = Circle(P[i], 0);
			for (int j = 0; j <= i; j++) {
				if (mec < P[j]) {
					Circle ans = enclose_circle(P[i], P[j]);
					if (zero(mec.r)) { mec = ans; continue; }
					Circle l = INVAL, r = INVAL;
					//Pos vec = P[j] - P[i];
					for (int k = 0; k <= j; k++) {
						if (ans < P[k]) {
							//ld CCW = vec / (P[k] - P[j]);
							ld CCW = cross(P[i], P[j], P[k]);
							Circle c = enclose_circle(P[i], P[j], P[k]);
							if (c.r < 0) continue;
							//else if (CCW > 0 && (l.r < 0 || (vec / (c.c - P[i])) > (vec / (l.c - P[i])))) l = c;
							//else if (CCW < 0 && (r.r < 0 || (vec / (c.c - P[i])) < (vec / (r.c - P[i])))) r = c;
							else if (CCW > 0 && (l.r < 0 || cross(P[i], P[j], c.c) > cross(P[i], P[j], l.c))) l = c;
							else if (CCW < 0 && (r.r < 0 || cross(P[i], P[j], c.c) < cross(P[i], P[j], r.c))) r = c;
						}
					}
					if (l.r < 0 && r.r < 0) mec = ans;
					else if (l.r < 0) mec = r;
					else if (r.r < 0) mec = l;
					else mec = l.r < r.r ? l : r;
				}
			}
		}
	}
	return mec;
}
std::vector<Arc> merge(const Circle& p, const Circle& q, const ld& cur, const ld& nxt) {// merge 2 disks
	if (cur >= nxt) return std::vector<Arc>();// ignore when next disk is inside the hull

	Circle delta = p - q;// difference between p and q : delta.r > 0 when p.r > q.r
	//if (std::abs(delta.c.x) < TOL && std::abs(delta.c.y) < TOL) {
	if (delta.c == O) {
		if (delta.r >= 0) return { { cur, nxt, p } };// same center :: p.r > q.r  
		else return { { cur, nxt, q } };             // same center :: p.r < q.r
	}

	ld t = -ld(delta.r) / delta.c.mag();// angle between tangent and line segment between centers && size comparison
	if (t <= -1) return { { cur, nxt, p } };      // p > q
	if (t >= 1) return { { cur, nxt, q } };       // p < q

	ld phi = atan2l(delta.c.y, delta.c.x);// angle between x-line and line segment between centers
	ld x1 = asin(std::abs(t)) + (t < 0 ? PI : 0);// normalizated angle between tangent and line segment between centers
	ld x2 = PI - x1;
	x1 = norm(x1 - phi);// absolute angle of tangent 1
	x2 = norm(x2 - phi);// absolute angle of tangent 2
	if (x1 > x2) std::swap(x1, x2);

	std::vector<Arc> h2d;
	if (p.H((x1 + x2) / 2) < q.H((x1 + x2) / 2)) // p is to the right of q
		h2d = { {ld(0), x1, p}, {x1, x2, q}, {x2, 2 * PI, p} };// when the HullDisks starts at p
	else // p is to the left of q
		h2d = { {ld(0), x1, q}, {x1, x2, p}, {x2, 2 * PI, q} };// when the HullDisks starts at q

	std::vector<Arc> hull;
	for (auto [lo, hi, c] : h2d) {// ignore all arc when it out of angle cur - nxt
		lo = std::max(lo, cur);
		hi = std::min(hi, nxt);
		if (lo >= hi) continue;
		hull.push_back({ lo, hi, c });
	}
	return hull;
}
std::vector<Arc> merge(const std::vector<Arc>& H1, const std::vector<Arc>& H2) {// merge hull H1, H2
	int i = 0, j = 0;
	ld cur = 0;// merge continues until "cur" completes one revolution
	std::vector<Arc> tmp;
	while (i < H1.size() && j < H2.size()) {
		if (H1[i].hi < H2[j].hi) {// tangent of H1[i] is superior to H2[j]'s
			std::vector<Arc> h2d = merge(H1[i].c, H2[j].c, cur, H1[i].hi);
			cur = H1[i++].hi;// hand over the initiative to next disk
			for (const Arc& x : h2d) tmp.push_back(x);
		}
		else {
			std::vector<Arc> h2d = merge(H1[i].c, H2[j].c, cur, H2[j].hi);
			cur = H2[j++].hi;
			for (const Arc& x : h2d) tmp.push_back(x);
		}
	}

	std::vector<Arc> hull;
	cur = 0;
	for (int i = 0; i < tmp.size(); i++) {
		if (i + 1 == tmp.size() || tmp[i + 1].c != tmp[i].c) {
			hull.push_back({ cur, tmp[i].hi, tmp[i].c });// complete one revolution
			cur = tmp[i].hi;
		}
	}
	return hull;
}
std::vector<Arc> solve(int l, int r, std::vector<Circle>& C) {// divide and conquer
	// Returns the convex hull of circles in O(N log N)
	// Arcs are in the form (lo, hi, c) where (c.x, c.y) denotes the center, c.r denotes the radius, and [lo, hi] denotes the radian range.
	// Code inspired by https://codeforces.com/gym/104334/submission/204263938
	// refer to koosaga'
	if (l == r) return { {0, 2 * PI, C[l]} };
	int m = l + r >> 1;
	std::vector<Arc> HL = solve(l, m, C);
	std::vector<Arc> HR = solve(m + 1, r, C);
	return merge(HL, HR);
}
std::vector<Arc> HullDisks(std::vector<Circle>& C) {
	for (auto& c : C) std::swap(c.c.x, c.c.y);// swap x - y coord for convenience of angle calculation
	std::vector<Arc> hull = solve(0, C.size() - 1, C);
	for (auto& A : hull) std::swap(A.c.c.x, A.c.c.y);// set back
	return hull;
}
ld get_round(const std::vector<Arc>& H) {// hull = [[lo, hi, c], [lo, hi, c], [lo, hi, c]...]
	ld R = 0;
	for (int i = 0; i < H.size(); i++) {
		Arc p = H[i], q = H[(i + 1) % H.size()];
		Circle d = p.c - q.c;
		R += p.c.r * (p.hi - p.lo);// calculate arc / r * theta
		R += sqrt((d.c.x * d.c.x + d.c.y * d.c.y) - d.r * d.r);// calculate bridge / sqrt(a^2 - c^2) = b
	}
	return R;
}
//ld get_area(std::vector<Arc>& H) {
//	using point = complex<ld>;
//	std::vector<point> v;
//	ld A = 0;
//	for (auto& [lo, hi, c] : H) {
//		A += 1.0 * c.r * c.r * (hi - lo);
//		A += c.r * (c.c.x * (sin(hi) - sin(lo)) - c.c.y * (cos(hi) - cos(lo)));
//		point x{ 1.0 * c.c.x, 1.0 * c.c.y };
//		point r{ 1.0 * c.r, 0.0 };
//		v.push_back(x + r * exp(point(0, lo)));
//		v.push_back(x + r * exp(point(0, hi)));
//	}
//	for (int i = 0; i < v.size(); i += 2) {
//		A += (conj(v[(i + v.size() - 1) % v.size()]) * v[i]).imag();
//	}
//	return A * .5;
//}
//3D============================================================================//
//3D============================================================================//
//3D============================================================================//
struct Pos3D {
	ld x, y, z;
	Pos3D(ld X = 0, ld Y = 0, ld Z = 0) : x(X), y(Y), z(Z) {}
	bool operator == (const Pos3D& p) const { return zero(x - p.x) && zero(y - p.y) && zero(z - p.z); }
	bool operator != (const Pos3D& p) const { return !zero(x - p.x) || !zero(y - p.y) || !zero(z - p.z); }
	bool operator < (const Pos3D& p) const { return zero(x - p.x) ? zero(y - p.y) ? z < p.z : y < p.y : x < p.x; }
	ld operator * (const Pos3D& p) const { return x * p.x + y * p.y + z * p.z; }
	Pos3D operator / (const Pos3D& p) const {
		Pos3D ret;
		ret.x = y * p.z - z * p.y;
		ret.y = z * p.x - x * p.z;
		ret.z = x * p.y - y * p.x;
		return ret;
	}
	Pos3D operator + (const Pos3D& p) const { return { x + p.x, y + p.y, z + p.z }; }
	Pos3D operator - (const Pos3D& p) const { return { x - p.x, y - p.y, z - p.z }; }
	Pos3D operator * (const ld& scalar) const { return { x * scalar, y * scalar, z * scalar }; }
	Pos3D operator / (const ld& scalar) const { return { x / scalar, y / scalar, z / scalar }; }
	Pos3D& operator += (const Pos3D& p) { x += p.x; y += p.y; z += p.z; return *this; }
	Pos3D& operator -= (const Pos3D& p) { x -= p.x; y -= p.y; z -= p.z; return *this; }
	Pos3D& operator *= (const ld& scalar) { x *= scalar; y *= scalar; z *= scalar; return *this; }
	Pos3D& operator /= (const ld& scalar) { x /= scalar; y /= scalar; z /= scalar; return *this; }
	ld Euc() const { return x * x + y * y + z * z; }
	ld mag() const { return sqrtl(Euc()); }
	ld lon() const { return atan2(y, x); }
	ld lat() const { return atan2(z, sqrtl(x * x + y * y)); }
	Pos3D unit() const { return *this / mag(); }
	Pos3D norm(const Pos3D& p) const { return (*this / p).unit(); }
	Pos3D rotate(const ld& th, const Pos3D& axis) const {
		ld SIN = sin(th), COS = cos(th);
		Pos3D u = axis.unit();
		return u * (*this * u) * (1 - COS) + (*this * COS) - (*this / u) * SIN;
	}
	friend std::istream& operator >> (std::istream& is, Pos3D& p) { is >> p.x >> p.y >> p.z; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos3D& p) { os << p.x << " " << p.y << " " << p.z; return os; }
};
const Pos3D O3D = { 0, 0, 0 };
const Pos3D X_axis = { 1, 0, 0 };
const Pos3D Y_axis = { 0, 1, 0 };
const Pos3D Z_axis = { 0, 0, 1 };
const Pos3D MAXP3D = { INF, INF, INF };
std::vector<Pos3D> pos;
Pos3D S2C(const ld& lon, const ld& lat) {//Spherical to Cartesian
	ld phi = lon * PI / 180;
	ld the = lat * PI / 180;
	return Pos3D(cos(phi) * cos(the), sin(phi) * cos(the), sin(the));
}
std::vector<Pos3D> poses, distorted;//3D
std::vector<Pos3D> CANDI;//2D
struct Line3D {
	Pos3D dir, p0;
	Line3D(Pos3D DIR = Pos3D(0, 0, 0), Pos3D P0 = Pos3D(0, 0, 0)) : dir(DIR), p0(P0) {}
};
struct Plane {
	ld a, b, c, d;
	Plane(ld A = 0, ld B = 0, ld C = 0, ld D = 0) : a(A), b(B), c(C), d(D) {}
	Pos3D norm() const { return Pos3D(a, b, c); };
	friend std::istream& operator >> (std::istream& is, Plane& f) { 
		is >> f.a >> f.b >> f.c >> f.d;
		return is;
	}
	friend std::ostream& operator << (std::ostream& os, const Plane& f) {
		os << f.a << " " << f.b << " " << f.c << " " << f.d;
		return os;
	}
} knife;
bool circle_intersection(const Pos3D& a, const Pos3D& b, const ld& th, std::vector<Pos3D>& inxs) {
	inxs.clear();
	Pos3D mid = (a + b) * .5;
	if (zero(mid.mag())) return 0;
	ld x = cos(th) / mid.mag();
	if (x < -1 || 1 < x) return 0;
	Pos3D w = mid.unit() * x;
	ld ratio = sqrtl(1 - x * x);
	Pos3D h = (mid.unit() / (b - a).unit()) * ratio;
	inxs.push_back(w + h);
	inxs.push_back(w - h);
	return 1;
}
bool plane_circle_intersection(const Pos3D& a, const Pos3D& perp, const ld& th, std::vector<Pos3D>& inxs) {
	inxs.clear();
	Pos3D vec = a - (perp * (perp * a));
	if (zero(vec.mag())) return 0;
	ld x = cos(th) / vec.mag();
	if (x < -1 || 1 < x) return 0;
	Pos3D w = vec.unit() * x;
	ld ratio = sqrtl(1 - x * x);
	Pos3D h = (vec.unit() / perp) * ratio;
	inxs.push_back(w + h);
	inxs.push_back(w - h);
	return 1;
}
Pos3D point(const Pos3D Xaxis, const Pos3D Yaxis, const ld& th) { return Xaxis * cos(th) + Yaxis * sin(th); }
ld angle(const Pos3D Xaxis, const Pos3D Yaxis, const Pos3D& p) {
	ld X = Xaxis * p;
	ld Y = Yaxis * p;
	ld th = atan2(Y, X);
	return norm(th);
}
int above(const Plane& S, const Pos3D& p) {
	ld ret = p * S.norm() + S.d;
	return dcmp(ret);
}
ld randTOL() {
	ld rand01 = rand() / (ld)RAND_MAX;
	ld err = (rand01 - .5) * TOL;
	return err;
}
Pos3D add_noise(const Pos3D& p) {//refer to BIGINTEGER
	return p + Pos3D(randTOL(), randTOL(), randTOL());
}
Pos3D cross(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) { return (d2 - d1) / (d3 - d2); }
ld dot(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) { return (d2 - d1) * (d3 - d2); }
int ccw(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3, const Pos3D& norm) {
	Pos3D CCW = cross(d1, d2, d3);
	ld ret = CCW * norm;
	return zero(ret) ? 0 : ret > 0 ? 1 : -1;
}
ld area(const std::vector<Pos3D>& H, const Pos3D& norm) {
	ld ret = 0;
	if (H.size() < 3) return ret;
	Pos3D O = H[0];
	int sz = H.size();
	for (int i = 0; i < sz; i++) {
		Pos3D cur = H[i], nxt = H[(i + 1) % sz];
		ret += cross(O, cur, nxt) * norm / norm.mag();
	}
	return std::abs(ret * .5);
}
bool on_seg_strong(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) {
	ld ret = dot(d1, d3, d2);
	return zero(cross(d1, d2, d3).mag()) && (ret > 0 || zero(ret));
}
bool on_seg_weak(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) {
	ld ret = dot(d1, d3, d2);
	return zero(cross(d1, d2, d3).mag()) && ret > 0;
}
//std::vector<Pos3D> graham_scan(std::vector<Pos3D>& C, const Pos3D& norm) {
ld graham_scan(std::vector<Pos3D>& C, const Pos3D& norm) {
	//if (C.size() < 3) {
	//	std::sort(C.begin(), C.end());
	//	return C;
	// }
	if (C.size() < 3) return 0;
	std::vector<Pos3D> H;
	std::swap(C[0], *min_element(C.begin(), C.end()));
	std::sort(C.begin() + 1, C.end(), [&](const Pos3D& p, const Pos3D& q) -> bool {
		ld ret = ccw(C[0], p, q, norm);
		if (zero(ret)) return (C[0] - p).Euc() < (C[0] - q).Euc();
		return ret > 0;
		}
	);
	C.erase(unique(C.begin(), C.end()), C.end());
	int sz = C.size();
	for (int i = 0; i < sz; i++) {
		while (H.size() >= 2 && ccw(H[H.size() - 2], H.back(), C[i], norm) <= 0)
			H.pop_back();
		H.push_back(C[i]);
	}
	//return H;
	return area(H, norm);
}
Line3D L(const Pos3D& p1, const Pos3D& p2) { return { p2 - p1, p1 }; }
Pos3D intersection(const Plane& S, const Line3D& l) {
	ld det = S.norm() * l.dir;
	if (zero(det)) return { INF, INF, INF };
	ld t = -((S.norm() * l.p0 + S.d) / det);
	return l.p0 + (l.dir * t);
}
Pos3D intersection(const Plane& S, const Pos3D& p1, const Pos3D& p2) {
	Line3D l = L(p1, p2);
	Pos3D inx = intersection(S, l);
	//if (!on_seg_strong(p1, p2, inx)) return { INF, INF, INF };
	return inx;
}
bool inner_check(const Pos3D& d1, const Pos3D& d2, const Pos3D& t) {
	Pos3D nrm = cross(O3D, d1, d2);
	Pos3D p1 = d1, p2 = d2;
	if (ccw(O3D, p1, p2, nrm) < 0) std::swap(p1, p2);
	return ccw(O3D, p1, t, nrm) >= 0 && ccw(O3D, p2, t, nrm) <= 0;
}
int inner_check(std::vector<Pos3D>& H, const Pos3D& p) {//for convex hull
	int sz = H.size();
	if (sz <= 1) return -1;
	if (sz == 2) {
		if (on_seg_strong(H[0], H[1], p)) return 0;
		else return -1;
	}
	Pos3D torque0 = cross(H[0], H[1], p);
	for (int i = 1; i < sz; i++) {
		Pos3D cur = H[i], nxt = H[(i + 1) % sz];
		Pos3D torqueI = cross(cur, nxt, p);
		if (zero(torqueI.mag())) {
			if (on_seg_strong(cur, nxt, p)) return 0;
			else return -1;
		}
		if (torque0 * torqueI < 0) return -1;
	}
	return 1;
}
struct Planar {
	Pos3D norm, p0;
	Planar(Pos3D NORM = Pos3D(0, 0, 0), Pos3D P0 = Pos3D(0, 0, 0)) : norm(NORM), p0(P0) {}
};
Planar P(const Pos3D& p1, const Pos3D& p2, const Pos3D& p3) {
	Pos3D norm = (p2 - p1) / (p3 - p2);
	return Planar(norm, p1);
}
Planar P(std::vector<Pos3D>& tri) {
	Pos3D p1 = tri[0], p2 = tri[1], p3 = tri[2];
	Pos3D norm = (p2 - p1) / (p3 - p2);
	return Planar(norm, p1);
}
Pos3D intersection(const Planar& S, const Line3D& l) {
	ld det = S.norm * l.dir;
	if (zero(det)) return { INF, INF, INF };
	ld t = (S.norm * S.p0 - S.norm * l.p0) / det;
	return l.p0 + (l.dir * t);
}
struct Face {//refer to BIGINTEGER
	int v[3];
	Face(int a, int b, int c) { v[0] = a; v[1] = b; v[2] = c; }
	Pos3D norm(const std::vector<Pos3D>& P) const {
		return cross(P[v[0]], P[v[1]], P[v[2]]);
	}
	bool visible(const std::vector<Pos3D>& P, int i) const {
		return (P[i] - P[v[0]]) * norm(P) > 0;
	}
};
std::vector<Face> H3D;
std::vector<Face> ConvexHull3D(const std::vector<Pos3D>& P) {//refer to BIGINTEGER
	int sz = P.size();
	std::vector<std::vector<int>> vis(sz);
	for (int i = 0; i < sz; i++) vis[i].resize(sz);
	std::vector<Face> cur;
	cur.push_back(Face(0, 1, 2));
	cur.push_back(Face(2, 1, 0));
	for (int i = 3; i < sz; i++) {
		std::vector<Face> next;
		for (int j = 0; j < cur.size(); j++) {
			Face& f = cur[j];
			int ret = f.visible(P, i);
			if (!ret) next.push_back(f);
			for (int k = 0; k < 3; k++) vis[f.v[k]][f.v[(k + 1) % 3]] = ret;
		}
		for (int j = 0; j < cur.size(); j++) {
			for (int k = 0; k < 3; k++) {
				int a = cur[j].v[k], b = cur[j].v[(k + 1) % 3];
				if (vis[a][b] != vis[b][a] && vis[a][b])
					next.push_back(Face(a, b, i));
			}
		}
		cur = next;
	}
	return cur;
}
ld sc[4];
void get_angle(ld sc[], const Pos3D& norm) {
	ld a = norm.x, b = norm.y, c = norm.z;
	ld angle1 = -atan2(b, a);
	ld dx = sqrtl(a * a + b * b);
	ld angle2 = -atan2(dx, c);
	sc[0] = sin(angle1);
	sc[1] = cos(angle1);
	sc[2] = sin(angle2);
	sc[3] = cos(angle2);
	return;
}
Pos3D rotate(ld sc[], const Pos3D& p) {//project to xy_plane
	ld x = p.x * sc[1] - p.y * sc[0], y = p.x * sc[0] + p.y * sc[1], z = p.z;
	return Pos3D(z * sc[2] + x * sc[3], y, z * sc[3] - x * sc[2]);
}
void rotate(ld sc[], const Pos3D& p, std::vector<Pos>& C) {//project to xy_plane
	ld x = p.x * sc[1] - p.y * sc[0], y = p.x * sc[0] + p.y * sc[1], z = p.z;
	Pos3D q = Pos3D(z * sc[2] + x * sc[3], y, z * sc[3] - x * sc[2]);
	C.push_back(Pos(q.x, q.y));
	return;
}
Pos projecting2D(ld sc[], const Pos3D& p) {//project to xy_plane
	ld x = p.x * sc[1] - p.y * sc[0], y = p.x * sc[0] + p.y * sc[1], z = p.z;
	Pos3D q = Pos3D(z * sc[2] + x * sc[3], y, z * sc[3] - x * sc[2]);
	return Pos(q.x, q.y);
}