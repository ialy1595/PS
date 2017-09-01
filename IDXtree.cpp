#include <stdio.h>
#include <memory.h>
#include <algorithm>
#include <vector>
using namespace std;

#define INF 2147483647

//min
typedef struct idxtree {

	vector<int> idx;
	int ll, rr;

	int idx_init_t(int l, int r, int n, int* arr)
	{
		if (l == r)
		{
			idx[n] = arr[l];
			return idx[n];
		}
		int m = (l + r) / 2;
		idx[n] = min(idx_init_t(l, m, n * 2, arr), idx_init_t(m + 1, r, n * 2 + 1, arr));
		return idx[n];
	}

	void idx_init(int l, int r, int* arr)
	{
		idx.resize((r - l + 1) * 4);
		ll = l;
		rr = r;
		idx_init_t(l, r, 1, arr);
	}

	int idx_q_t(int l, int r, int n, int nl, int nr)
	{
		if (r < nl || nr < l) return INF;
		if (l <= nl && nr <= r) return idx[n];
		int m = (nl + nr) / 2;
		return min(idx_q_t(l, r, n * 2, nl, m), idx_q_t(l, r, n * 2 + 1, m + 1, nr));
	}

	int idx_q(int l, int r)
	{
		return idx_q_t(l, r, 1, ll, rr);
	}
}idxtree;