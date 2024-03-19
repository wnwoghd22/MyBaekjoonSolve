#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
typedef long long ll;
typedef double ld;
const ll INF = 1e17;
const int LEN = 2e5 + 1;
//const ld TOL = 1e-7;
int N;
bool V[LEN];
//bool zero(const ld& x) { return std::abs(x) < TOL; }

struct Pos {
	int x, y, i;
	Pos(int X = 0, int Y = 0, int I = 0) : x(X), y(Y), i(I) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
	ll operator * (const Pos& p) const { return { (ll)x * p.x + (ll)y * p.y }; }
	ll operator / (const Pos& p) const { return { (ll)x * p.y - (ll)y * p.x }; }
	bool operator < (const Pos& p) const { return *this / p > 0; }
	bool operator <= (const Pos& p) const { return *this / p >= 0; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	ll Euc() const { return (ll)x * x + (ll)y * y; }
	friend std::istream& operator >> (std::istream& is, Pos& p) {
		is >> p.x >> p.y;
		return is;
	}
	friend std::ostream& operator << (std::ostream& os, const Pos& p) {
		os << p.x << " " << p.y;
		return os;
	}
}; const Pos O = { 0, 0 };
std::vector<Pos> C, H;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
//ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ll ret = cross(d1, d2, d3);
	return !ret ? 0 : ret > 0 ? 1 : -1;
}
ll area(const std::vector<Pos>& H) {
	ll ret = 0;
	int sz = H.size();
	for (int i = 0; i < sz; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % sz];
		ret += cross(O, cur, nxt);
	}
	return ret;
}
bool norm(std::vector<Pos>& H) {
	if (area(H) > 0) {
		std::reverse(H.begin(), H.end());
		return 1;
	}
	return 0;
}
int invisible(const Pos& p1, const Pos& p2, const Pos& t) {
	if (p1 / t == 0 && p1.Euc() < t.Euc()) return 1;
	if (p2 / t == 0 && p2.Euc() < t.Euc()) return 1;
	return ccw(O, p1, t) < 0 && ccw(O, p2, t) > 0 && ccw(p1, p2, t) > 0;
}
std::vector<int> stack;
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	memset(V, 0, sizeof V);
	std::cin >> N;
	H.resize(N);
	for (int i = 0; i < N; i++) std::cin >> H[i], H[i].i = i + 1;
	norm(H);//sort cw
	//int r = -1, l = -1;
	//ll LMIN = INF, RMIN = INF;
	int r = 0, l = 0;
	ll LMIN = H[0].Euc(), RMIN = H[0].Euc();
	for (int i = 0; i < N; i++) {
		if (H[r] / H[i] < 0 ||
			(H[r] / H[i] == 0 && RMIN > H[i].Euc())) {
			r = i, RMIN = H[i].Euc();
		}
		if (H[l] / H[i] > 0 ||
			(H[l] / H[i] == 0 && LMIN > H[i].Euc())) {
			l = i, LMIN = H[i].Euc();
		}
	}

	bool fvis = 1, bvis = 1, rvs = 0;
	stack.clear();
	stack.push_back(r);
	V[H[r].i] = 1;// , V[H[l].i] = 1;
	for (int i = r; i < r + N; i++) {
		if ((i + 1) % N == l) break;
		if (fvis) {
			if (H[i % N] / H[(i + 1) % N] < 0) {//move backward
				if (bvis && !rvs && ccw(H[(i - 1 + N) % N], H[i % N], H[(i + 1) % N]) < 0) {
					rvs = 1;
					fvis = 0;
					continue;
				}
				rvs = 1;
				if (bvis || (!bvis && H[stack.back()] / H[(i + 1) % N] < 0)) {
					if (!bvis) bvis = 1;
					stack.pop_back();
					while (stack.size() && invisible(H[i % N], H[(i + 1) % N], H[stack.back()])) stack.pop_back();
					stack.push_back(i + 1);
				}
			}
			if (!bvis) continue;
			else if (H[i % N] / H[(i + 1) % N] == 0) {//move vertical
				//if (!bvis) continue;
				if (H[i % N].Euc() > H[(i + 1) % N].Euc()) {
					if (rvs) rvs = 0;
					stack.pop_back();
					stack.push_back(i + 1);
				}
			}
			else if (H[i % N] / H[(i + 1) % N] > 0) {//move forward
				//if (!bvis) continue;
				if (rvs && ccw(H[(i - 1 + N) % N], H[i % N], H[(i + 1) % N]) > 0) {
					rvs = 0;
					bvis = 0;
					continue;
				}
				if (rvs && ccw(H[(i - 1 + N) % N], H[i % N], H[(i + 1) % N]) < 0) {
					rvs = 0;
				}
				stack.push_back(i + 1);
			}
			
		}
		if (!fvis) {
			if (H[stack.back()] / H[(i + 1) % N] > 0) {
				rvs = 0;
				fvis = 1;
				stack.push_back(i + 1);
			}
		}
	}
	for (const int& i : stack) V[H[i].i] = 1;
	std::cout << stack.size() << "\n";
	for (int i = 1; i <= N; i++) if (V[i]) std::cout << i << " ";
	return;
}
int main() { solve(); return 0; }//boj3303 Printed Circuit Board


