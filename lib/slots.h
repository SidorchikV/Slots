#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <ctime>

#define ERROR_OPEN_REELS_FAILED 10001
#define ERROR_OPEN_WIN_TABLE_FAILED 10002

using namespace std;

class SlotGameParams
{
  bool countProbabilityTable();
  void make_permutation(int j, double &totalScore, vector<int>& tempComb);
public:
  int numOfReels;
  int numOfFruit;
  int numOfCombinations;
  vector<int> reelsLength;
  vector<vector<int>> reelsValue;
  vector<vector<double>> winTable;
  vector<vector<double>> tableOfProb;
  SlotGameParams(int _numOfReels, int _numOfFruit);
  void readReelsValue(string const &pathReelsValue);
  void readWinTable(string const &pathWinTable);
  double theoreticalWin();
  double randomStartsWin(int numOfStarts);
  double everyStartsWin();
};

int checkLine(vector<int> &line);