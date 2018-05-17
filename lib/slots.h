#pragma once
#include <vector>

class SlotGameParams
{
  size_t numOfReels() const;
  size_t numOfFruit() const;
  size_t reelsLength(size_t i) const;
  std::vector<std::vector<size_t>> reels;
  std::vector<std::vector<size_t>> winTable;
  bool inc(std::vector<size_t> &line);
public:
  size_t numOfCombinations() const;
  SlotGameParams(size_t _numOfReels, size_t _numOfFruit);
  void readReels(std::string const &pathReelsValue);
  void readWinTable(std::string const &pathWinTable);
  double calcInTheoryRTP();
  double estimateRTP(size_t numOfStarts, unsigned seed = 42);
  double calcPracticallyRTP();
  static bool pointTest(std::vector<std::vector<size_t>> reels, std::vector<std::vector<size_t>> winTable, double realRTP);
  static SlotGameParams randomReels(unsigned seed = 31);
};

size_t checkLine(std::vector<size_t> const &line);
bool randomParamsTest(unsigned seed = 31, size_t numOfStarts = 0, std::string const &pathreels = "");
