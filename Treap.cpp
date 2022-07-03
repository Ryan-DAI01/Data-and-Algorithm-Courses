#include<iostream>
#include<cstdio>
#include<algorithm>
#define MAXN 300001
using namespace std;
int total = 0;
int stu[MAXN];
int n, m;
int root = 0;

class Treap {
public:
	int size, value;
	int num;//重复人数
	int son[2];
	int randx;//随机生成优先级
}treap[MAXN];

void update(int p)
{
	treap[p].size = treap[treap[p].son[0]].size + treap[treap[p].son[1]].size + treap[p].num;
	return;
}

void rotation(int& p, int flag)//flag=0左旋，flag=1右旋
{
	if (!flag)
	{
		int tmp = treap[p].son[1];
		treap[p].son[1] = treap[tmp].son[0];
		treap[tmp].son[0] = p;
		update(p); update(tmp);
		p = tmp;
	}
	else
	{
		int tmp = treap[p].son[0];
		treap[p].son[0] = treap[tmp].son[1];
		treap[tmp].son[1] = p;
		update(p); update(tmp);
		p = tmp;
	}
	return;
}

void insert(int& p, int x)
{
	if (!p)
	{
		p = ++total;
		treap[p].size = 1;
		treap[p].num = 1;
		treap[p].value = x;
		treap[p].randx = rand();
		return;
	}
	if (treap[p].value == x)
	{
		treap[p].size += 1;
		treap[p].num += 1;
		return;
	}
	if (x > treap[p].value)
	{
		insert(treap[p].son[1], x);
		if (treap[p].randx > treap[treap[p].son[1]].randx)rotation(p, 0);//维护小根堆
	}
	else
	{
		insert(treap[p].son[0], x);
		if (treap[p].randx > treap[treap[p].son[0]].randx)rotation(p, 1);//维护小根堆
	}
	update(p);
	return;
}

void del(int& p, int x)
{
	if (!p)return;
	if (x < treap[p].value)del(treap[p].son[0], x);
	else if (x > treap[p].value) del(treap[p].son[1], x);
	else
	{
		if (!treap[p].son[0] && !treap[p].son[1])
		{
			treap[p].num--;
			treap[p].size--;
			if (!treap[p].num)p = 0;
		}
		else if (!treap[p].son[1])
		{
			rotation(p, 1);
			del(treap[p].son[1], x);
		}
		else if (!treap[p].son[0])
		{
			rotation(p, 0);
			del(treap[p].son[0], x);
		}
		else
		{
			if (treap[treap[p].son[0]].randx > treap[treap[p].son[1]].randx)
			{
				rotation(p, 0);
				del(treap[p].son[0], x);
			}
			else
			{
				rotation(p, 1);
				del(treap[p].son[1], x);
			}
		}
	}
	update(p);
}


int Rank(int p, int x)
{
	if (!p)return 0;
	if (treap[p].value == x)
		return treap[treap[p].son[1]].size + 1;
	if (x < treap[p].value)
		return treap[treap[p].son[1]].size + treap[p].num + Rank(treap[p].son[0], x);
	if (x > treap[p].value)
		return Rank(treap[p].son[1], x);
}

int pre(int p, int x)
{
	if (!p)return 0xFFFFFFFF;
	if (x >= treap[p].value)return pre(treap[p].son[1], x);
	else
	{
		int tmp = pre(treap[p].son[0], x);
		if (tmp == 0xFFFFFFFF)return treap[p].value;
		return tmp;
	}
}

int next(int p, int x)
{
	if (!p)return 0xFFFFFFFF;
	if (x <= treap[p].value)return next(treap[p].son[0], x);
	else
	{
		int tmp = next(treap[p].son[1], x);
		if (tmp == 0xFFFFFFFF)return treap[p].value;
		return tmp;
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
	int i, tmp_num, tmp_score;
	char flag;
	n = readNum(); m = readNum();
	for (i = 1; i <= n; i++)
	{
		tmp_num = readNum(); tmp_score = readNum();
		stu[tmp_num] = tmp_score;
		insert(root, tmp_score);
	}
	for (i = 1; i <= m; i++)
	{
		flag = getchar();
		if (flag == 'I')
		{
			tmp_num = readNum(); tmp_score = readNum();
			stu[tmp_num] = tmp_score;
			insert(root, tmp_score);
		}
		else if (flag == 'O')
		{
			tmp_num = readNum();
			del(root, stu[tmp_num]);
		}
		else if (flag == 'S')
		{
			tmp_num = readNum();
			printf("%d\n", Rank(root, stu[tmp_num]));
		}
		else if (flag == 'P')
		{
			tmp_num = readNum();
			printf("%d\n", pre(root, stu[tmp_num]));
		}
		else
		{
			tmp_num = readNum();
			printf("%d\n", next(root, stu[tmp_num]));
		}
	}
	return 0;
}