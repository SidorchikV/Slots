#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <ctime>

using namespace std;

class SlotGameParams
{
public:
	int numOfReels;
	int numOfFruit;

	vector<int> reelsLength;
	vector<vector<int>> reelsValue;
	vector<vector<double>> winTable;
	vector<vector<double>> tableOfProb;

	SlotGameParams(int _numOfReels, int _numOfFruit);
	bool readReelsValue(string *pathReelsValue);
	bool readWinTable(string *pathWinTable);
	int getNumOfReels();
	int getNumOfFruit();
};