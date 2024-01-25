#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <cstring>
typedef long long ll;
const int LEN = 1e6;
int N;
ll MAX = 1e17;

struct Pos {
	ll x, y;
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
} tmp[LEN];
struct Vec {
	ll vx, vy;
	ll operator / (const Vec& v) const { return vx * v.vy - vy * v.vx; }
};
std::vector<Pos> C, H, band;
ll Euclidean(const Pos& d1, const Pos& d2) {
	ll x = d1.x - d2.x, y = d1.y - d2.y;
	return  x * x + y * y ;
}
ll Manhattan(const Pos& d1, const Pos& d2) {
	ll x = d1.x - d2.x, y = d1.y - d2.y;
	return  std::abs(x) + std::abs(y) ;
}
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
}
std::vector<Pos> monotone_chain(std::vector<Pos>& C) {
	std::vector<Pos> H;
	std::sort(C.begin(), C.end());
	if (C.size() <= 2) {
		for (const Pos& p : C)
			H.push_back(p);
	}
	else {
		for (int i = 0; i < C.size(); i++) {
			while (H.size() > 1 && cross(H[H.size() - 2], H[H.size() - 1], C[i]) <= 0)
				H.pop_back();
			H.push_back(C[i]);
		}
		H.pop_back();
		int s = H.size() + 1;
		for (int i = C.size() - 1; i >= 0; i--) {
			while (H.size() > s && cross(H[H.size() - 2], H[H.size() - 1], C[i]) <= 0)
				H.pop_back();
			H.push_back(C[i]);
		}
		H.pop_back();
	}
	return H;
}
ll cross(const Vec& v1, const Vec& v2) {
	return v1.vx * v2.vy - v1.vy * v2.vx;
}
Vec V(std::vector<Pos>& H, int i) {
	int f = (i + 1) % H.size();
	i %= H.size();
	return { H[f].x - H[i].x, H[f].y - H[i].y };
}
ll rotating_calipers(std::vector<Pos>& H) {
	ll MD = 0;
	int f = 0, l = H.size();
	for (int i = 0; i < l; i++) {
		while (V(H, i) / V(H, f) > 0) {
			MD = std::max(MD, Euclidean(H[i], H[f]));
			f = (f + 1) % l;
		}
		MD = std::max(MD, Euclidean(H[i], H[f]));
	}
	return MD;
}
ll min_dist(bool flag, int s = 0, int e = N - 1) {
	if (s == e) return MAX;
	int m = s + e >> 1;
	ll x_mid = C[m].x, D = std::min(min_dist(flag, s, m), min_dist(flag, m + 1, e));

	int idx = s, l = s, r = m + 1;
	while (l <= m && r <= e) {
		if (C[l].y < C[r].y) tmp[idx++] = C[l++];
		else tmp[idx++] = C[r++];
	}
	while (l <= m) tmp[idx++] = C[l++];
	while (r <= e) tmp[idx++] = C[r++];
	for (int i = s; i <= e; i++) C[i] = tmp[i];// ...?

	band.clear();
	for (int i = s; i <= e; i++) {
		ll x = std::abs(C[i].x - x_mid);
		ll dist_x = flag ? x : x * x;
		if (dist_x < D) band.push_back(C[i]);
	}

	int sz = band.size();
	ll dist = D;
	for (int i = 1; i < sz; i++) {
		for (int j = i - 1; j >= 0; j--) {
			ll y = std::abs(band[i].y - band[j].y);
			ll dist_y = flag ? y : y * y;
			if (dist_y >= D) break;
			dist = flag ? std::min(dist, Manhattan(band[i], band[j])) : std::min(dist, Euclidean(band[i], band[j]));
		}
	}
	return dist;
}
void init() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	C.resize(N);
	for (int i = 0; i < N; i++) std::cin >> C[i].x >> C[i].y;
	H = monotone_chain(C);
}
ll Euclidean_MAX() { return rotating_calipers(H); }
ll Euclidean_MIN() { return min_dist(0); }
ll Chebyshev_MAX() {
	ll Mx = -MAX, My = -MAX, mx = MAX, my = MAX;
	for (int i = 0; i < N; i++) {
		Mx = std::max(Mx, C[i].x);
		My = std::max(My, C[i].y);
		mx = std::min(mx, C[i].x);
		my = std::min(my, C[i].y);
	}
	return std::max(Mx - mx, My - my);
}
ll Manhattan_MIN() { return min_dist(1); }
void rotate() {
	ll x, y;
	for (int i = 0; i < N; i++) {
		x = C[i].x + C[i].y, y = C[i].x - C[i].y;
		C[i] = { x, y };
	}
	std::sort(C.begin(), C.end());
	return;
}
ll Chebyshev_MIN() { return min_dist(1); }
ll Manhattan_MAX() { return Chebyshev_MAX(); }
//void solve() {
//	ll EMAX, EMIN, MMAX, MMIN, CMAX, CMIN;
//	init();
//	EMAX = Euclidean_MAX();
//	EMIN = Euclidean_MIN();
//	CMAX = Chebyshev_MAX();
//	MMIN = Manhattan_MIN();
//	rotate();
//	CMIN = Chebyshev_MIN() >> 1;
//	MMAX = Manhattan_MAX();
//	std::cout << EMAX << "\n" << EMIN << "\n" << MMAX << "\n" << MMIN << "\n" << CMAX << "\n" << CMIN << "\n";
//	return;
//}
void solve() {
	ll EMAX, EMIN, MMAX, MMIN, CMAX, CMIN;
	init();
	EMIN = Euclidean_MIN();
	std::cout << EMIN << "\n";
	return;
}
int main() { solve(); return 0; }//boj1830 algorithm from hui