#include<iostream>
#include<stack>
#include<string>
#include<algorithm>
#define Maxx(a,b) (a>b?a:b)
using namespace std;

stack<int> Sd;
stack<char> Sr;

class matrix{
public:
	int row, col;
	int** mat;
	matrix(int x = 1, int y = 1,int num = 1) {
		row = x;
		col = y;
		mat = new int* [row];
		for (int i = 0; i < row; i++)
			mat[i] = new int[col];
		for (int i = 0; i < row; i++)
			for (int j = 0; j < col; j++)
				mat[i][j] = num;
	}
	~matrix() {
		for (int i = 0; i < row; i++)
			delete[] mat[i];
		delete[] mat;
	}
	matrix(const matrix& mat2) {
		row = mat2.row;
		col = mat2.col;
		mat = new int* [row];
		for (int i = 0; i < row; i++)
			mat[i] = new int[col];
		for (int i = 0; i < row; i++)
			for (int j = 0; j < col; j++)
				mat[i][j] = mat2.mat[i][j];
	}
	matrix& operator=(const matrix& mat2) {
		this->row = mat2.row;
		this->col = mat2.col;
		this->mat = new int* [row];
		for (int i = 0; i < row; i++)
			this->mat[i] = new int[col];
		for (int i = 0; i < row; i++)
			for (int j = 0; j < col; j++)
				this->mat[i][j] = mat2.mat[i][j];
		return *this;
	}
	matrix& operator+(const matrix& mat2) {
		matrix *tmp=new matrix(Maxx(row, mat2.row), Maxx(col, mat2.col), 0);
		if (row == 1 && col == 1)
		{
			for (int i = 0; i < tmp->row; i++)
				for (int j = 0; j < tmp->col; j++)
					tmp->mat[i][j] = mat[0][0] + mat2.mat[i][j];
		}
		else if (mat2.row == 1 && mat2.col == 1)
		{
			for (int i = 0; i < tmp->row; i++)
				for (int j = 0; j < tmp->col; j++)
					tmp->mat[i][j] = mat[i][j] + mat2.mat[0][0];
		}
		else
		{
			for (int i = 0; i < tmp->row; i++)
				for (int j = 0; j < tmp->col; j++)
					tmp->mat[i][j] = mat[i][j] + mat2.mat[i][j];
		}
		return *tmp;
	}
	matrix& operator*(const matrix& mat2) {
		matrix* tmp = new matrix(Maxx(row, mat2.row), Maxx(col, mat2.col), 0);
		if (row == 1 && col == 1)
		{
			for (int i = 0; i < tmp->row; i++)
				for (int j = 0; j < tmp->col; j++)
					tmp->mat[i][j] = mat[0][0] * mat2.mat[i][j];
		}
		else if (mat2.row == 1 && mat2.col == 1)
		{
			for (int i = 0; i < tmp->row; i++)
				for (int j = 0; j < tmp->col; j++)
					tmp->mat[i][j] = mat[i][j] * mat2.mat[0][0];
		}
		else
		{
			for (int i = 0; i < tmp->row; i++)
				for (int j = 0; j < tmp->col; j++)
					tmp->mat[i][j] = mat[i][j] * mat2.mat[i][j];
		}
		return *tmp;
	}
	void print() {
		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < col; j++)
				cout << mat[i][j]<<" ";
			//cout << endl;
		}		
	}
};

stack<matrix>Sm;

matrix& join(const matrix& a, const matrix& b, int axis = 0) {
	if (axis == 0)
	{
		int index = 0;
		matrix* tmp = new matrix(a.row + b.row, a.col, 0);
		for (int i = 0; i < a.row; i++)
		{
			for (int j = 0; j < a.col; j++)
			{
				tmp->mat[index][j] = a.mat[i][j];
			}
			index++;
		}
		for (int i = 0; i < b.row; i++)
		{
			for (int j = 0; j < a.col; j++)
			{
				tmp->mat[index][j] = b.mat[i][j];
			}
			index++;
		}
		return *tmp;
	}
	else
	{
		matrix* tmp = new matrix(a.row, a.col + b.col, 0);
		for (int i = 0; i < a.row; i++)
		{
			int index = 0;
			for (int j = 0; j < a.col; j++)
			{
				tmp->mat[i][index] = a.mat[i][j];
				index++;
			}
			for (int j = 0; j < b.col; j++)
			{
				tmp->mat[i][index] = b.mat[i][j];
				index++;
			}
		}
		return *tmp;
	}
}

void cal()
{
	int x, y;
	char tmp;
	x = Sd.top();
	Sd.pop();
	y = Sd.top();
	Sd.pop();
	tmp = Sr.top();
	Sr.pop();
	if (tmp == '+')
	{
		Sd.push(x + y);
		//cout << x << "+" << y << "=" << x + y << endl;
	}
	else if (tmp == '*')
	{
		Sd.push(x * y);
		//cout << x << "*" << y << "=" << x * y << endl;
	}
}

