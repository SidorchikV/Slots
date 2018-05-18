#pragma once
#include <vector>

typedef std::vector<std::vector<size_t>> reels_t;
typedef std::vector<std::vector<size_t>> wins_t;
typedef std::pair<size_t, size_t> rational_t;

reels_t readReels(std::istream &fin);
wins_t readWinTable(std::istream &fin);

class SlotGameParams
{
  reels_t reels;
  wins_t winTable;
  bool inc(std::vector<size_t> &line);
  std::vector<std::vector<size_t>> countProbabilityTable();
  std::vector<size_t> countDenominators();
  SlotGameParams();
  SlotGameParams(size_t _numOfReels, size_t _numOfFruit);
public:
  SlotGameParams(reels_t &reels, wins_t &winTable);

  SlotGameParams(SlotGameParams&& o);
  SlotGameParams &operator=(SlotGameParams &&o);
  SlotGameParams(SlotGameParams const &o) = delete;
  SlotGameParams &operator=(SlotGameParams const &o) = delete;

  size_t numOfReels() const;
  size_t numOfFruit() const;
  size_t reelsLength(size_t i) const;
  size_t numOfCombinations() const;

  rational_t calcRTPUsingFrequency();
  rational_t estimateRTP(size_t numOfStarts, unsigned seed = 42);
  rational_t calcRTPBruteForce();
  static bool pointTest(reels_t reels, wins_t winTable, rational_t realRTP);
  //static SlotGameParams readReels(std::istream &fin1, std::istream &fin2);
  static SlotGameParams randomReels(unsigned seed = 31);
};

size_t checkLine(std::vector<size_t> const &line);
bool randomParamsTest(unsigned seed = 31, size_t numOfStarts = 0, std::string const &pathreels = "");
