#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <algorithm>
#include <vector>
using namespace std;

#define MAXLEN 220000
#define INF 2147483647

typedef struct idxtree {

	vector<int> idx;
	int ll, rr;

	int idx_init_t(int l, int r, int n, vector<int>* arr)
	{
		if (l == r)
		{
			idx[n] = (*arr)[l];
			return idx[n];
		}
		int m = (l + r) / 2;
		idx[n] = min(idx_init_t(l, m, n * 2, arr), idx_init_t(m + 1, r, n * 2 + 1, arr));
		return idx[n];
	}

	void idx_init(int l, int r, vector<int>* arr)
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

typedef struct sfxarray {
	vector<int> nums;
	vector<int> sfxarr;
	vector<int> psfxarr;
	vector<int> ord;
	vector<int> nord;
	vector<int> cnt;
	vector<int> lcp;
	vector<int> fnear;
	vector<int> bnear;
	int len;
	int sz;
	idxtree idt;

	//s is i-index
	void sfx_init(char* strr, int mm, int base)
	{
		int i, m, p, t;

		len = strlen(strr + 1);
		m = mm;

		sz = max(len, m);

		nums.resize(sz + 2);
		sfxarr.resize(sz + 2);
		psfxarr.resize(sz + 2);
		ord.resize(sz * 2 + 2);
		nord.resize(sz * 2 + 2);
		cnt.resize(sz + 2);
		//fill(cnt.begin(), cnt.end(), 0);
		memset(&cnt[0], 0, sizeof(cnt[0]) * cnt.size());
		//fill(ord.begin(), ord.end(), 0);
		memset(&ord[0], 0, sizeof(ord[0]) * ord.size());

		for (i = 1; i <= len; i++) nums[i] = strr[i] - base + 1;

		for (i = 1; i <= len; i++) ord[i] = nums[i];
		for (i = 1; i <= len; i++) cnt[ord[i]]++;
		for (i = 1; i <= m; i++) cnt[i] += cnt[i - 1];
		for (i = len; i > 0; i--) sfxarr[cnt[ord[i]]--] = i;

		for (p = 1; p < len; p <<= 1)
		{
			t = 0;
			for (i = len - p + 1; i <= len; i++) psfxarr[++t] = i;
			for (i = 1; i <= len; i++) if (sfxarr[i] > p) psfxarr[++t] = sfxarr[i] - p;

			//memset(cnt, 0, sizeof(cnt));
			//fill(cnt.begin(), cnt.end(), 0);
			memset(&cnt[0], 0, sizeof(cnt[0]) * cnt.size());
			for (i = 1; i <= len; i++) cnt[ord[i]]++;
			for (i = 2; i <= m; i++) cnt[i] += cnt[i - 1];
			for (i = len; i > 0; i--) sfxarr[cnt[ord[psfxarr[i]]]--] = psfxarr[i];

			m = 0;
			for (i = 1; i <= len; i++)
			{
				if (i == 1 || ord[sfxarr[i]] != ord[sfxarr[i - 1]] || ord[sfxarr[i] + p] != ord[sfxarr[i - 1] + p]) m++;
				nord[sfxarr[i]] = m;
			}
			//memcpy(ord, nord, sizeof(nord));
			ord = nord;
			if (m == len) break;
		}
	}

	void lcp_init()
	{
		int i, j, h;

		lcp.resize(sz + 2);

		for (i = 1, h = 0; i <= len; i++)
		{
			if (ord[i] > 1)
			{
				j = sfxarr[ord[i] - 1];
				while (nums[i + h] == nums[j + h]) h++;
				lcp[ord[i]] = h;
			}
			if (h > 0) h--;
		}
	}

	void lcp_q_init()
	{
		idt.idx_init(2, len, &lcp);
	}

	int lcp_q(int l, int r)
	{
		return idt.idx_q(min(ord[l], ord[r]) + 1, max(ord[l], ord[r]));
	}

	//for pre-string
	void near_init(int pos)
	{
		int i, fn, bn;

		fnear.resize(sz);
		bnear.resize(sz);

		for (i = 1, fn = 0; i <= len; i++)
		{
			if (sfxarr[i] > pos)
			{
				fn = i;
			}
			else if (sfxarr[i] < pos)
			{
				fnear[sfxarr[i]] = fn;
			}
		}
		for (i = len, bn = 0; i > 0; i--)
		{
			if (sfxarr[i] > pos)
			{
				bn = i;
			}
			else if (sfxarr[i] < pos)
			{
				bnear[sfxarr[i]] = bn;
			}
		}
	}

	int lcp_q_fb(int x)
	{
		int maxl = 0, lcplen;
		if (fnear[x] != 0)
		{
			lcplen = idt.idx_q(min(ord[x], fnear[x]) + 1, max(ord[x], fnear[x]));
			maxl = max(maxl, lcplen);
		}

		if (bnear[x] != 0)
		{
			lcplen = idt.idx_q(min(ord[x], bnear[x]) + 1, max(ord[x], bnear[x]));
			maxl = max(maxl, lcplen);
		}

		return maxl;
	}
}sfxarray;