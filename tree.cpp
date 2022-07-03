#include<iostream>
#include<algorithm>
#define INT_MAX 0x7FFFFFFF
using namespace std;

int axis;
int left_up[2], left_below[2], right_up[2], right_below[2];

class town_data {
public:
    int data[2];
    bool operator<(const town_data& x) const {
        return data[axis] < x.data[axis];
    }
}town[100001], town_not_change[100001];

class Node {
public:
    int data[2] = { 0,0 };
    int range_x[2];
    int range_y[2];
    int left = -1;
    int right = -1;
}node[100001];

int Build_tree(int l, int r, int ax)
{
    axis = ax;
    int tmp = (l + r) >> 1;
    nth_element(town + l, town + tmp, town + r + 1);
    node[tmp].data[0] = town[tmp].data[0];
    node[tmp].data[1] = town[tmp].data[1];
    node[tmp].range_x[0] = node[tmp].range_x[1] = town[tmp].data[0];
    node[tmp].range_y[0] = node[tmp].range_y[1] = town[tmp].data[1];
    if (l < tmp)
    {
        node[tmp].left = Build_tree(l, tmp - 1, ax ^ 1);
        node[tmp].range_x[0] = min(node[tmp].range_x[0], node[node[tmp].left].range_x[0]);
        node[tmp].range_x[1] = max(node[tmp].range_x[1], node[node[tmp].left].range_x[1]);
        node[tmp].range_y[0] = min(node[tmp].range_y[0], node[node[tmp].left].range_y[0]);
        node[tmp].range_y[1] = max(node[tmp].range_y[1], node[node[tmp].left].range_y[1]);
    }
    if (r > tmp)
    {
        node[tmp].right = Build_tree(tmp + 1, r, ax ^ 1);
        node[tmp].range_x[0] = min(node[tmp].range_x[0], node[node[tmp].right].range_x[0]);
        node[tmp].range_x[1] = max(node[tmp].range_x[1], node[node[tmp].right].range_x[1]);
        node[tmp].range_y[0] = min(node[tmp].range_y[0], node[node[tmp].right].range_y[0]);
        node[tmp].range_y[1] = max(node[tmp].range_y[1], node[node[tmp].right].range_y[1]);
    }
    return tmp;
}

int search_max(int tmp, int x, int y)
{
    int maxx = abs(node[tmp].data[0] - x) + abs(node[tmp].data[1] - y);
    if (node[tmp].left == -1 && node[tmp].right == -1)return maxx;
    if (node[tmp].left == -1)
    {
        int maxx_right = max(abs(node[node[tmp].right].range_x[0] - x), abs(node[node[tmp].right].range_x[1] - x)) + max(abs(node[node[tmp].right].range_y[0] - y), abs(node[node[tmp].right].range_y[1] - y));
        if (maxx >= maxx_right)return maxx;
        return max(maxx, search_max(node[tmp].right, x, y));
    }
    if (node[tmp].right == -1)
    {
        int maxx_left = max(abs(node[node[tmp].left].range_x[0] - x), abs(node[node[tmp].left].range_x[1] - x)) + max(abs(node[node[tmp].left].range_y[0] - y), abs(node[node[tmp].left].range_y[1] - y));
        if (maxx >= maxx_left)return maxx;
        return max(maxx, search_max(node[tmp].left, x, y));
    }
    int maxx_left = max(abs(node[node[tmp].left].range_x[0] - x), abs(node[node[tmp].left].range_x[1] - x)) + max(abs(node[node[tmp].left].range_y[0] - y), abs(node[node[tmp].left].range_y[1] - y));
    int maxx_right = max(abs(node[node[tmp].right].range_x[0] - x), abs(node[node[tmp].right].range_x[1] - x)) + max(abs(node[node[tmp].right].range_y[0] - y), abs(node[node[tmp].right].range_y[1] - y));
    if (maxx >= maxx_left && maxx >= maxx_right)return maxx;
    else if (maxx >= maxx_left)return max(maxx, search_max(node[tmp].right, x, y));
    else if (maxx >= maxx_right)return max(maxx, search_max(node[tmp].left, x, y));
    else
    {
        if (maxx_left > maxx_right)
        {
            int cur_max = search_max(node[tmp].left, x, y);
            if (cur_max >= maxx_right)return max(cur_max, maxx);
            int temp = max(cur_max, search_max(node[tmp].right, x, y));
            return max(temp, maxx);
        }
        else
        {
            int cur_max = search_max(node[tmp].right, x, y);
            if (cur_max >= maxx_left)return max(cur_max, maxx);
            int temp = max(cur_max, search_max(node[tmp].left, x, y));
            return max(temp, maxx);
        }
    }
}

