#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <memory.h>
#include <queue>
#include <utility>  

using namespace std;

#define INF 1000000007

typedef struct edg
{
	int d, i, f, c;
} edg;

typedef struct MCMF
{
	int vn;
	vector<vector<edg>> edgs;
	vector<int> bfsq;
	vector<int> lv;
	queue<int> qu;
	vector<int> inQ;
	vector<pair<int, pair<int, int>>> spfa;

	void init(int n)
	{
		int i;
		vn = n;
		edgs.resize(n);
		for (i = 0; i < n; i++) edgs[i].clear();
	}

	void addEdge(int s, int d, int f, int c)
	{
		edg x{ d,edgs[d].size(), f, c };
		edg y{ s,edgs[s].size(), 0, -c };

		edgs[s].push_back(x);
		edgs[d].push_back(y);
	}

	bool bfse(int s, int d)
	{
		int v, i, j, t = 0;
		memset(&lv[0], 0, sizeof(lv[0]) * lv.size());
		lv[s] = 1;
		bfsq[t++] = s;
		for (i = 0; i < t && lv[d] == 0; i++)
		{
			v = bfsq[i];
			for (j = 0; j < edgs[v].size(); j++) if (lv[edgs[v][j].d] == 0 && edgs[v][j].f > 0)
			{
				lv[edgs[v][j].d] = lv[v] + 1;
				bfsq[t++] = edgs[v][j].d;
			}
		}
		if (lv[d] > 0) return true;
		else return false;
	}

	int flowing(int x, int d, int f)
	{
		if (x == d) return f;
		int i, ff;
		for (i = 0; i < edgs[x].size(); i++)
		{
			edg &e = edgs[x][i];
			if (e.f > 0 && lv[e.d] == lv[x] + 1)
			{
				ff = flowing(e.d, d, min(f, e.f));
				if (ff > 0)
				{
					e.f -= ff;
					edgs[e.d][e.i].f += ff;
					return ff;
				}
			}
		}
		return 0;
	}

	int solve(int s, int d)
	{
		bfsq.resize(vn);
		lv.resize(vn);
		int ff, ans = 0;
		while (bfse(s, d))
		{
			while (1)
			{
				ff = flowing(s, d, INF);
				if (ff == 0) break;
				ans += ff;
			}
		}
		return ans;
	}

	pair<int, int> solveMC(int s, int d, int clmt)
	{
		int i;
		int u, v, w;
		int ff, cost = 0, fl = 0;
		spfa.resize(vn);
		inQ.resize(vn);

		while (1)
		{
			while (!qu.empty()) qu.pop();
			qu.push(s);
			for (i = 0; i < vn; i++)
			{
				spfa[i].first = INF;
				spfa[i].second.second = INF;
				inQ[i] = 0;
			}
			spfa[s].first = 0;
			inQ[s]++;

			while (!qu.empty())
			{
				v = qu.front();
				qu.pop();
				inQ[v] = 0;
				for (i = 0; i < edgs[v].size(); i++) if (edgs[v][i].f > 0)
				{
					u = edgs[v][i].d;
					w = edgs[v][i].c;
					if (spfa[v].first + w < spfa[u].first)
					{
						spfa[u].first = spfa[v].first + w;
						spfa[u].second.first = edgs[v][i].i;
						spfa[u].second.second = min(spfa[v].second.second, edgs[v][i].f);
						if (inQ[u] == 0)
						{
							qu.push(u);
							inQ[u]++;
						}
					}
				}
			}

			if (spfa[d].first >= clmt) break;

			ff = spfa[d].second.second;
			fl += ff;

			v = d;
			while (1)
			{
				edg &ei = edgs[v][spfa[v].second.first];
				edg &e = edgs[ei.d][ei.i];
				e.f -= ff;
				ei.f += ff;
				cost += e.c*ff;
				if (ei.d == s) break;
				v = ei.d;
			}
		}

		return make_pair(fl, cost);
	}
} MCMF;