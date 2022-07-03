#include<iostream>
using namespace std;

int list[200001][3];//list[][0]ÊÇ·ñÔÚ¶ÓÁĞ [1]×ó±ß [2]ÓÒ±ß
int head, tail, len = 0;

void Insert(int num1,int num2,int judge);
void Out(int num1);
void Swap(int num1, int num2);
void print_a();
void print_b();

int main() {
	ios::sync_with_stdio(false);
	int n, m, k, l;
	int flag, num1, num2, judge, reverse_true = 0;
	cin >> n >> m >> k >> l;
	int sum = n + m + k + l;
	while (sum--) {
		cin >> flag;
		if (flag == 1) {
			cin >> num1 >> num2 >> judge;
			if (reverse_true == 0)
				Insert(num1, num2, judge);
			else
				Insert(num1, num2, 1 - judge);
		}
		else if (flag == 0) {
			cin >> num1;
			Out(num1);
		}
		else if (flag == -1) {
			cin >> num1 >> num2;
			Swap(num1, num2);
		}
		else {
			reverse_true = 1 - reverse_true;
		}
	}
	if (reverse_true == 0)
		print_a();
	else
		print_b();
	return 0;
}

void Insert(int num1, int num2, int judge) {
	if (len == 0)
	{
		list[num1][0] = 1;
		head = num1;
		tail = num1;
		list[num1][1] = num1;
		list[num1][2] = num1;
		len++;
	}
	else
	{
		list[num1][0] = 1;
		if (judge == 0)
		{
			list[list[num2][1]][2] = num1;
			list[num1][1] = list[num2][1];
			list[num1][2] = num2;
			list[num2][1] = num1;
			if (head == num2)head = num1;
			len++;
		}
		else
		{
			list[list[num2][2]][1] = num1;
			list[num1][2] = list[num2][2];
			list[num1][1] = num2;
			list[num2][2] = num1;
			if (tail == num2)tail = num1;
			len++;
		}
	}
}

void Out(int num1)
{
	if (head == num1)head = list[num1][2];
	if (tail == num1)tail = list[num1][1];
	list[list[num1][1]][2] = list[num1][2];
	list[list[num1][2]][1] = list[num1][1];
	list[num1][0] = 0;
	len--;
}

void Swap(int num1, int num2)
{
	if (head == num1)head = num2;
	else if (head == num2)head = num1;
	if (tail == num1)tail = num2;
	else if (tail == num2)tail = num1;
	int left = list[num1][1];
	int right = list[num1][2];
	if (list[num2][1] != num1)
	{
		list[list[num2][1]][2] = num1;
		list[num1][1] = list[num2][1];
	}
	else
	{
		list[num1][1] = num2;
	}
	if (list[num2][2] != num1)
	{
		list[list[num2][2]][1] = num1;
		list[num1][2] = list[num2][2];
	}
	else
	{
		list[num1][2] = num2;
	}

	if (left != num2)
	{
		list[left][2] = num2;
		list[num2][1] = left;
	}
	else
	{
		list[num2][1] = num1;
	}
	if (right != num2)
	{
		list[right][1] = num2;
		list[num2][2] = right;
	}
	else
	{
		list[num2][2] = num1;
	}
}

void print_a()
{
	int index = head, cnt = 0;
	for (cnt = 0; cnt < len; cnt++)
	{
		cout << index << " ";
		index = list[index][2];
	}
	cout << "-1" << endl;
}

void print_b()
{
	int index = tail, cnt = 0;
	for (cnt = 0; cnt < len; cnt++)
	{
		cout << index << " ";
		index = list[index][1];
	}
	cout << "-1" << endl;
}