int search_min(int tmp, int x, int y)
{
    int minn = abs(node[tmp].data[0] - x) + abs(node[tmp].data[1] - y);
    if (minn == 0)minn = INT_MAX;
    if (node[tmp].left == -1 && node[tmp].right == -1)return minn;
    if (node[tmp].left == -1)
    {
        int minn_right = max(node[node[tmp].right].range_x[0] - x, 0) + max(x - node[node[tmp].right].range_x[1], 0) + max(node[node[tmp].right].range_y[0] - y, 0) + max(y - node[node[tmp].right].range_y[1], 0);
        if (minn <= minn_right)return minn;
        return min(minn, search_min(node[tmp].right, x, y));
    }
    if (node[tmp].right == -1)
    {
        int minn_left = max(node[node[tmp].left].range_x[0] - x, 0) + max(x - node[node[tmp].left].range_x[1], 0) + max(node[node[tmp].left].range_y[0] - y, 0) + max(y - node[node[tmp].left].range_y[1], 0);
        if (minn <= minn_left)return minn;
        return min(minn, search_min(node[tmp].left, x, y));
    }
    int minn_left = max(node[node[tmp].left].range_x[0] - x, 0) + max(x - node[node[tmp].left].range_x[1], 0) + max(node[node[tmp].left].range_y[0] - y, 0) + max(y - node[node[tmp].left].range_y[1], 0);
    int minn_right = max(node[node[tmp].right].range_x[0] - x, 0) + max(x - node[node[tmp].right].range_x[1], 0) + max(node[node[tmp].right].range_y[0] - y, 0) + max(y - node[node[tmp].right].range_y[1], 0);
    if (minn <= minn_left && minn <= minn_right)return minn;
    else if (minn <= minn_left)return min(minn, search_min(node[tmp].right, x, y));
    else if (minn <= minn_right)return min(minn, search_min(node[tmp].left, x, y));
    else
    {
        if (minn_left < minn_right)
        {
            int cur_min = search_min(node[tmp].left, x, y);
            if (cur_min <= minn_right)return min(minn, cur_min);
            int temp = min(cur_min, search_min(node[tmp].right, x, y));
            return min(minn, temp);
        }
        else
        {
            int cur_min = search_min(node[tmp].right, x, y);
            if (cur_min <= minn_left)return min(minn, cur_min);
            int temp = min(cur_min, search_min(node[tmp].left, x, y));
            return min(minn, temp);
        }
    }
}


int readNum()
{
    char ch = getchar();
    while (ch < '0' || ch > '9') ch = getchar();
    int v = 0;
    while (ch >= '0' && ch <= '9') {
        v = v * 10 + ch - '0';
        ch = getchar();
    }
    return v;
};


int main()
{
    int T, n, i, x, y, minn, maxx;
    int diff = INT_MAX, ans = 0;
    T = readNum();
    while (T--)
    {
        int maxcross1 = 0x80000000, maxcross2 = 0x80000000;
        int mincross1 = INT_MAX, mincross2 = INT_MAX;
        n = readNum();
        diff = INT_MAX;
        for (i = 0; i < n; i++)
        {
            town[i].data[0] = x = readNum();
            town[i].data[1] = y = readNum();
            town_not_change[i].data[0] = town[i].data[0];
            town_not_change[i].data[1] = town[i].data[1];
            if (y - x > maxcross1)
            {
                maxcross1 = y - x;
                left_up[0] = x;
                left_up[1] = y;
            }
            if (x + y < mincross2)
            {
                mincross2 = x + y;
                left_below[0] = x;
                left_below[1] = y;
            }
            if (y - x < mincross1)
            {
                mincross1 = y - x;
                right_below[0] = x;
                right_below[1] = y;
            }
            if (x + y > maxcross2)
            {
                maxcross2 = x + y;
                right_up[0] = x;
                right_up[1] = y;
            }
        }
        int root = Build_tree(0, n - 1, 0);
        for (i = 0; i < n; i++)
        {
            x = town_not_change[i].data[0];
            y = town_not_change[i].data[1];
            minn = search_min(root, x, y);
            //maxx = search_max(root, x, y);
            maxx = abs(x - left_up[0]) + abs(y - left_up[1]);
            maxx = max(maxx, abs(x - left_below[0]) + abs(y - left_below[1]));
            maxx = max(maxx, abs(x - right_up[0]) + abs(y - right_up[1]));
            maxx = max(maxx, abs(x - right_below[0]) + abs(y - right_below[1]));
            if (maxx - minn < diff)
            {
                diff = maxx - minn;
                ans = i + 1;
            }
        }
        printf("%d\n", ans);
    }
    return 0;
}