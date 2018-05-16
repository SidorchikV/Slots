#pragma once
#include <vector>

typedef struct testStruct
{
  std::vector<std::vector<size_t>> reelsValue;
  std::vector<std::vector<size_t>> winTable;
  double realRTP;
};

class SlotGameParams
{
  size_t numOfReels();
  size_t numOfFruit();
  size_t reelsLength(size_t i);
  std::vector<std::vector<size_t>> reelsValue;
  std::vector<std::vector<size_t>> winTable;
  std::vector<std::vector<size_t>> tableOfProb;
  void countProbabilityTable();
  bool inc(std::vector<size_t> &line);
public:
  size_t numOfCombinations();
  SlotGameParams(size_t _numOfReels, size_t _numOfFruit);
  void readReels(std::string const &pathReelsValue);
  void readWinTable(std::string const &pathWinTable);
  double estimateRTP();
  double approxRTP(size_t numOfStarts, unsigned seed = 42);
  double calcRTP();
  static bool pointTest(testStruct& TS);
  static SlotGameParams randomReels(unsigned seed = 31);
};

size_t checkLine(std::vector<size_t> const &line);
bool randomParamsTest(unsigned seed = 31, size_t numOfStarts = 0, std::string const &pathReelsValue = "");
