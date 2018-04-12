#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <ctime>

using namespace std;
int numOfReels = 3;
int numOfFruit = 4;
vector<int> reelsLength(numOfReels);
vector<vector<int>> reelsValue(numOfReels);
vector<vector<double>> winTable(numOfFruit, vector<double>(numOfReels));
vector<vector<double>> tableOfProb(numOfReels, vector<double>(numOfFruit, 0));
vector<double> wins(numOfReels);
vector<int> tempComb(numOfReels);
string pathReels = "reels.txt";
string pathWinTable = "winTable.txt";
double totalScore = 0;
double probScore = 0;
double theoreticalScore = 0;

void readFromFile(string path)
{
	ifstream fin;
	fin.open(path);
	if (!fin.is_open())
	{
		cout << "Ошибка открытия файла!" << endl;
	}
	else {
		for (int i = 0; i < numOfReels; i++)
		{
			fin >> reelsLength[i];
			reelsValue[i].resize(reelsLength[i]);
		}
		for (int i = 0; i < numOfReels; i++)
		{

			for (int j = 0; j < reelsLength[i]; j++)
			{
				fin >> reelsValue[i][j];
			}
		}
	}
}
void readWinTable(string path)
{
	ifstream fin;
	fin.open(path);
	if (!fin.is_open())
	{
		cout << "Ошибка открытия файла!" << endl;
	}
	else {
		for (int i = 0; i < numOfFruit; i++)
		{
			for (int j = 0; j < numOfReels; j++)
			{
				fin >> winTable[i][j];
			}
		}
	}
}
int check(vector<int> vec)
{
	for (int i = 1; i < vec.capacity(); i++)
	{
		if (vec[i - 1] != vec[i])
			return i;
	}
	return vec.capacity();
}
void make_permutation(int j)
{
	for (int i = 0; i < reelsLength[j]; i++)
	{
		tempComb[j] = reelsValue[j][i];
		if (j == (numOfReels - 1))
		{
			totalScore += winTable[tempComb[0] - 1][check(tempComb) - 1];
		}
		else
		{
			make_permutation(j + 1);
		}
	}
}
int main()
{
	setlocale(LC_ALL, "ru");
	srand(time(NULL));
	readFromFile(pathReels);
	readWinTable(pathWinTable);

	//Вывод матрицы выигр.
	cout << "Матрица выигрышей:\n";
	for (int i = 0; i < numOfFruit; i++)
	{
		copy(winTable[i].begin(), winTable[i].end(), ostream_iterator<double>(cout, " "));
		cout << endl;
	}

	int numOfCombinations = 1;
	for (int i = 0; i < numOfReels; i++)
	{
		numOfCombinations *= reelsLength[i];
	}

	// средний выигрыш случайных запусков
	for (int i = 0; i < numOfCombinations; i++)
	{
		for (int j = 0; j < numOfReels; j++)
		{
			tempComb[j] = reelsValue[j][(rand()) % reelsLength[j]];
		}
		probScore += winTable[tempComb[0] - 1][check(tempComb) - 1];
	}
	// подсчет среднего выигрыша от всевозможных перестановок
	make_permutation(0);

	for (int i = 0; i < numOfReels; i++)
	{
		for (int j = 0; j < reelsLength[i]; j++)
		{
			tableOfProb[i][(reelsValue[i][j] - 1)]++;
		}
	}
	cout << "\nМатрица вероятностей:\n";
	for (int i = 0; i < numOfReels; i++)
	{
		for (int j = 0; j < numOfFruit; j++)
		{
			tableOfProb[i][j] /= reelsLength[i];
			cout << tableOfProb[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
	//вычисление мо выигрыша
	for (int f = 0; f < numOfFruit; f++)
	{
		wins = winTable[f];
		for (int i = 0; i < numOfReels; i++)
		{
			//cout << wins[i] << "::";
			for (int k = 0; k <= i; k++)
			{
				wins[i] *= tableOfProb[k][f];
			}
			if (i + 1 < numOfReels)
			{
				wins[i] *= (1 - tableOfProb[i + 1][f]);
			}
			//cout << wins[i] << " ";
			theoreticalScore += wins[i];
		}
		//cout << endl;
	}

	cout << "\nprobScore : " << (probScore) / (numOfCombinations) << endl;
	cout << "\ntotalScore : " << (totalScore) / (numOfCombinations) << endl;
	cout << "\ntheoreticalScore : " << theoreticalScore << endl << endl;
	system("pause");
	return 0;
}