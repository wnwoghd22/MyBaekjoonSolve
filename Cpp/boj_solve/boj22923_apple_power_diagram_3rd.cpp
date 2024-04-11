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
const ld TOL = 1e-9;
const ld PI = acos(-1);
const int LEN = 3e3 + 5;
int N, M, T, Q;
bool V[LEN];
bool zero(const ld& x) { return std::abs(x) < TOL; }
int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }

///=========================================================///
typedef double db;
const db EPS = 1e-9;
//inline int sign(db a) { return a < -EPS ? -1 : a > EPS; }
inline int cmp(db a, db b) { return sign(a - b); }
//refer to bulijiojiodibuliduo
//O(n^2logN + 6QN) power-diagram
///=========================================================///

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
	//ld mag() const { return hypot(x, y); }
	ld mag() const { return sqrt(Euc()); }
	ld ang() const { return atan2(y, x); }
	Pos unit() const { return *this / mag(); }
	//int quad() const { return y > 0 || (zero(y) && x > -TOL); }
	int quad() const { return sign(y) == 1 || (zero(y) && sign(x) >= 0); }
	Pos rot(ld the) { return { x * cos(the) - y * sin(the),x * sin(the) + y * cos(the) }; }
	friend bool cmpq(const Pos& a, const Pos& b) {
		if (a.quad() != b.quad()) return a.quad() < b.quad();
		else return a / b > 0;
	}
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = { 0, 0 };
typedef std::vector<Pos> Polygon;
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
struct Linear {//ps[0] -> ps[1] :: refer to bulijiojiodibuliduo
	Pos ps[2];
	Pos dir_;
	Pos& operator[](int i) { return ps[i]; }
	Pos dir() const { return dir_; }
	Linear(Pos a = Pos(0, 0), Pos b = Pos(0, 0)) {
		ps[0] = a;
		ps[1] = b;
		dir_ = (ps[1] - ps[0]).unit();
	}
	bool include(const Pos& p) const { return (dir_ / (p - ps[0])) > 0; }
	Linear push() const {//push eps outward
		const double eps = 1e-8;
		Pos delta = ~(ps[1] - ps[0]).unit() * eps;
		return Linear(ps[0] + delta, ps[1] + delta);
	}
	Linear operator + (const double eps) const {//push eps outward
		Pos delta = ~(ps[1] - ps[0]).unit() * eps;
		return Linear(ps[0] + delta, ps[1] + delta);
	}
	Linear operator - (const double eps) const {//pull eps inward
		Pos delta = ~(ps[1] - ps[0]).unit() * eps;
		return Linear(ps[0] - delta, ps[1] - delta);
	}
	bool operator < (const Linear& l0) const {
		if (zero(this->dir() / l0.dir()) && l0.dir() * this->dir() > 0) return l0.include(ps[0]);
		else return cmpq(this->dir(), l0.dir());
	}
	friend bool parallel(Linear l0, Linear l1) { return zero(l0.dir() / l1.dir()); }
	friend bool same_dir(Linear l0, Linear l1) { return parallel(l0, l1) && l0.dir() * l1.dir() > 0; }
};
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = cross(d1, d2, d3);
	return zero(ret) ? 0 : ret > 0 ? 1 : -1;
}
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
ld dist(const Pos& d1, const Pos& d2, const Pos& t) {
	return cross(d1, d2, t) / (d1 - d2).mag();
}
Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) {
	ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2);
	return (p1 * a2 + p2 * a1) / (a1 + a2);
}
Pos intersection(Linear& l1, Linear& l2) { return intersection(l1[0], l1[1], l2[0], l2[1]); }
ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
Polygon convex_cut(const std::vector<Pos>& ps, const Pos& b1, const Pos& b2) {
	std::vector<Pos> qs;
	int n = ps.size();
	for (int i = 0; i < n; i++) {
		Pos p1 = ps[i], p2 = ps[(i + 1) % n];
		int d1 = ccw(b1, b2, p1), d2 = ccw(b1, b2, p2);
		if (d1 >= 0) qs.push_back(p1);
		if (d1 * d2 < 0) qs.push_back(intersection(p1, p2, b1, b2));
	}
	return qs;
}
Polygon sutherland_hodgman(const std::vector<Pos>& C, const std::vector<Pos>& clip) {
	int sz = clip.size();
	std::vector<Pos> ret = C;
	for (int i = 0; i < sz; i++) {
		Pos b1 = clip[i], b2 = clip[(i + 1) % sz];
		ret = convex_cut(ret, b1, b2);
	}
	return ret;
}
std::vector<Pos> circle_line_intersection(const Pos& o, const ld& r, const Pos& p1, const Pos& p2) {
	ld d = dist(p1, p2, o);
	if (std::abs(d) > r) return {};
	Pos vec = p2 - p1;
	Pos m = intersection(p1, p2, o, o + ~vec);
	ld distance = vec.mag();
	ld ratio = sqrt(r * r - d * d);
	Pos m1 = m - vec * ratio / distance;
	Pos m2 = m + vec * ratio / distance;
	if (dot(p1, p2, m1, m2) < 0) std::swap(m1, m2);
	return { m1, m2 };//p1->p2

	////refer to bulijiojiodibuliduo
	//if (std::abs(cross(o, p1, p2) / (p1 - p2).mag()) > r) return {};
	//if (std::abs(dist(p1, p2, o)) > r) return {};
	//ld x = (p1 - o) * (p2 - p1);
	//ld y = (p2 - p1).Euc();
	//ld d = x * x - y * ((p1 - o).Euc() - r * r);
	//d = std::max(d, 0.);
	//Pos m = p1 - (p2 - p1) * (x / y);
	//Pos dr = (p2 - p1) * (sqrt(d) / y);
	//return { m - dr, m + dr };
}
ld area_cut(const ld& r, const Pos& p1,const Pos& p2) {
	std::vector<Pos> is = circle_line_intersection(O, r, p1, p2);
	if (is.empty()) return r * r * rad(p1, p2) * .5;
	bool b1 = p1.Euc() > r * r, b2 = p2.Euc() > r * r;
	if (b1 && b2) {
		if ((p1 - is[0]) * (p2 - is[0]) < TOL) return r * r * (rad(p1, is[0]) + rad(is[1], p2)) / 2 + is[0] / is[1] * .5;
		else return r * r * rad(p1, p2) / 2;
	}
	if (b1) return (r * r * rad(p1, is[0]) + is[0] / p2) * .5;
	if (b2) return (p1 / is[1] + r * r * rad(is[1], p2)) * .5;
	return p1 / p2 * .5;
}
std::vector<Pos> half_plane_intersection(std::vector<Linear>& HP) {//refer to bulijiojiodibuliduo
	auto check = [&](Linear& u, Linear& v, Linear& w) -> bool {
		return w.include(intersection(u, v));
		};
	std::sort(HP.begin(), HP.end());
	std::deque<Linear> dq;
	int sz = HP.size();
	for (int i = 0; i < sz; ++i) {
		if (i && same_dir(HP[i], HP[(i - 1) % sz])) continue;
		while (dq.size() > 1 && !check(dq[dq.size() - 2], dq[dq.size() - 1], HP[i])) dq.pop_back();
		while (dq.size() > 1 && !check(dq[1], dq[0], HP[i])) dq.pop_front();
		dq.push_back(HP[i]);
	}
	while (dq.size() > 2 && !check(dq[dq.size() - 2], dq[dq.size() - 1], dq[0])) dq.pop_back();
	while (dq.size() > 2 && !check(dq[1], dq[0], dq[dq.size() - 1])) dq.pop_front();
	std::vector<Pos> HPI;
	sz = dq.size();
	for (int i = 0; i < sz; ++i) HPI.push_back(intersection(dq[i], dq[(i + 1) % sz]));
	return HPI;
}
struct Circle {
	Pos c;
	int r;
	Circle(Pos C = Pos(0, 0), int R = 0) : c(C), r(R) {}
	bool operator == (const Circle& C) const { return c == C.c && std::abs(r - C.r) < TOL; }
	bool operator != (const Circle& C) const { return !(*this == C); }
	bool operator < (const Circle& q) const {
		ld dist = (c - q.c).mag();
		return r <= q.r && dist + r < q.r + TOL;
		//return r <= q.r && dist + r <= q.r;
	}
	bool operator > (const Pos& p) const { return r > (c - p).mag(); }
	bool operator >= (const Pos& p) const { return r + TOL > (c - p).mag(); }
	bool operator < (const Pos& p) const { return r < (c - p).mag(); }
	Circle operator + (const Circle& C) const { return { c + C.c, r + C.r }; }
	Circle operator - (const Circle& C) const { return { c - C.c, r - C.r }; }
	ld H(const ld& th) const { return sin(th) * c.x + cos(th) * c.y + r; }//coord trans | check right
	ld A() const { return 1. * r * r * PI; }
	friend std::istream& operator >> (std::istream& is, Circle& c) { is >> c.c >> c.r; return is; }
	friend std::ostream& operator << (std::ostream& os, const Circle& c) { os << c.c << " " << c.r; return os; }
} INVAL = { { 0, 0 }, -1 };
bool cmpr(const Circle& p, const Circle& q) { return p.r > q.r; }//sort descending order
std::vector<Pos> pd[LEN];//power diagram (Laguerre-Voronoi diagram)
std::vector<Circle> disks;
void query() {
	ll x, y, w, h;
	std::vector<Pos> box;
	std::cin >> x >> y >> w >> h;
	box = { Pos(x, y), Pos(x + w, y), Pos(x + w, y + h), Pos(x, y + h) };
	ld ret = 0;
	for (int i = 0; i < N; i++) {
		std::vector<Pos> rem = sutherland_hodgman(pd[i], box);
		int sz = rem.size();
		if (sz < 3) continue;
		for (int j = 0; j < sz; j++)
			ret += area_cut(disks[i].r, rem[j] - disks[i].c, rem[(j + 1) % sz] - disks[i].c);
	}
	ret = ret * 100 / w / h;
	ret = std::min(std::max(ret, 0.), 100.);
	std::cout << ret << "\n";
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(15);
	std::cin >> N >> Q;
	std::vector<Circle> tmp(N);
	for (Circle& c : tmp) std::cin >> c;
	std::sort(tmp.begin(), tmp.end(), cmpr);//sort descending order - by r
	memset(V, 0, sizeof V);
	for (int i = 0; i < N; i++) {//remove
		if (V[i]) continue;
		for (int j = i + 1; j < N; j++) {
			if (tmp[j] == tmp[i]) V[j] = 1;
			if (tmp[j] < tmp[i]) V[j] = 1;
		}
	}

	//memset(V, 0, sizeof V);//refer to bulijiojiodibuliduo
	//for (int i = 0; i < N; i++) {
	//	for (int j = 0; j < N; j++) {
	//		if (i != j && std::make_pair(tmp[i].r, i) <= std::make_pair(tmp[j].r, j))
	//			if ((tmp[i].c - tmp[j].c).mag() - TOL < (tmp[j].r - tmp[i].r))
	//				V[i] = 1;
	//	}
	//}

	for (int i = 0; i < N; i++) if (!V[i]) disks.push_back(tmp[i]);
	N = disks.size();

	int bnd = 3e6;
	for (int i = 0; i < N; i++) {//compose power diagram
		std::vector<Linear> HP;
		HP.push_back(Linear(Pos(bnd, bnd), Pos(-bnd, bnd)));
		HP.push_back(Linear(Pos(-bnd, bnd), Pos(-bnd, -bnd)));
		HP.push_back(Linear(Pos(-bnd, -bnd), Pos(bnd, -bnd)));
		HP.push_back(Linear(Pos(bnd, -bnd), Pos(bnd, bnd)));
		for (int j = 0; j < N; j++) {
			if (i == j) continue;
			Pos& ca = disks[i].c, cb = disks[j].c;
			ld ra = disks[i].r, rb = disks[j].r;
			Pos vec = cb - ca;//vec a -> b
			ld distance = vec.mag();
			ld X = (ra * ra - rb * rb + vec.Euc()) / (2 * distance);
			Pos m = ca + vec * X / distance;
			HP.push_back(Linear(m, m + ~vec));
		}
		pd[i] = half_plane_intersection(HP);
	}

	//int bnd = 3e6;//refer to bulijiojiodibuliduo
	//for (int i = 0; i < N; i++) {//compose power diagram
	//	std::vector<Linear> HP;
	//	HP.push_back(Linear(Pos(bnd, bnd), Pos(-bnd, bnd)));
	//	HP.push_back(Linear(Pos(-bnd, bnd), Pos(-bnd, -bnd)));
	//	HP.push_back(Linear(Pos(-bnd, -bnd), Pos(bnd, -bnd)));
	//	HP.push_back(Linear(Pos(bnd, -bnd), Pos(bnd, bnd)));
	//	for (int j = 0; j < N; j++) {
	//		if (i == j) continue;
	//		Pos& ca = disks[i].c, cb = disks[j].c;
	//		ld ra = disks[i].r, rb = disks[j].r;
	//		Pos d = (cb - ca) * 2;
	//		ld v = (cb * cb - rb * rb) - (ca * ca - ra * ra);
	//		Pos p1(0, 0);
	//		if (fabs(d.x) > fabs(d.y)) p1 = Pos(v / d.x, 0);
	//		else p1 = Pos(0, v / d.y);
	//		Pos p2 = p1 + ~d;
	//		HP.push_back(Linear(p1, p2));
	//	}
	//	pd[i] = half_plane_intersection(HP);
	//}

	//for (int i = 0; i < N; i++) {
	//	std::cout << "DEBUG:: pd[" << i << "] : len:: " << pd[i].size() << " ::\n";
	//	for (Pos& p : pd[i]) std::cout << p << " ";
	//	std::cout << "\n";
	//}
	while (Q--) query();
	return;
}
int main() { solve(); return 0; }//NAC 2021 B Apple Orchard
//refer to bulijiojiodibuliduo