//#define _CRT_SECURE_NO_WARNINGS
//#include <iostream>
//#include <algorithm>
//#include <cmath>
//#include <cstring>
//#include <cassert>
//#include <vector>
//typedef long long ll;
//typedef double ld;
//const ll INF = 1e17;
//const int LEN = 2e5 + 1;
//const ld TOL = 1e-7;
//int N;
//bool V[LEN];
//bool zero(const ld& x) { return std::abs(x) < TOL; }
////ll gcd(ll a, ll b) { return !b ? a : gcd(b, a % b); }
//
//struct Pos {
//	int x, y, i;
//	Pos(int X = 0, int Y = 0, int I = 0) : x(X), y(Y), i(I) {}
//	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
//	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
//	//bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
//	ll operator * (const Pos& p) const { return { (ll)x * p.x + (ll)y * p.y }; }
//	ll operator / (const Pos& p) const { return { (ll)x * p.y - (ll)y * p.x }; }
//	bool operator < (const Pos& p) const { return *this / p > 0; }
//	//bool operator <= (const Pos& p) const { return x == p.x ? y <= p.y : x <= p.x; }
//	bool operator <= (const Pos& p) const { return *this / p >= 0; }
//	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
//	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
//	//Pos operator * (const ll& n) const { return { x * n, y * n }; }
//	//Pos operator / (const ll& n) const { return { x / n, y / n }; }
//	//Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
//	//Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
//	//Pos& operator *= (const ll& scale) { x *= scale; y *= scale; return *this; }
//	//Pos& operator /= (const ll& scale) { x /= scale; y /= scale; return *this; }
//	//Pos operator ~ () const { return { -y, x }; }
//	//ll operator ! () const { return x * y; }
//	ll Euc() const { return (ll)x * x + (ll)y * y; }
//	//ll Man() const { return std::abs(x) + std::abs(y); }
//	friend std::istream& operator >> (std::istream& is, Pos& p) {
//		is >> p.x >> p.y;
//		return is;
//	}
//	friend std::ostream& operator << (std::ostream& os, const Pos& p) {
//		os << p.x << " " << p.y;
//		return os;
//	}
//}; const Pos O = { 0, 0 };
//std::vector<Pos> C, H;
//ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
////ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
//ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
////ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
//int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
//	ll ret = cross(d1, d2, d3);
//	return !ret ? 0 : ret > 0 ? 1 : -1;
//}
////bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) {
////	return !ccw(d1, d2, d3) && dot(d1, d3, d2) >= 0;
////}
////bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) {
////	return !ccw(d1, d2, d3) && dot(d1, d3, d2) > 0;
////}
////ll area(Pos H[], const int& sz) {
////	ll ret = 0;
////	for (int i = 0; i < sz; i++) {
////		Pos cur = H[i], nxt = H[(i + 1) % sz];
////		ret += cross(O, cur, nxt);
////	}
////	return ret;
////}
//ll area(const std::vector<Pos>& H) {
//	ll ret = 0;
//	int sz = H.size();
//	for (int i = 0; i < sz; i++) {
//		Pos cur = H[i], nxt = H[(i + 1) % sz];
//		ret += cross(O, cur, nxt);
//	}
//	return ret;
//}
////void norm(Pos H[], const int& sz) { if (area(H, sz) < 0) std::reverse(H, H + sz); }
//bool norm(std::vector<Pos>& H) {
//	if (area(H) > 0) {
//		std::reverse(H.begin(), H.end());
//		return 1;
//	}
//	return 0;
//}
//int invisible(const Pos& p1, const Pos& p2, const Pos& t) {
//	if (p1 / t == 0 && p1.Euc() < t.Euc()) return 1;
//	if (p2 / t == 0 && p2.Euc() < t.Euc()) return 1;
//	return ccw(O, p1, t) < 0 && ccw(O, p2, t) > 0 && ccw(p1, p2, t) > 0;
//}
//std::vector<int> stack;
//void solve() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	memset(V, 0, sizeof V);
//	std::cin >> N;
//	H.resize(N);
//	int r = -1, l = -1;
//	ll LMIN = INF, RMIN = INF;
//	for (int i = 0; i < N; i++) {
//		std::cin >> H[i];
//		H[i].i = i + 1;
//		//if (!~r ||
//		//	(H[r] / H[i] < 0 || 
//		//		(H[r] / H[i] == 0 && RMIN > H[i].Euc()))) {
//		//	r = i, RMIN = H[i].Euc();
//		//}
//		//if (!~l ||
//		//	(H[l] / H[i] > 0 || 
//		//		(H[l] / H[i] == 0 && LMIN > H[i].Euc()))) {
//		//	l = i, LMIN = H[i].Euc();
//		//}
//	}
//	bool f = norm(H);//sort cw
//	//if (f) l = N - 1 - l, r = N - 1 - r;
//	for (int i = 0; i < N; i++) {
//		if (!~r ||
//			(H[r] / H[i] < 0 ||
//				(H[r] / H[i] == 0 && RMIN > H[i].Euc()))) {
//			r = i, RMIN = H[i].Euc();
//		}
//		if (!~l ||
//			(H[l] / H[i] > 0 ||
//				(H[l] / H[i] == 0 && LMIN > H[i].Euc()))) {
//			l = i, LMIN = H[i].Euc();
//		}
//	}
//
//	bool fvis = 1, bvis = 1, rvs = 0;
//	stack.clear();
//	stack.push_back(r);
//	V[H[r].i] = 1;// , V[H[l].i] = 1;
//	for (int i = r; i < r + N; i++) {
//		if (i % N == l) break;
//		if (fvis) {
//			if (H[i % N] / H[(i + 1) % N] < 0) {
//				rvs = 1;
//				if (cross(H[(i - 1 + N) % N], H[i % N], H[(i + 1) % N]) < 0) {
//					rvs = 0; fvis = 0; continue;
//				}
//				stack.pop_back();
//				while (stack.size() && invisible(H[i % N], H[(i + 1) % N], H[stack.back()])) H.pop_back();
//				stack.push_back(i + 1);
//			}
//			else if (H[i % N] / H[(i + 1) % N] == 0) {
//				if (H[i % N].Euc() > H[(i + 1) % N].Euc()) {
//					if (rvs) rvs = 0;
//					stack.pop_back();
//					stack.push_back(i + 1);
//				}
//				else if (H[i % N].Euc() < H[(i + 1) % N].Euc()) continue;
//
//			}
//			else if (H[i % N] / H[(i + 1) % N] > 0) {
//				if (rvs && ccw(H[(i - 1 + N) % N], H[i % N], H[(i + 1) % N]) < 0) {
//					rvs = 0;
//				}
//				else if (rvs && ccw(H[(i - 1 + N) % N], H[i % N], H[(i + 1) % N]) > 0) {
//					rvs = 0, bvis = 0; continue;
//				}
//				stack.push_back(i + 1);
//			}
//			
//		}
//		if (!fvis) {
//			if (H[stack.back()] / H[(i + 1) % N] > 0) {
//				fvis = 1;
//				stack.push_back(i + 1);
//			}
//		}
//		if (!bvis) {
//			if (H[stack.back()] / H[(i + 1) % N] > 0) {
//				bvis = 1;
//				stack.push_back(i + 1);
//			}
//		}
//	}
//	for (const int& i : stack) V[H[i].i] = 1;
//	for (int i = 1; i <= N; i++) if (V[i]) std::cout << i << " ";
//	return;
//}
//int main() { solve(); return 0; }//boj3303 Printed Circuit Board