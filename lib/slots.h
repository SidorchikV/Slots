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
	bool countProbabilityTable();
	void make_permutation(int j, double *totalScore, vector<int>* tempComb);
public:
	int numOfReels;
	int numOfFruit;
	int numOfCombinations;
	vector<int> reelsLength;
	vector<vector<int>> reelsValue;
	vector<vector<double>> winTable;
	vector<vector<double>> tableOfProb;
	SlotGameParams(int _numOfReels, int _numOfFruit);
	bool readReelsValue(string *pathReelsValue);
	bool readWinTable(string *pathWinTable);
	double theoreticalWin();
	double randomStartsWin(int numOfStarts);
	double everyStartsWin();
	int getNumOfReels();
	int getNumOfFruit();
};

int checkLine(vector<int>* line);