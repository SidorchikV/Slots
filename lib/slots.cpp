
#include "slots.h"

SlotGameParams::SlotGameParams(int _numOfReels, int _numOfFruit)
{
	numOfReels = _numOfReels;
	numOfFruit = _numOfFruit;
	reelsLength = vector<int>(_numOfReels);
	reelsValue = vector<vector<int>>(_numOfReels);
	winTable = vector<vector<double>>(_numOfFruit, vector<double>(_numOfReels));
	tableOfProb = vector<vector<double>>(_numOfReels, vector<double>(_numOfFruit, 0));
}

bool SlotGameParams::readReelsValue(string * pathReelsValue)
{
	ifstream fin;
	fin.open(*pathReelsValue);
	if (!fin.is_open())
	{
		cout << "Ошибка открытия файла!" << endl;
		return false;
	}
	else {
		for (int i = 0; i < numOfReels; i++)
		{
			fin >> (reelsLength)[i];
			(reelsValue)[i].resize((reelsLength)[i]);
		}
		for (int i = 0; i < numOfReels; i++)
		{

			for (int j = 0; j < (reelsLength)[i]; j++)
			{
				fin >> (reelsValue)[i][j];
			}
		}
	}
	return true;
}

bool SlotGameParams::readWinTable(string * pathWinTable)
{
	ifstream fin;
	fin.open(*pathWinTable);
	if (!fin.is_open())
	{
		cout << "Ошибка открытия файла!" << endl;
		return false;
	}
	else {
		for (int i = 0; i < numOfFruit; i++)
		{
			for (int j = 0; j < numOfReels; j++)
			{
				fin >> (winTable)[i][j];
			}
		}
	}
	return true;
}

int SlotGameParams::getNumOfReels()
{
	return numOfReels;
}

int SlotGameParams::getNumOfFruit()
{
	return numOfFruit;
}
