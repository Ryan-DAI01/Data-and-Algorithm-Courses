#include<iostream>
#include<cstdio>
using namespace std;

int** Init_matrix(int size) {
	int** mat = new int* [size];
	for (int i = 0; i < size; i++)
		mat[i] = new int[size] {};
	return mat;
}

void Destroy_matrix(int** mat, int size) {
	for (int i = 0; i < size; i++)
		delete[] mat[i];
	delete[] mat;
}

void Read_matrix(int** mat, int size) {
	int i, j;
	char tmp;
	for(i=0;i<size;i++)
		for (j = 0; j < size; j++)
		{
			cin >> tmp;
			mat[i][j] = int(tmp - 97);
		}
}

void Matrix_mul(int** mat1, int** mat2, int** ans, int size) {
	int i, j, k, r;
	for (i = 0; i < size; i++)
		for (k = 0; k < size; k++)
		{
			r = mat1[i][k];
			if (r == 0)continue;
			for (j = 0; j < size; j++)
				ans[i][j] += r * mat2[k][j];
			ans[i][k] %= 26;
		}
}

int main() {
	ios::sync_with_stdio(false);
	int num, size;
	cin >> num;
	while (num--) {
		int flag = 1;
		cin >> size;
		int** mat1 = Init_matrix(size);
		int** mat2 = Init_matrix(size);
		int** mat3 = Init_matrix(size);
		int** ans = Init_matrix(size);
		Read_matrix(mat1, size);
		Read_matrix(mat2, size);
		Read_matrix(mat3, size);
		Matrix_mul(mat1, mat2, ans, size);
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
				if ((ans[i][j] % 26) != mat3[i][j])flag = 0;
		if (flag)cout << "yes" << endl;
		else cout << "no" << endl;
		Destroy_matrix(mat1, size);
		Destroy_matrix(mat2, size);
		Destroy_matrix(mat3, size);
	}
	return 0;
}