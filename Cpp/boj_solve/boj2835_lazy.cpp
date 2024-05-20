#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <queue>
#include <cassert>
typedef long long ll;
typedef double ld;
const int LEN = 1e5 + 5;
const ll INF = 1e17;

int N, M, K;
int a, b, c;
ll d;
ll arr[LEN], tree[LEN << 2], lazy[LEN << 2];
void lazy_propagate(int s, int e, int i) {
	if (lazy[i]) {
		tree[i] += lazy[i] * ((ll)e - s + 1);
		if (s ^ e) {
			lazy[i << 1] += lazy[i];
			lazy[i << 1 | 1] += lazy[i];
		}
		lazy[i] = 0;
	}
	return;
}
void update(int l, int r, ll diff, int s = 1, int e = K, int i = 1) {
	lazy_propagate(s, e, i);
	if (r < s || e < l) return;
	if (l <= s && e <= r) {
		tree[i] += diff * ((ll)e - s + 1);
		if (s ^ e) {
			lazy[i << 1] += diff;
			lazy[i << 1 | 1] += diff;
		}
		return;
	}
	int m = s + e >> 1;
	update(l, r, diff, s, m, i << 1);
	update(l, r, diff, m + 1, e, i << 1 | 1);
	tree[i] = tree[i << 1] + tree[i << 1 | 1];
	return;
}
ll search(int l, int r, int s = 1, int e = K, int i = 1) {
	lazy_propagate(s, e, i);
	if (r < s || e < l) return 0;
	if (l <= s && e <= r) return tree[i];
	int m = s + e >> 1;
	return search(l, r, s, m, i << 1) + search(l, r, m + 1, e, i << 1 | 1);
}
ll init(int s = 1, int e = N, int i = 1) {
	if (s == e) return tree[i] = arr[s];
	int m = s + e >> 1;
	tree[i << 1] = init(s, m, i << 1);
	tree[i << 1 | 1] = init(m + 1, e, i << 1 | 1);
	return tree[i] = tree[i << 1] + tree[i << 1 | 1];
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(9);
	K = 87'000;
	std::cin >> N;
	for (int i = 1; i <= N; i++) {
		int hs, ms, ss, he, me, se;
		int f = scanf("%d:%d:%d - %d:%d:%d", &hs, &ms, &ss, &he, &me, &se);
		int s = hs * 3600 + ms * 60 + ss + 1;
		int e = he * 3600 + me * 60 + se + 1;
		if (s < e) update(s + 1, e + 1, 1);
		else update(1, e + 1, 1), update(s + 1, 86400, 1);
	}
	std::cin >> M;
	while (M--) {
		int hs, ms, ss, he, me, se;
		int f = scanf("%d:%d:%d - %d:%d:%d", &hs, &ms, &ss, &he, &me, &se);
		int s = hs * 3600 + ms * 60 + ss + 1;
		int e = he * 3600 + me * 60 + se + 1;
		ld ret;
		if (s <= e) ret = search(s + 1, e + 1) / ((ld)e - s + 1);
		else ret = (search(1, e + 1) + search(s + 1, 86400)) / ((ld)e + (86400. - s) + 1.);
		std::cout << ret << "\n";
	}
	return;
}
int main() { solve(); return 0; }//10999 Find sum of intervals 2
