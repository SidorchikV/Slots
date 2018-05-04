#include "slots.h"

int numOfReels = 3;
int numOfFruit = 4;
vector<double> wins(numOfReels);
vector<int> tempComb(numOfReels);
vector<int> reelsLength(numOfReels);
vector<vector<int>> reelsValue(numOfReels);
vector<vector<double>> winTable(numOfFruit, vector<double>(numOfReels));
vector<vector<double>> tableOfProb(numOfReels, vector<double>(numOfFruit, 0));
double totalScore = 0;
double probScore = 0;
double theoreticalScore = 0;

void readReelsValue(string* path, vector<int>* reelsLength, vector<vector<int>>* reelsValue)
{
	ifstream fin;
	fin.open(*path);
	if (!fin.is_open())
	{
		cout << "Ошибка открытия файла!" << endl;
	}
	else {
		for (int i = 0; i < numOfReels; i++)
		{
			fin >> (*reelsLength)[i];
			(*reelsValue)[i].resize((*reelsLength)[i]);
		}
		for (int i = 0; i < numOfReels; i++)
		{

			for (int j = 0; j < (*reelsLength)[i]; j++)
			{
				fin >> (*reelsValue)[i][j];
			}
		}
	}
}
void readWinTable(string *path, vector<vector<double>>* winTable)
{
	ifstream fin;
	fin.open(*path);
	if (!fin.is_open())
	{
		cout << "Ошибка открытия файла!" << endl;
	}
	else {
		for (int i = 0; i < numOfFruit; i++)
		{
			for (int j = 0; j < numOfReels; j++)
			{
				fin >> (*winTable)[i][j];
			}
		}
	}
}
int checkLine(vector<int>* line)
{
	for (int i = 1; i < (*line).capacity(); i++)
	{
		if ((*line)[i - 1] != (*line)[i])
			return i;
	}
	return (*line).capacity();
}
void make_permutation(int j)
{
	for (int i = 0; i < reelsLength[j]; i++)
	{
		tempComb[j] = reelsValue[j][i];
		if (j == (numOfReels - 1))
		{
			totalScore += winTable[tempComb[0] - 1][checkLine(&tempComb) - 1];
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
	string pathReels = "../reels.txt";
	string pathWinTable = "../winTable.txt";
	readReelsValue(&pathReels, &reelsLength, &reelsValue);
	readWinTable(&pathWinTable, &winTable);
	hello();
	cout << "Win table:\n";
	for (int i = 0; i < numOfFruit; i++)
	{
		copy(winTable[i].begin(), winTable[i].end(), ostream_iterator<double>(cout, " "));
		cout << endl;
	}cout << "Reels:\n";
	for (int i = 0; i < numOfReels; i++)
	{
		copy(reelsValue[i].begin(), reelsValue[i].end(), ostream_iterator<double>(cout, " "));
		cout << endl;
	}

	int numOfCombinations = 1;
	for (int i = 0; i < numOfReels; i++)
	{
		numOfCombinations *= reelsLength[i];
	}

	// average win of random starts
	for (int i = 0; i < numOfCombinations; i++)
	{
		for (int j = 0; j < numOfReels; j++)
		{
			tempComb[j] = reelsValue[j][(rand()) % reelsLength[j]];
		}
		probScore += winTable[tempComb[0] - 1][checkLine(&tempComb) - 1];
	}
	//the calculation of the average gain from all possible permutations
	make_permutation(0);

	for (int i = 0; i < numOfReels; i++)
	{
		for (int j = 0; j < reelsLength[i]; j++)
		{
			tableOfProb[i][(reelsValue[i][j] - 1)]++;
		}
	}
	cout << "\nProbability table:\n";
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
	//MO
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

	cout << "\nProbabilistic average win : " << (probScore) / (numOfCombinations) << endl;
	cout << "\nReal average win : " << (totalScore) / (numOfCombinations) << endl;
	cout << "\nTheoretical average win : " << theoreticalScore << endl << endl;
	system("pause");
	return 0;
}