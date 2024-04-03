#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
typedef long long ll;
const ll INF = 1e17;
const int LEN = 1e5 + 5;
int T, N, M, Q;
bool stack[LEN << 1];
int pos[LEN], tree[LEN << 3];

ll sum(const int& s, const int& e, int l = 1, int r = N, int n = 1) {
	if (e < l || r < s) return 0;
	if (s <= l && r <= e) return tree[n];
	int m = l + r >> 1;
	ll L = sum(s, e, l, m, n << 1);
	ll R = sum(s, e, m + 1, r, n << 1 | 1);
	return L + R;
}
void update(const int& idx, const int& diff, int l = 1, int r = N, int n = 1) {
	if (r < idx || idx < l) return;
	tree[n] += diff;
	if (l != r) {
		int m = l + r >> 1;
		update(idx, diff, l, m, n << 1);
		update(idx, diff, m + 1, r, n << 1 | 1);
	}
	return;
}
ll init(int l = 1, int r = N, int n = 1) {
	if (l == r) return tree[n] = stack[l];
	int m = l + r >> 1;
	ll L = init(l, m, n << 1);
	ll R = init(m + 1, r, (n << 1) + 1);
	return tree[n] = L + R;
}
void query(const int& q, int& t) {
	int i = pos[q];
	std::cout << sum(i + 1, t) << " ";
	update(i, -1);
	update(t, 1);
	pos[q] = t;
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> T;
	while (T--) {
		memset(stack, 0, sizeof stack);
		memset(pos, 0, sizeof pos);
		memset(tree, 0, sizeof tree);
		std::cin >> N >> M;
		int t = N;
		for (int i = 1; i <= N; i++) pos[i] = i, stack[i] = 1;
		N += M + 1;
		init();
		for (int j = 0; j < M; j++) std::cin >> Q, query(Q, ++t);
		std::cout << "\n";
	}
	return;
}
int main() { solve(); return 0; }//boj3653 Movie collection