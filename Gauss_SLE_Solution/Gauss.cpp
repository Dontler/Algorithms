#include "stdafx.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

double **a; //Матрица
int m, n; //Столбцы и Строки
int *indexes;
int flag = 0;
int solCount = 0;

void MatrixCreate(const int, const int); //Создание матрицы
void MatrixDelete(const int, const int); //Очистка матрицы
void MatrixEnter(); //Получение матрицы с клавиатуры
void FileMatrixEnter(); //Получение матрицы из файла
void CheckConsistency(); //Проверка на совместность
void Gauss(); //Метод Гаусса
void ColumnClearing(int); //Обнуление столбца
int NonZeroLine(int); //Поиск ненулевой строки
int NonZeroRow(int); //Поиск ненулевого столбца
void LineSwap(int, int); //Меняет строки местами
void RowSwap(int, int);
void LineSubtraction(int, int, double); //Вычетание из строки
void UnitReceive(int, double); //Получение единиц на диагонали
void PrintSolution(); //Напечатать решение
void SpecialSolution();
void PrintMatrix();

int main()
{
	//MatrixEnter();
	FileMatrixEnter();
	PrintMatrix();
	Gauss();
	PrintSolution();
	MatrixDelete(m, n + 1);
    return 0;
}

void MatrixCreate(const int rows, const int cols) {
	a = new double*[rows];
	for (int i = 0; i < rows; i++) {
		a[i] = new double[cols];
	}
}

void MatrixDelete(const int rows, const int cols) {
	for (int i = 0; i < rows; i++) {
		delete[] a[i];
	}
	delete[] a;
}

void MatrixEnter() {
	std::cout << "Enter the number of rows: ";
	std::cin >> m;
	std::cout << "Enter the number of columns: ";
	std::cin >> n;
	MatrixCreate(m, n + 1);
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n + 1; j++) {
			std::cout << "Enter a[" << i << "][" << j << "]: ";
			std::cin >> a[i][j];
			std::cout << std::endl;
		}
	}
}

void FileMatrixEnter() {
	ifstream inFile("Matrix.txt");
	if (!inFile) {
		std::cout << "File not found!";
		exit(404);
	}
	char* test = new char[1024];
	string size = "";
	string row = "";
	getline(inFile, size);
	istringstream iss(size, istringstream::in);
	iss >> m >> n;
	MatrixCreate(m, n + 1);
	solCount = n > m ? m : n;
	for (int i = 0; i < m; i++) {
		getline(inFile, row);
		istringstream iss(row, istringstream::in);
		for (int j = 0; j < n + 1; j++) {
			iss >> a[i][j];
		}
		if (inFile.eof() && i != m - 1) {
			std::cout << "Worst input!\n";
			exit(3);
		}
	}
	inFile.close();
	indexes = new int[n];
	for (int i = 0; i < n; i++) {
		indexes[i] = i;
	}
}

void ColumnClearing(int k) {
	int ind;
	if (a[k][k] == 0) {
		ind = NonZeroLine(k);
		if (ind == m) {
			flag = 1;
		}
		else {
			LineSwap(k, ind);
			flag = 0;
			for (int i = 0; i < m; i++) {
				if (i != k) {
					double mult = a[i][k] / a[k][k];
					LineSubtraction(k, i, mult);
				}
			}

		}
	}
	else {
		for (int i = 0; i < m; i++) {
			if (i != k) {
				double mult = a[i][k] / a[k][k];
				LineSubtraction(k, i, mult);
			}
		}
	}
	if (flag == 1) {
		ind = NonZeroRow(k);
		if (ind == k) {
			return;
		}
		else {
			//solCount--;
			RowSwap(k, ind);
			flag = 0;
			for (int i = 0; i < m; i++) {
				if (i != k) {
					double mult = a[i][k] / a[k][k];
					LineSubtraction(k, i, mult);
				}
			}
		}
	}
}