/*

4 3
-1 -1 3
1 -1 3
-1 1 3
1 1 3
4
-3e+06 0 -3e+06 -3e+06 0 -3e+06 0 0
4
0 0 0 -3e+06 3e+06 -3e+06 3e+06 0
4
-3e+06 3e+06 -3e+06 0 0 0 0 3e+06
4
0 3e+06 0 0 3e+06 0 3e+06 3e+06
-4 -4 8 8
0
-1 -4 2 8
0
-3 -1 12 3
0

4 3
-1 -1 3
1 -1 3
-1 1 3
1 1 3
4
-3e+06 0 -3e+06 -3e+06 0 -3e+06 0 0
4
0 0 0 -3e+06 3e+06 -3e+06 3e+06 0
4
-3e+06 3e+06 -3e+06 0 0 -0 0 3e+06
4
0 3e+06 -0 0 3e+06 0 3e+06 3e+06
-4 -4 8 8
87.222142377564509
-1 -4 2 8
98.586991372916088
-3 -1 12 3
57.862330457638706

*/

//Pos p[LEN];
//int r[LEN];
//void solve_bulijiojiodibuliduo() {
//	scanf("%d%d", &N, &Q);
//	for (int i = 0; i < N; i++) {
//		int x, y;
//		scanf("%d%d%d", &x, &y, &r[i]);
//		p[i] = Pos(x, y);
//	}
//	for (int i = 0; i < N; i++) {
//		for (int j = 0; j < N; j++) {
//			if (i != j && std::make_pair(r[i], i) <= std::make_pair(r[j], j))
//				if ((p[i] - p[j]).mag() - TOL < (r[j] - r[i]))
//					V[i] = 1;
//		}
//	}
//	int n = 0;
//	for (int i = 0; i < N; i++) if (!V[i]) {
//		p[n] = p[i];
//		r[n] = r[i];
//		++n;
//	}
//	N = n;
//	int bnd = 3e6;
//	ld ss = 0;
//	for (int i = 0; i < N; i++) {
//		std::vector<Linear> l;
//		l.push_back(Linear(Pos(bnd, bnd), Pos(-bnd, bnd)));
//		l.push_back(Linear(Pos(-bnd, bnd), Pos(-bnd, -bnd)));
//		l.push_back(Linear(Pos(-bnd, -bnd), Pos(bnd, -bnd)));
//		l.push_back(Linear(Pos(bnd, -bnd), Pos(bnd, bnd)));
//
//		for (int j = 0; j < N; j++)
//			if (i != j) {
//				Pos d = (p[j] - p[i]) * 2;
//				ld v = (p[j] * p[j] - 1. * r[j] * r[j]) - (p[i] * p[i] - 1. * r[i] * r[i]);
//				Pos p1(0, 0);
//				if (fabs(d.x) > fabs(d.y)) p1 = Pos(v / d.x, 0);
//				else p1 = Pos(0, v / d.y);
//				Pos p2 = p1 + ~d;
//				l.push_back(Linear(p1, p2));
//			}
//		pd[i] = half_plane_intersection(l);
//		//std::cout << pd[i].size() << "\n";
//		//for (Pos& p : pd[i]) std::cout << p << " ";
//		//std::cout << "\n";
//	}
//	while (Q--) {
//		int x1, y1, w, h, x2, y2;
//		scanf("%d%d%d%d", &x1, &y1, &w, &h);
//		//x2,&y2);
//		x2 = x1 + w; y2 = y1 + h;
//		ld ret = 0;
//		for (int i = 0; i < N; i++) {
//			std::vector<Pos> box = { Pos(x1, y1),Pos(x2, y1), Pos(x2, y2), Pos(x1, y2) };
//			std::vector<Pos> rev = sutherland_hodgman(pd[i], box);
//			//std::vector<Pos> rev = pd[i];
//			//rev = convex_cut(rev, Pos(x1, y1), Pos(x2, y1));
//			//rev = convex_cut(rev, Pos(x2, y1), Pos(x2, y2));
//			//rev = convex_cut(rev, Pos(x2, y2), Pos(x1, y2));
//			//rev = convex_cut(rev, Pos(x1, y2), Pos(x1, y1));
//			//printf("cnm %d\n",i);
//			int m = rev.size();
//			if (m < 3) continue;
//			for (int j = 0; j < m; j++) ret += area_cut(r[i], rev[j] - p[i], rev[(j + 1) % m] - p[i]);
//		}
//		ret = ret * 100 / w / h;
//		ret = std::min(std::max(ret, 0.), 100.);
//		printf("%.15f\n", ret);
//	}
//	return;
//}
//int main() { solve_bulijiojiodibuliduo(); return 0; }
//refer to bulijiojiodibuliduo
