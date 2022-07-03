#include<iostream>
#include<cstdio>
#include<algorithm>
#include<queue>
#define MAXN 300001

using namespace std;

int head[MAXN], dis[MAXN][11], edge_num, j;
bool vis[MAXN][11];
int ans[MAXN][11];

struct edge {
	int dis_e;
	int end_e;
	int next_e;
}e[5 * MAXN];

struct node {
	int dis;
	int pos;
	int level;
	bool operator<(const node& x)const
	{
		if (level != x.level)return level > x.level;//DP优先确立上一层
		return dis > x.dis;
	}
};

priority_queue<node>q;

void add_edge(int begin, int end, int dis)
{
	edge_num++;
	e[edge_num].dis_e = dis;
	e[edge_num].end_e = end;
	e[edge_num].next_e = head[begin];
	head[begin] = edge_num;
}

void dijkstra(int s)
{
	dis[s][0] = 0;
	ans[s][0] = 1;
	node first;
	first.dis = 0;
	first.pos = s;
	first.level = 0;
	q.push(first);
	while (!q.empty())
	{
		node tmp = q.top();
		q.pop();
		int x = tmp.pos, dist = tmp.dis, cur_level = tmp.level;
		if (vis[x][cur_level])
			continue;
		vis[x][cur_level] = 1;
		for (int i = head[x]; i; i = e[i].next_e) //DP
		{
			if (dis[e[i].end_e][cur_level] == dis[x][cur_level] + e[i].dis_e)
				ans[e[i].end_e][cur_level] += ans[x][cur_level];
			if (dis[e[i].end_e][cur_level] > dis[x][cur_level] + e[i].dis_e) //不免费
			{
				dis[e[i].end_e][cur_level] = dis[x][cur_level] + e[i].dis_e;
				ans[e[i].end_e][cur_level] = ans[x][cur_level];
				node temp;
				temp.dis = dis[e[i].end_e][cur_level];
				temp.pos = e[i].end_e;
				temp.level = cur_level;
				if (!vis[e[i].end_e][cur_level])
					q.push(temp);
			}
			if (cur_level < j) //免费
			{
				if (dis[e[i].end_e][cur_level + 1] == dis[x][cur_level])
					ans[e[i].end_e][cur_level + 1] += ans[x][cur_level];
				if (dis[e[i].end_e][cur_level + 1] > dis[x][cur_level])
				{
					dis[e[i].end_e][cur_level + 1] = dis[x][cur_level];
					ans[e[i].end_e][cur_level + 1] = ans[x][cur_level];
					node temp;
					temp.dis = dis[e[i].end_e][cur_level + 1];
					temp.pos = e[i].end_e;
					temp.level = cur_level + 1;
					if (!vis[e[i].end_e][cur_level+1])
						q.push(temp);
				}
			}
		}
	}
}

int readNum() {   // 快速读入
	char ch = getchar();
	while (ch < '0' or ch > '9') ch = getchar();
	int v = 0;
	while (ch >= '0' and ch <= '9') {
		v = v * 10 + ch - '0';
		ch = getchar();
	}
	return v;
}

int main()
{
	int n, m;
	int s, d;
	int res = 0;
	n = readNum(); m = readNum(); j = readNum();
	//s = readNum();
	for (int i = 0; i <= n; i++)
		for(int k=0;k<11;k++)
			dis[i][k] = 0x7FFFFFFF;
	for (int i = 1; i <= m; i++)
	{
		int begin, end, dis;
		begin = readNum(); end = readNum(); dis = readNum();
		add_edge(begin, end, dis);
		add_edge(end, begin, dis);
	}
	s = readNum(); d = readNum();
	dijkstra(s);
	//for (int i = 1; i <= n; i++)
		//printf("%d ", dis[i]);
	int minn = 0x7FFFFFFF;
	for (int i = 0; i < 11; i++)
	{
		if (minn > dis[d][i])
		{
			res = i;
			minn = dis[d][i];
		}
	}
	printf("%d %d", minn, ans[d][res]);
	return 0;
}