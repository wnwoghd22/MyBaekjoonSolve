#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <vector>
typedef long long ll;
//typedef long double ld;
typedef double ld;
const ll INF = 1e17;
int T, N;

struct Pos {
	ll x, y;
	Pos(ll X = 0, ll Y = 0) : x(X), y(Y) {}
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	ll operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
	ll operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
	Pos& operator *= (const ll& scale) { x *= scale; y *= scale; return *this; }
	ll Euc() const { return x * x + y * y; }
	ld mag() const { return hypot(x, y); }
};
std::vector<Pos> C, H;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
//std::vector<Pos> monotone_chain(std::vector<Pos>& C) {
//	std::vector<Pos> H;
//	std::sort(C.begin(), C.end());
//	if (C.size() <= 2) {
//		for (const Pos& p : C) H.push_back(p);
//		return H;
//	}
//	for (int i = 0; i < C.size(); i++) {
//		while (H.size() > 1 && cross(H[H.size() - 2], H[H.size() - 1], C[i]) <= 0)
//			H.pop_back();
//		H.push_back(C[i]);
//	}
//	H.pop_back();
//	int sz = H.size() + 1;
//	for (int i = C.size() - 1; i >= 0; i--) {
//		while (H.size() > sz && cross(H[H.size() - 2], H[H.size() - 1], C[i]) <= 0)
//			H.pop_back();
//		H.push_back(C[i]);
//	}
//	H.pop_back();
//	return H;
//}
//void init() {
//	std::cin >> N;
//	C.resize(N);
//	for (int i = 0; i < N; i++) std::cin >> C[i].x >> C[i].y;
//	//H = monotone_chain(C);
//	//N = H.size();
//	return;
//}
void rotating_calipers() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(6);
	std::cin >> T;
	while (T--) {
		std::cin >> N;
		H.resize(N);
		for (int i = 0; i < N; i++) std::cin >> H[i].x >> H[i].y;
		std::reverse(H.begin(), H.end());
		//std::swap(C[0], *min_element(C.begin(), C.end()));
		//std::sort(C.begin() + 1, C.end(), [&](const Pos& p, const Pos& q) {
		//	ll ret = cross(C[0], p, q);
		//	if (ret != 0) return ret > 0;
		//	return (C[0] - p).Euc() < (C[0] - q).Euc();
		//	});
		//for (int i = 0; i < N; i++) {
		//	while (H.size() >= 2 && cross(H[H.size() - 2], H.back(), C[i]) <= 0) {
		//		H.pop_back();
		//	}
		//	H.push_back(C[i]);
		//}
		//N = H.size();
		if (N == 2) { std::cout << "0\n"; continue; }

		int R = 0, U = 0;
		for (int j = 0; j < N; j++) if (dot(H[0], H[1], H[j], H[(j + 1) % N]) <= 0) { R = j; break; }
		for (int k = R; k < N; k++) if (cross(H[0], H[1], H[k], H[(k + 1) % N]) <= 0) { U = k; break; }
		//for (int l = U; l <= N; l++) if (dot(H[0], H[1], H[l % N], H[(l + 1) % N]) >= 0) { L = l; break; }

		int cnt = 0;
		for (int i = 0; i < N; i++) {
			//while (dot(H[i], H[(i + 1) % N], H[R % N], H[(R + 1) % N]) > 0) R++;
			while (cross(H[i], H[(i + 1) % N], H[U % N], H[(U + 1) % N]) > 0) U++;
			//while (dot(H[i], H[(i + 1) % N], H[L % N], H[(L + 1) % N]) < 0) L++;
			cnt++;
			if (!cross(H[i], H[(i + 1) % N], H[U % N], H[(U + 1) % N])) cnt++, U++;
			if (U == N + 1) break;
		}
		std::cout << cnt << "\n";
	}
	return;
}
int main() { rotating_calipers(); return 0; }//boj6090
