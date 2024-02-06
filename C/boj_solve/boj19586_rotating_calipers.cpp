#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <vector>
typedef long long ll;
typedef long double ld;
const ll INF = 1e17;
const int LEN = 5e4;
int N;

struct Pos {
	ll x, y;
	Pos(ll X, ll Y) : x(X), y(Y) {}
	Pos() : x(0), y(0) {}
	//bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	//Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	//Pos operator * (const ll& n) const { return { x * n, y * n }; }
	//Pos operator / (const ll& n) const { return { x / n, y / n }; }
	ll operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
	ll operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
	Pos operator ~ () const { return { -y, x }; }
	//ll operator ! () const { return x * y; }
	Pos& operator *= (const ll& scale) { x *= scale; y *= scale; return *this; }
	ll Euc() const { return x * x + y * y; }
	ld mag() const { return hypot(x, y); }
};
std::vector<Pos> C, H;
const Pos O = { 0, 0 }, MAXL = { 0, INF }, MAXR = { INF, 0 }, pivot = { -1, -1 };
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ll ret = cross(d1, d2, d3);
	return !ret ? 0 : ret > 0 ? 1 : -1;
}
bool on_seg(const Pos& d1, const Pos& d2, const Pos& d3) {
	return !ccw(d1, d2, d3) && dot(d1, d3, d2) >= 0;
}
std::vector<Pos> monotone_chain(std::vector<Pos>& C) {
	std::vector<Pos> H;
	std::sort(C.begin(), C.end());
	if (C.size() <= 2) {
		for (const Pos& pos : C) H.push_back(pos);
		return H;
	}
	for (int i = 0; i < C.size(); i++) {
		while (H.size() > 1 && cross(H[H.size() - 2], H[H.size() - 1], C[i]) <= 0) 
			H.pop_back();
		H.push_back(C[i]);
	}
	H.pop_back();
	int sz = H.size() + 1;
	for (int i = C.size() - 1; i >= 0; i--) {
		while (H.size() > sz && cross(H[H.size() - 2], H[H.size() - 1], C[i]) <= 0) 
			H.pop_back();
		H.push_back(C[i]);
	}
	H.pop_back();
	return H;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(8);
	std::cin >> N;
	C.resize(N);
	for (int i = 0; i < N; i++) std::cin >> C[i].x >> C[i].y, C[i] *= 2;
	H = monotone_chain(C);
	N = H.size();
	if (N == 2) { std::cout << (H[0] - H[1]).mag() << "\n"; return; }
	//for (int i = 0; i < N; i++) std::cout << H[i].x << " " << H[i].y << "\n";

	int U{ 0 }, R{ 0 }, L{ 0 };
	for (int j = 1; j < N; j++) if (cross(H[0], H[1], H[j % N], H[(j + 1) % N]) <= 0) { U = j; break; }
	//std::cout << "DEBUG U: " << U << " \n";
	for (int k = 0; k < N; k++) if (dot(H[0], H[1], H[k % N], H[(k + 1) % N]) <= 0) { R = k; break; }
	//std::cout << "DEBUG R: " << R << " \n";
	for (int l = U; l <= N; l++) if (dot(H[0], H[1], H[l % N], H[(l + 1) % N]) >= 0) { L = l; break; }
	//std::cout << "DEBUG L: " << L << " \n";
	U %= N, R %= N, L %= N;

	ld MIN{ INF };
	for (int i = 0; i <= N; i++) {
		while (cross(H[i % N], H[(i + 1) % N], H[U % N], H[(U + 1) % N]) > 0) U++;
		while (dot(H[i % N], H[(i + 1) % N], H[R % N], H[(R + 1) % N]) > 0) R++;
		while (dot(H[i % N], H[(i + 1) % N], H[L % N], H[(L + 1) % N]) < 0) L++;
		U %= N, R %= N, L %= N;
		ld h = cross(H[i % N], H[(i + 1) % N], H[U % N]) / (H[i % N] - H[(i + 1) % N]).mag();
		ld w = dot(H[i % N], H[(i + 1) % N], H[L % N], H[R % N]) / (H[i % N] - H[(i + 1) % N]).mag();
		ld box = h + w;
		MIN = std::min(MIN, box);
		//std::cout << "DEBUG i: " << i << " U: " << U << " R: " << R << " L: " << L << " box: " << box << " area: " << MIN << "\n";
	}
	std::cout << MIN << "\n";
	return;
}
int main() { solve(); return 0; }//boj19586

/*

3
0 0
2 0
1 2

3
0 0
2 0
-2 2

4.4721359 0.893327

*/



//Pos V(std::vector<Pos>& H, int i) {
//	int f = (i + 1) % H.size();
//	i %= H.size();
//	return { H[f].x - H[i].x, H[f].y - H[i].y };
//}
//ll rotating_calipers(std::vector<Pos>& H) {
//	ll MD = 0;
//	int f = 0, l = H.size();
//	for (int i = 0; i < l; i++) {
//		while (V(H, i) / V(H, f) > 0) {
//			MD = std::max(MD, (H[i] - H[f]).Euc());
//			f = (f + 1) % l;
//		}
//		MD = std::max(MD, (H[i] - H[f]).Euc());
//	}
//	return MD;
//}