void cal_mat(int axis = 0)
{
	matrix x, y;
	char tmp;
	x = Sm.top();
	Sm.pop();
	y = Sm.top();
	Sm.pop();
	tmp = Sr.top();
	Sr.pop();
	if (tmp == '+')
	{
		Sm.push(x + y);
		//cout << x << "+" << y << "=" << x + y << endl;
	}
	else if (tmp == '*')
	{
		Sm.push(x * y);
		//cout << x << "*" << y << "=" << x * y << endl;
	}
	else if (tmp == '@')
	{
		Sm.push(join(y, x, axis));
	}
}

void pro_num(string s)
{
	int i = 0;
	while (i < s.length())
	{
		char tmp = s[i];
		int ans = 0, flag = 0;
		while (tmp >= 48 && tmp <= 57)
		{
			ans = ans * 10 + (tmp - 48);
			tmp = s[++i];
			if (!flag)flag = 1;
		}
		if (flag)Sd.push(ans);
		else
		{
			if (tmp == '+')
			{
				while (Sr.top() == '+' || Sr.top() == '*')
					cal();
				Sr.push('+');
			}
			else if (tmp == '*')
			{
				while (Sr.top() == '*')
					cal();
				Sr.push('*');
			}
			else if (tmp == '(')
			{
				Sr.push(tmp);
			}
			else if (tmp == ')')
			{
				while (Sr.top() != '(')
					cal();
				Sr.pop();
			}
			else
			{
				while (Sr.top() != ';')
					cal();
			}
			i++;
		}
	}
	cout << Sd.top();
}

bool checkones(string s, int index)
{
	int i = index, flag = 1;
	string tmp = "np.ones";
	for (int j = 0; j < 7; j++)
	{
		if (s[i] != tmp[j])return 0;
		i++;
	}
	return flag;
}

bool checkcon(string s, int index)
{
	int i = index, flag = 1;
	string tmp = "np.concatenate";
	for (int j = 0; j < 14; j++)
	{
		if (s[i] != tmp[j])return 0;
		i++;
	}
	return flag;
}

void pro_mat(string s)
{
	int i = 0;
	while (i < s.length())
	{
		char tmp = s[i];
		int ans = 0, flag = 0;
		while (tmp >= 48 && tmp <= 57)
		{
			ans = ans * 10 + (tmp - 48);
			tmp = s[++i];
			if (!flag)flag = 1;
		}
		if (flag)
		{
			matrix num(1, 1, ans);
			Sm.push(num);
		}
		else
		{
			if (s[i]=='n' && checkones(s,i) == 1)
			{
				int row = 0, col = 0;
				if (s[i + 8] == '(')
				{
					i += 9;
					tmp = s[i];
					while (tmp >= 48 && tmp <= 57)
					{
						row = row * 10 + (tmp - 48);
						tmp = s[++i];
					}
					i++;
					tmp = s[i];
					while (tmp >= 48 && tmp <= 57)
					{
						col = col * 10 + (tmp - 48);
						tmp = s[++i];
					}
					i += 2;
				}
				else
				{
					i += 8;
					tmp = s[i];
					while (tmp >= 48 && tmp <= 57)
					{
						row = row * 10 + (tmp - 48);
						tmp = s[++i];
					}
					if (s[i] == ')')
					{
						i++;
						col = 1;
					}
					else
					{
						i++;
						tmp = s[i];
						while (tmp >= 48 && tmp <= 57)
						{
							col = col * 10 + (tmp - 48);
							tmp = s[++i];
						}
						i++;
					}
				}
				matrix tmp1(row, col, 1);
				Sm.push(tmp1);
			}
			else if (s[i] == 'n' && checkcon(s, i) == 1)
			{
				Sr.push('@');
				i += 16;
			}
			else if (s[i] == ',')
			{
				while (Sr.top()!='@')
					cal_mat();
				Sr.push(',');
				i++;
			}
			else if (s[i] == ']')
			{
				int axis = 0;
				if (s[i + 1] == ')')
				{
					axis = 0;
					i += 2;
				}
				else if (s[i + 7] == '0')
				{
					axis = 0;
					i += 9;
				}
				else
				{
					axis = 1;
					i += 9;
				}
				while (Sr.top() != ',')
				{
					cal_mat(axis);
				}
				Sr.pop();
				cal_mat(axis);
			}
			else if (tmp == '+')
			{
				while (Sr.top() == '+' || Sr.top() == '*')
					cal_mat();
				Sr.push('+');
				i++;
			}
			else if (tmp == '*')
			{
				while (Sr.top() == '*')
					cal_mat();
				Sr.push('*');
				i++;
			}
			else if (tmp == '(')
			{
				Sr.push(tmp);
				i++;
			}
			else if (tmp == ')')
			{
				while (Sr.top() != '(')
					cal_mat();
				Sr.pop();
				i++;
			}
			else if(tmp == ';')
			{
				while (Sr.top() != ';')
					cal_mat();
				i++;
			}
			else {
				i++;
			}
		}
	}
	Sm.top().print();
}

int main()
{
	string s;
	cin >> s;
	s = s + ';';
	Sr.push(';');
	if (s.find("np") == -1)
		pro_num(s);
	else
		pro_mat(s);
	return 0;
}