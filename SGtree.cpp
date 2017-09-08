#include <stdio.h>
#include <vector>
#include <algorithm>
using namespace std;

#define ll long long int

struct ND
{
	ll s, e, x, l;
};

struct SGtree
{
	vector<ND> tr;
	ll n;

	void init(ll* arr, ll nn)
	{
		n = nn;
		tr.resize(4 * n);
		init_tr(arr, 0, n - 1, 1);
	}

	ll init_tr(ll* arr, ll s, ll e, ll t)
	{
		tr[t].s = s;
		tr[t].e = e;
		tr[t].l = 0;
		if (s == e) tr[t].x = arr[s];
		else tr[t].x = init_tr(arr, s, (s + e) / 2, t * 2) + init_tr(arr, ((s + e) / 2) + 1, e, (t * 2) + 1);
		return tr[t].x;
	}

	ll q_sum(ll s, ll e, ll t)
	{
		if (tr[t].e < s || tr[t].s > e) return 0;
		if (tr[t].l != 0)
		{
			tr[t].x += (tr[t].e - tr[t].s + 1) * tr[t].l;
			if (tr[t].s != tr[t].e)
			{
				tr[t * 2].l += tr[t].l;
				tr[t * 2 + 1].l += tr[t].l;
			}
			tr[t].l = 0;
		}
		if (tr[t].s < s || tr[t].e > e)
		{
			return q_sum(s, e, t * 2) + q_sum(s, e, t * 2 + 1);
		}
		else
		{
			return tr[t].x;
		}
	}

	void q_update(ll s, ll e, ll x, ll t)
	{
		if (tr[t].e < s || tr[t].s > e) return;
		if (tr[t].l > 0)
		{
			tr[t].x += (tr[t].e - tr[t].s + 1) * tr[t].l;
			if (tr[t].s != tr[t].e)
			{
				tr[t * 2].l += tr[t].l;
				tr[t * 2 + 1].l += tr[t].l;
			}
			tr[t].l = 0;
		}
		tr[t].x += (min(tr[t].e, e) - max(tr[t].s, s) + 1) * x;
		if (tr[t].s < s || tr[t].e > e)
		{
			q_update(s, e, x, t * 2);
			q_update(s, e, x, t * 2 + 1);
		}
		else
		{
			if (tr[t].s != tr[t].e)
			{
				tr[t * 2].l += x;
				tr[t * 2 + 1].l += x;
			}
		}
	}
};