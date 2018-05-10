#pragma once
#include <vector>
#include <random>
#include <chrono>

class SlotGameParams
{
  size_t numOfReels;
  size_t numOfFruit;
  size_t numOfCombinations;
  std::vector<size_t> reelsLength;
  std::vector<std::vector<size_t>> reelsValue;
  std::vector<std::vector<size_t>> winTable;
  std::vector<std::vector<size_t>> tableOfProb;
  void countProbabilityTable();
  void make_permutation(size_t j, size_t &totalScore, std::vector<size_t>& tempComb);
public:
  SlotGameParams(size_t _numOfReels, size_t _numOfFruit);
  void readReelsValue(std::string const &pathReelsValue);
  void readWinTable(std::string const &pathWinTable);
  double theoreticalWin();
  double randomStartsWin(size_t numOfStarts, unsigned seed = std::chrono::system_clock::now().time_since_epoch().count() );
  double everyStartsWin();
};

size_t checkLine(std::vector<size_t> const &line);