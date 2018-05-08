#pragma once
#include <vector>
#include <random>
#include <chrono>

class SlotGameParams
{
  void countProbabilityTable();
  void make_permutation(int j, double &totalScore, std::vector<int>& tempComb);
public:
  int numOfReels;
  int numOfFruit;
  int numOfCombinations;
  std::vector<int> reelsLength;
  std::vector<std::vector<int>> reelsValue;
  std::vector<std::vector<int>> winTable;
  std::vector<std::vector<int>> tableOfProb;
  SlotGameParams(int _numOfReels, int _numOfFruit);
  void readReelsValue(std::string const &pathReelsValue);
  void readWinTable(std::string const &pathWinTable);
  double theoreticalWin();
  double randomStartsWin(int numOfStarts, unsigned seed = std::chrono::system_clock::now().time_since_epoch().count() );
  double everyStartsWin();
};

int checkLine(std::vector<int> &line);