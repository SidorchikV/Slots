#pragma once
#include <vector>

typedef std::vector<size_t> reel_t;
typedef std::vector<reel_t> reels_t;
typedef std::vector<size_t> iline_t;
typedef std::vector<size_t> line_t;
typedef std::vector<size_t> win_line_t;
typedef std::vector<win_line_t> wins_t;
typedef std::vector<std::vector<size_t>> prob_table_t;
typedef std::pair<size_t, size_t> rational_t;
typedef std::vector<size_t> denominators_t;

reels_t readReels(std::istream &fin);
wins_t readWinTable(std::istream &fin, size_t reels_size);

class SlotGameParams
{
  reels_t reels;
  wins_t winTable;

  bool inc(iline_t &line) const;
  prob_table_t countProbabilityTable() const;
  denominators_t countDenominators() const;

  SlotGameParams();
  SlotGameParams(size_t numOfReels, size_t numOfFruit);
public:
  SlotGameParams(reels_t const &reels, wins_t const &winTable);
  static SlotGameParams randomReels(unsigned seed = 31);

  SlotGameParams(SlotGameParams&& o);
  SlotGameParams &operator=(SlotGameParams &&o);
  SlotGameParams(SlotGameParams const &o) = delete;
  SlotGameParams &operator=(SlotGameParams const &o) = delete;

  size_t numOfReels() const;
  size_t numOfFruit() const;
  size_t reelLength(size_t reel_no) const;
  size_t numOfCombinations() const;

  rational_t calcRTPUsingFrequency() const;
  rational_t estimateRTP(size_t numOfStarts, unsigned seed = 42) const;
  rational_t calcRTPBruteForce() const;
  //static SlotGameParams readReels(std::istream &fin1, std::istream &fin2);
};