int NonZeroLine(int k) {
	int i = 0;
	for (i = k; i < m && a[i][k] == 0; i++) {
		;
	}
	return i;
}

int NonZeroRow(int k) {
	int i = n - 1, j = k;
	for (i = n - 1; i > k; i--) {
		for (j = k; j < m && a[j][i] == 0; j++) {
			;
		}
		if (j != m) {
			LineSwap(k, j);
			return i;
		}
		if (i - 1 != k)
			solCount--;
	}
	solCount--;
	return i;
}

void RowSwap(int k, int g) {
	for (int i = 0; i < m; i++) {
		double temp = a[i][k];
		a[i][k] = a[i][g];
		a[i][g] = temp;
	}
	int temp = indexes[k];
	indexes[k] = indexes[g];
	indexes[g] = temp;
}

void LineSwap(int k, int g) {
	for (int i = 0; i < n + 1; i++) {
		double temp = a[k][i];
		a[k][i] = a[g][i];
		a[g][i] = temp;
	}
}

void LineSubtraction(int k, int g, double mult) {
	for (int i = k; i < n + 1; i++) {
		a[g][i] -= a[k][i] * mult;
	}
}

void UnitReceive(int k, double div) {
	if (div == 0) return;
	for (int i = 0; i < n + 1; i++) {
		a[k][i] /= div;
	}
}

void CheckConsistency() {
	double sum = 0;
	int count = 0;
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			sum += a[i][j];
		}
		if (sum == 0 && a[i][n] != 0) {
			flag = 3;
			return;
		}
		else if (sum == 0 && a[i][n] == 0) {
			count++;
		}
		sum = 0;
	}
	if (count == m) flag = 2;
	else flag = 0;
}

void Gauss() {
	int num = n > m ? m : n;
	for (int i = 0; i < num; i++) {
		cout << endl;
		ColumnClearing(i);
		if (flag == 1) {
			CheckConsistency();
			if (flag == 3 || flag == 2) return;
			break;
		}
		PrintMatrix();
	}

	for (int i = 0; i < num; i++) {
		UnitReceive(i, a[i][i]);
	}
}

void SpecialSolution() {
	
}

void PrintSolution() {
	cout << endl;
	int num = n > m ? m : n;
	if (n == 1 && m != 1) {
		double sum = 0;
		for (int i = 0; i < m; i++) {
			for (int j = 0; j < n + 1; j++) {
				sum += a[i][j];
			}
			if (sum != a[i][n]) {
				cout << "No solutions!" << endl;
				return;
			}
		}
	}
	else if (n < m) {
		CheckConsistency();
	}

	if (flag == 3) {
		cout << "No solutions!" << endl;
		return;
	}
	else if (flag == 2) {
		for (int i = 0; i < n; i++) {
			cout << "X" << indexes[i] + 1 << " = any;" << endl;
		}
		cout << endl;
		return;
	}
	else {
		for (int i = 0; i < num; i++) {
			if (a[i][i] != 0) {
				cout << "X" << indexes[i] + 1 << " = ";
				for (int j = i + 1; j < n + 1; j++) {
					if (a[i][j] == 0 && j != n) continue;
					else if (a[i][j] > 0 && j != n) {
						cout << -a[i][j] << "X" << indexes[j] + 1;
					}
					else if (a[i][j] < 0 && j != n) {
						cout << " + " << a[i][j] << "X" << indexes[j] + 1;
					}
					else if (a[i][j] >= 0) cout << " + " << a[i][j];
					else if (a[i][j] < 0) cout << a[i][j];
				}
			}
			else {
				cout << "X" << indexes[i] + 1 << " = any;";
			}
			cout << endl;
		}
		for (int i = num; i < n; i++) {
			cout << "X" << indexes[i] + 1 << " = any;" << endl;
		}
	}
}

void PrintMatrix() {
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n + 1; j++) {
			cout << a[i][j] << " ";
		}
		cout << endl;
	}
}
