#pragma once
#include <vector>

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
  bool inc(std::vector<size_t> &line);
public:
  SlotGameParams(size_t _numOfReels, size_t _numOfFruit);
  void readReelsValue(std::string const &pathReelsValue);
  void readWinTable(std::string const &pathWinTable);
  double estimateRTP();
  double approxRTP(size_t numOfStarts, unsigned seed = 42);
  double calcRTP();
  static SlotGameParams* randomReels(unsigned seed = 31);
};

size_t checkLine(std::vector<size_t> const &line);