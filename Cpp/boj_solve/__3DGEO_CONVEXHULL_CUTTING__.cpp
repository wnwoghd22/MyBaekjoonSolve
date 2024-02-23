#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cstring>
#include <cassert>
#include <random>
#include <array>
#include <tuple>
typedef long long ll;
//typedef long double ld;
typedef double ld;
typedef std::pair<int, int> pi;
const ld INF = 1e17;
const ld TOL = 1e-7;
const int LEN = 1e3;
int N, M, T, Q;
ld sc[4];

//struct
struct Seq {
	int x, y;
	Seq(int X = 0, int Y = 0) : x(X), y(Y) {}
};
std::vector<Seq> seq;
bool zero(const ld& x) { return std::abs(x) < TOL; }
int dcmp(const ld& x) { return std::abs(x) < TOL ? 0 : x > 0 ? 1 : -1; }
struct Pos {
	ld x, y;
	Pos(ld X = 0, ld Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ld& n) const { return { x * n, y * n }; }
	Pos operator / (const ld& n) const { return { x / n, y / n }; }
	ld operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
	ld operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
	Pos operator ~ () const { return { -y, x }; }
	ld operator ! () const { return x * y; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator *= (const ld& scale) { x *= scale; y *= scale; return *this; }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return hypot(x, y); }
};
std::vector<Pos> H2D, C;
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = cross(d1, d2, d3);
	return zero(ret) ? 0 : ret > 0 ? 1 : -1;
}
ld area(const std::vector<Pos>& H) {
	Pos P = { 0, 0 };
	ld ret = 0;
	int h = H.size();
	for (int i = 0; i < h; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % h];
		ret += cross(P, cur, nxt);
	}
	return ret / 2;
}
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
	//Pos3D& operator += (const Pos3D& p) { x + p.x; y + p.y; z + p.z; return *this; }
	Pos3D operator - (const Pos3D& p) const { return { x - p.x, y - p.y, z - p.z }; }
	Pos3D operator * (const ld& scalar) const { return { x * scalar, y * scalar, z * scalar }; }
	//Pos3D& operator *= (const ld& scalar) { x * scalar; y * scalar; z * scalar; return *this; }
	ld Euc() const { return x * x + y * y + z * z; }
	ld mag() const { return sqrtl(Euc()); }
	friend std::istream& operator >> (std::istream& is, Pos3D& p);
	friend std::ostream& operator << (std::ostream& os, const Pos3D& p);
}; const Pos3D MAXP = { INF, INF, INF };
std::vector<Pos3D> poses, distorted;//3D
std::vector<Pos3D> CANDI, HPI;//2D
std::istream& operator >> (std::istream& is, Pos3D& p) { is >> p.x >> p.y >> p.z; return is; }
std::ostream& operator << (std::ostream& os, const Pos3D& p) {
	os << p.x << " " << p.y << " " << p.z << "\n";
	return os;
}
struct Line3D {
	Pos3D dir, p0;
	Line3D(Pos3D DIR = Pos3D(0, 0, 0), Pos3D P0 = Pos3D(0, 0, 0)) : dir(DIR), p0(P0) {}
};
//struct Plane {
//	Pos3D norm, p0;
//	Plane(Pos3D NORM = Pos3D(0, 0, 0), Pos3D P0 = Pos3D(0, 0, 0)) : norm(NORM), p0(P0) {}
//};
struct Plane {
	ld a, b, c, d;
	Plane(ld A = 0, ld B = 0, ld C = 0, ld D = 0) : a(A), b(B), c(C), d(D) {}
	Pos3D norm() const { return Pos3D(a, b, c); };
	friend std::istream& operator >> (std::istream& is, Plane& f);
	friend std::ostream& operator << (std::ostream& os, const Plane& f);
} knife;
std::istream& operator >> (std::istream& is, Plane& f) { is >> f.a >> f.b >> f.c >> f.d; return is; }
std::ostream& operator << (std::ostream& os, const Plane& f) {
	os << f.a << " " << f.b << " " << f.c << " " << f.d << "\n";
	return os;
}
using Mesh = std::array<int, 3>; std::vector<Mesh> Hull3D;
struct Edge {
	int mesh_num, edge_num;
	Edge(int t = 0, int v = 0) : mesh_num(t), edge_num(v) {}
};
//fn
int above(const Plane& S, const Pos3D& p) {
	ld ret = p * S.norm() + S.d;
	return dcmp(ret);
}
ld randTOL() {
	ld rand01 = rand() / (ld)RAND_MAX;
	ld err = (rand01 - .5) * TOL;
	return err;
}
Pos3D add_noise(const Pos3D& p) {
	ld rand01 = rand() / (ld)RAND_MAX;
	ld err = (rand01 - .5) * TOL;
	return p + Pos3D(randTOL(), randTOL(), randTOL());
}
Pos3D rotate(Pos3D& p) {
	ld x = p.x * sc[1] - p.y * sc[0], y = p.x * sc[0] + p.y * sc[1], z = p.z;
	return Pos3D(z * sc[2] + x * sc[3], y, z * sc[3] - x * sc[2]);
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
//	if (C.size() <= 1) return {};
//	std::vector<Pos3D> H;
//	std::swap(C[0], *min_element(C.begin(), C.end()));
//	std::sort(C.begin() + 1, C.end(), [&](const Pos3D& p, const Pos3D& q) -> bool {
//		ld ret = ccw(C[0], p, q, norm);
//		//if (!zero(ret)) return ret > 0;
//		//return (C[0] - p).Euc() < (C[0] - q).Euc();
//		if (zero(ret)) return (C[0] - p).Euc() < (C[0] - q).Euc();
//		return ret > 0;
//		}
//	);
//	C.erase(unique(C.begin(), C.end()), C.end());
//	int sz = C.size();
//	for (int i = 0; i < sz; i++) {
//		while (H.size() >= 2 && ccw(H[H.size() - 2], H.back(), C[i], norm) <= 0) H.pop_back();
//		H.push_back(C[i]);
//	}
//	//std::cout << "DEBUG candidate::\n";
//	//for (const Pos3D& c : C)  std::cout << c;
//	//std::cout << "DEBUG candidate::\n";
//	return H;
//}
ld graham_scan(std::vector<Pos3D>& C, const Pos3D& norm) {
	if (C.size() <= 2) return 0;
	std::vector<Pos3D> H;
	std::swap(C[0], *min_element(C.begin(), C.end()));
	std::sort(C.begin() + 1, C.end(), [&](const Pos3D& p, const Pos3D& q) -> bool {
		ld ret = ccw(C[0], p, q, norm);
		//if (!zero(ret)) return ret > 0;
		//return (C[0] - p).Euc() < (C[0] - q).Euc();
		if (zero(ret)) return (C[0] - p).Euc() < (C[0] - q).Euc();
		return ret > 0;
		}
	);
	C.erase(unique(C.begin(), C.end()), C.end());
	int sz = C.size();
	for (int i = 0; i < sz; i++) {
		while (H.size() >= 2 && ccw(H[H.size() - 2], H.back(), C[i], norm) <= 0) H.pop_back();
		H.push_back(C[i]);
	}
	//std::cout << "DEBUG candidate::\n";
	//for (const Pos3D& c : C)  std::cout << c;
	//std::cout << "DEBUG candidate::\n";
	return area(H, norm);
}
Line3D L(const Pos3D& p1, const Pos3D& p2) { return { p2 - p1, p1 }; }
Pos3D intersection(const Plane& S, const Line3D& l) {
	ld det = S.norm() * l.dir;
	if (zero(det)) return { INF, INF, INF };
	ld t = -((S.norm() * l.p0 + S.d) / det);
	////DEBUG
	//std::cout << "norm: " << S.norm() << "det: " << det << " t: " << t << "\n";
	//std::cout << "p0: " << l.p0;
	//std::cout << "dir: " << l.dir;
	////DEBUG
	return l.p0 + (l.dir * t);
}
Pos3D intersection(const Plane& S, const Pos3D& p1, const Pos3D& p2) {
	Line3D l = L(p1, p2);
	//std::cout << "DEBUG intersection::\n";
	//std::cout << "p1: " << p1 << " p2: " << p2;
	Pos3D inx = intersection(S, l);
	//std::cout << "inx: " << inx;
	//std::cout << "DEBUG intersection::\n";
	//if (inx == MAXP) return { INF, INF, INF };
	//if (on_seg_strong(p1, p2, inx)) return inx;
	//else return { INF, INF, INF };
	return inx;
}
struct Face {
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
		for (int j = 0; j < cur.size(); j++)
			for (int k = 0; k < 3; k++) {
				int a = cur[j].v[k], b = cur[j].v[(k + 1) % 3];
				if (vis[a][b] != vis[b][a] && vis[a][b])
					next.push_back(Face(a, b, i));
			}
		cur = next;
	}
	return cur;
}
bool V[LEN + 1][LEN + 1];
std::vector<Pos> H;
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(3);
	std::cin >> T;
	for (int t = 1; t <= T; t++) {
		memset(V, 0, sizeof V);
		distorted.clear();
		seq.clear();
		std::cin >> N >> Q;
		poses.resize(N);
		for (int i = 0; i < N; i++) {
			std::cin >> poses[i];
			distorted.push_back(add_noise(poses[i]));
		}

		H3D = ConvexHull3D(distorted);
		//H3D = ConvexHull3D(poses);

		for (int i = 0; i < H3D.size(); i++) {
			int* v = H3D[i].v;
			std::sort(v, v + 3);
			for (int j = 0; j < 3; j++) {
				for (int k = 0; k < j; k++) {
					if (!V[v[j]][v[k]]) {
						V[v[j]][v[k]] = 1;
						seq.push_back(Seq(v[j], v[k]));
					}
				}
			}
		}
		std::cout << "Case #" << t << ":\n";
		while (Q--) {
			std::cin >> knife;
			ld angle1 = -atan2(knife.b, knife.a);
			ld dx = sqrtl(knife.a * knife.a + knife.b * knife.b);
			ld angle2 = -atan2(dx, knife.c);
			sc[0] = sin(angle1);
			sc[1] = cos(angle1);
			sc[2] = sin(angle2);
			sc[3] = cos(angle2);
			//C.clear(); H.clear();
			CANDI.clear(); HPI.clear();
			for (const Seq& p : seq) {
				Pos3D cur = poses[p.x], nxt = poses[p.y];
				int abv1 = above(knife, cur), abv2 = above(knife, nxt);
				if (abv1 != abv2) {
					Pos3D inx = intersection(knife, cur, nxt);
					//if (inx == MAXP) continue;
					//inx = rotate(inx);
					//C.push_back(Pos(inx.x, inx.y));
					CANDI.push_back(inx);
				}
			}
			std::cout << graham_scan(CANDI, knife.norm()) << "\n";
			//std::sort(C.begin(), C.end());
			//if (C.size() <= 2) { for (const Pos& p : C) H.push_back(p); }
			//else {
			//	for (int i = 0; i < C.size(); i++) {
			//		while (H.size() > 1 && ccw(H[H.size() - 2], H[H.size() - 1], C[i]) <= 0)
			//			H.pop_back();
			//		H.push_back(C[i]);
			//	}
			//	H.pop_back();
			//	int s = H.size() + 1;
			//	for (int i = C.size() - 1; i >= 0; i--) {
			//		while (H.size() > s && ccw(H[H.size() - 2], H[H.size() - 1], C[i]) <= 0)
			//			H.pop_back();
			//		H.push_back(C[i]);
			//	}
			//	H.pop_back();
			//}
			//std::cout << cutting_hull(poses, seq, knife, 0) << "\n";
			//std::cout << area(H) << "\n";
		}
	}
	return;
}
int main() { solve(); return 0; }//boj19508 Convex Hull

/*
test

1
6 1
0 0 1
1 0 1
0 1 1
0 0 -1
1 0 -1
0 1 -1
0 0 1 0

1
6 5
0 0 1
1 0 1
0 1 1
0 0 -1
1 0 -1
0 1 -1
0 0 -1 0
0 1 0 0
0 -1 0 0
1 0 0 0
-1 0 0 0

1
8 12
1 1 1
1 -1 1
-1 1 1
-1 -1 1
1 1 -1
1 -1 -1
-1 1 -1
-1 -1 -1
0 0 1 0
0 0 -1 0
0 1 0 0
0 -1 0 0
1 0 0 0
-1 0 0 0
1 1 0 0
1 0 1 0
0 1 1 0
-1 -1 0 0
-1 0 -1 0
0 -1 -1 0

*/
