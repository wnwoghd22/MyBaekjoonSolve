#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
typedef long long ll;
typedef long double ld;
const ld TOL = 1e-6;
const ld INF = 1e7;
const ld LIMIT = 3e4;
int N, M;

bool z(const ld& x) { return std::abs(x) < TOL; }
struct Pos {
	ld x, y;
	Pos(ld X, ld Y) : x(X), y(Y) {}
	Pos() : x(0), y(0) {}
	//bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator == (const Pos& p) const { return z(x - p.x) && z(y - p.y); }
	//bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	bool operator < (const Pos& p) const { return z(x - p.x) ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ld& n) const { return { x * n, y * n }; }
	Pos operator / (const ld& n) const { return { x / n, y / n }; }
	ld operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
	ld operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
	Pos operator ~ () const { return { -y, x }; }
	ld operator ! () const { return x * y; }
	Pos& operator *= (const ld& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const ld& scale) { x /= scale; y /= scale; return *this; }
	ld mag() const { return hypot(x, y); }
} velA, velB, pa, pb; const Pos O = { 0, 0 };
std::vector<Pos> HA, HB;
struct Vec {
	ld vy, vx;
	bool operator < (const Vec& v) const { return z(vy - v.vy) ? vx < v.vx : vy < v.vy; }
	bool operator == (const Vec& v) const { return (z(vy - v.vy) && z(vx - v.vx)); }
	ld operator / (const Vec& v) const { return vy * v.vx - vx * v.vy; }
}; const Vec Zero = { 0, 0 };
struct Line {
	Vec s;
	ld c;
	bool operator < (const Line& l) const {
		bool f1 = Zero < s;
		bool f2 = Zero < l.s;
		if (f1 != f2) return f1;
		ld ccw = s / l.s;
		return z(ccw) ? c * hypot(l.s.vy, l.s.vx) < l.c * hypot(s.vy, s.vx) : ccw > 0;
	}
	ld operator / (const Line& l) const { return s / l.s; }
};
Line L(const Pos& s, const Pos& e) {
	ld dy, dx, c;
	dy = e.y - s.y;
	dx = s.x - e.x;
	c = dy * s.x + dx * s.y;
	return { {dy, dx} , c };
}
Pos intersection(const Line& l1, const Line& l2) {
	Vec v1 = l1.s, v2 = l2.s;
	ld det = v1 / v2;
	return {
		(l1.c * v2.vx - l2.c * v1.vx) / det,
		(l2.c * v1.vy - l1.c * v2.vy) / det
	};
}
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ld cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = cross(d1, d2, d3);
	return z(ret) ? 0 : ret > 0 ? 1 : -1;
}
int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
	ld ret = cross(d1, d2, d3, d4);
	return z(ret) ? 0 : ret > 0 ? 1 : -1;
}
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
bool on_seg(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld dot_ = dot(d1, d3, d2);
	return z(cross(d1, d2, d3)) && (dot_ > 0 || z(dot_));
}
ld dist(const Pos& d1, const Pos& d2, const Pos& target) {
	if (dot(d1, d2, target) < 0 && dot(d2, d1, target) < 0) return (cross(d1, d2, target) / (d1 - d2).mag());
	else return (std::min((d1 - target).mag(), (d2 - target).mag()));
}
bool cross_check(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
	bool f1 = ccw(d1, d2, d3) * ccw(d2, d1, d4) > 0;
	bool f2 = ccw(d3, d4, d1) * ccw(d4, d3, d2) > 0;
	bool f3 = on_seg(d1, d2, d3) || on_seg(d1, d2, d4) || on_seg(d3, d4, d1) || on_seg(d3, d4, d2);
	return (f1 && f2) || f3;
}
std::vector<Pos> moved_hull(const std::vector<Pos>& H, const Pos& dir, const ld& t) {
	std::vector<Pos> nxt;
	Pos disp = dir * t;
	for (const Pos& h : H) nxt.push_back(h + disp);
	return nxt;
}
int inner_check_bi_search(const std::vector<Pos>& H, const Pos& p) {
	int sz = H.size();
	if (sz < 3 || cross(H[0], H[1], p) < 0 || cross(H[0], H[sz - 1], p) > 0) return 0;
	if (on_seg(H[0], H[1], p) || on_seg(H[0], H[sz - 1], p)) return 1;
	int s = 0, e = sz - 1, m;
	while (s + 1 < e) {
		m = s + e >> 1;
		if (cross(H[0], H[m], p) > 0) s = m;
		else e = m;
	}
	if (cross(H[s], H[e], p) > 0 || on_seg(H[s], H[e], p)) return 1;
	else 0;
}
bool cross_check(const std::vector<Pos>& H1, const std::vector<Pos>& H2, const Pos& vel2) {
	for (const Pos& h : H2) {
		Pos nxt = h + (vel2 * 2e4), pr = H1[0], pl = H1[0];
		for (int i = 1; i < H1.size(); i++) {
			if (ccw(h, pr, H1[i]) < 0) pr = H1[i];
			if (ccw(h, pl, H1[i]) > 0) pl = H1[i];
		}
		if (cross_check(h, nxt, pl, pr)) return 1;
	}
	return 0;
}
ld cal_time(const std::vector<Pos>& H1, const std::vector<Pos>& H2, Pos& vel2, bool f = 0) {
	ld d = !f ? INF : -INF;
	int sz1 = H1.size(), sz2 = H2.size();
	vel2 *= -1;
	for (int i = 0; i < sz1; i++) {
		Pos p1 = H1[i];
		if (!f && inner_check_bi_search(H2, p1)) return 0;
		for (int j = 0; j < sz2; j++) {
			Pos cur = H2[j], nxt = H2[(j + 1) % sz2];
			if (!ccw(cur, nxt, p1)) {
				if (!f) d = std::min(d, dist(cur, nxt, p1));
				if (f) d = std::max(d, dist(cur, nxt, p1));
			}
			else {
				Pos inx = intersection(L(cur, nxt), L(p1, p1 + vel2));
				if (on_seg(cur, nxt, inx) && !(dot(p1 + vel2, p1, inx) > 0)) {
					if (!f) d = std::min(d, (p1 - inx).mag());
					if (f) d = std::max(d, (p1 - inx).mag());
				}
			}
		}
	}
	vel2 *= -1;
	for (int j = 0; j < sz2; j++) {
		Pos p2 = H2[j];
		if (!f && inner_check_bi_search(H1, p2)) return 0;
		for (int i = 0; i < sz1; i++) {
			Pos cur = H1[i], nxt = H1[(i + 1) % sz1];
			if (!ccw(cur, nxt, p2)) {
				if (!f) d = std::min(d, dist(cur, nxt, p2));
				if (f) d = std::max(d, dist(cur, nxt, p2));
			}
			else {
				Pos inx = intersection(L(cur, nxt), L(p2, p2 + vel2));
				if (on_seg(cur, nxt, inx) && !(dot(p2 + vel2, p2, inx) > 0)) {
					if (!f) d = std::min(d, (p2 - inx).mag());
					if (f) d = std::max(d, (p2 - inx).mag());
				}
			}
		}
	}
	return d / vel2.mag();
}
std::vector<Pos> monotone_chain(std::vector<Pos>& C) {
	std::vector<Pos> H;
	std::sort(C.begin(), C.end());
	if (C.size() <= 2) {
		for (const Pos& p : C) H.push_back(p);
		return H;
	}
	for (int i = 0; i < C.size(); i++) {
		while (H.size() > 1 && (cross(H[H.size() - 2], H[H.size() - 1], C[i]) < 0 || z(cross(H[H.size() - 2], H[H.size() - 1], C[i])))) {
			H.pop_back();
		}
		H.push_back(C[i]);
	}
	H.pop_back();
	int s = H.size() + 1;
	for (int i = C.size() - 1; i >= 0; i--) {
		while (H.size() > s && (cross(H[H.size() - 2], H[H.size() - 1], C[i]) < 0 || z(cross(H[H.size() - 2], H[H.size() - 1], C[i])))) {
			H.pop_back();
		}
		H.push_back(C[i]);
	}
	H.pop_back();
	return H;
}
ld overlapped_area(const std::vector<Pos>& H1, const std::vector<Pos>& H2, const Pos& vel2, const ld& t, bool f = 0) {
	std::vector<Pos> B;
	std::vector<Pos> MH2 = moved_hull(H2, vel2, t);
	int sz1 = H1.size(), sz2 = MH2.size();
	for (int i = 0; i < sz1; i++) {
		Pos h1 = H1[i], h2 = H1[(i + 1) % sz1];
		if (inner_check_bi_search(MH2, h1)) B.push_back(h1);
		for (int j = 0; j < sz2; j++) {
			Pos cur = MH2[j], nxt = MH2[(j + 1) % sz2];
			if (cross_check(h1, h2, cur, nxt)) {
				if (on_seg(cur, nxt, h1)) B.push_back(h1);
				if (on_seg(cur, nxt, h2)) B.push_back(h2);
				if (ccw(h1, h2, cur, nxt)) B.push_back(intersection(L(h1, h2), L(cur, nxt)));
			}
		}
	}
	for (int j = 0; j < sz2; j++) {
		Pos h1 = MH2[j], h2 = MH2[(j + 1) % sz2];
		if (inner_check_bi_search(H1, h1)) B.push_back(h1);
		for (int i = 0; i < sz1; i++) {
			Pos cur = H1[i], nxt = H1[(i + 1) % sz1];
			if (cross_check(h1, h2, cur, nxt)) {
				if (on_seg(cur, nxt, h1)) B.push_back(h1);
				if (on_seg(cur, nxt, h2)) B.push_back(h2);
				if (ccw(h1, h2, cur, nxt)) B.push_back(intersection(L(h1, h2), L(cur, nxt)));
			}
		}
	}
	std::vector<Pos> ol = monotone_chain(B);
	ld area{ 0 };
	int sz = ol.size();
	for (int i = 0; i < sz; i++) {
		Pos cur = ol[i], nxt = ol[(i + 1) % sz];
		area += cross(O, cur, nxt);
	}
	return std::abs(area);
}
ld ternary_search(const ld& t1, const ld& t2) {
	ld s = t1, e = t2, l, r, AL, AR;
	int cnt = 50;
	while (cnt--) {
		l = (s + s + e) / 3;
		r = (s + e + e) / 3;
		AL = overlapped_area(HA, HB, velB, l);
		AR = overlapped_area(HA, HB, velB, r);
		if (z(AL) && z(AR)) e = r;
		else if (AL > AR) e = r;
		else s = l;
	}
	return s;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(6);

	std::cin >> N;
	HA.resize(N);
	for (int i = 0; i < N; i++) std::cin >> HA[i].x >> HA[i].y;
	std::cin >> velA.x >> velA.y;

	std::cin >> M;
	HB.resize(M);
	for (int j = 0; j < M; j++) std::cin >> HB[j].x >> HB[j].y;
	std::cin >> velB.x >> velB.y;
	
	Pos rel = velB - velA;
	if (!cross_check(HA, HB, rel)) std::cout << "never\n";
	else {
		ld low_vel = std::min(std::abs(rel.x), std::abs(rel.y));
		ld tmin = cal_time(HA, HB, rel);
		ld tmax = cal_time(HA, HB, rel, 1);
		ld ans = ternary_search(tmin, tmax);
		std::cout << ans << "\n";
	}
	return;
}
int main() { solve(); return 0; }//boj10785